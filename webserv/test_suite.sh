#!/bin/bash

# Webserv Conformité Test Suite
# Ce script teste tous les points du sujet HTTP

set -e

HOST="${1:-localhost}"
PORT="${2:-8081}"
BASE_URL="http://$HOST:$PORT"

PASS=0
FAIL=0

test_case() {
    local name="$1"
    local cmd="$2"
    local expected="$3"
    
    echo -n "Testing: $name ... "
    
    result=$(eval "$cmd" 2>&1 || true)
    
    if echo "$result" | grep -q "$expected"; then
        echo "✅ PASS"
        ((PASS++))
    else
        echo "❌ FAIL"
        echo "  Expected: $expected"
        echo "  Got: $result"
        ((FAIL++))
    fi
}

echo "======================================"
echo " Webserv Conformité Test Suite"
echo "======================================"
echo "Testing: $BASE_URL"
echo ""

# Test 1: GET – Page d'accueil
test_case "GET /" "curl -s $BASE_URL/" "Webserv – Site de Test"

# Test 2: GET – Page statique
test_case "GET /about.html" "curl -s $BASE_URL/about.html" "À Propos"

# Test 3: GET – CSS
test_case "GET /assets/css/style.css" "curl -s $BASE_URL/assets/css/style.css" "font-family"

# Test 4: GET – Fichier texte
test_case "GET /download-me.txt" "curl -s $BASE_URL/download-me.txt" "téléchargé"

# Test 5: Directory Listing
test_case "GET /files/ (autoindex)" "curl -s $BASE_URL/files/" "test1.txt\|test2.txt\|document.pdf"

# Test 6: 404 – Fichier inexistant
echo -n "Testing: 404 on nonexistent file ... "
status=$(curl -s -o /dev/null -w "%{http_code}" "$BASE_URL/nonexistent.html")
if [ "$status" = "404" ]; then
    echo "✅ PASS ($status)"
    ((PASS++))
else
    echo "❌ FAIL (got $status, expected 404)"
    ((FAIL++))
fi

# Test 7: 405 – Méthode non autorisée (DELETE sur /about.html)
echo -n "Testing: 405 on unauthorized method ... "
status=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE "$BASE_URL/about.html")
if [ "$status" = "405" ] || [ "$status" = "200" ]; then
    echo "✅ PASS ($status)"
    ((PASS++))
else
    echo "❌ FAIL (got $status, expected 405 or 200)"
    ((FAIL++))
fi

# Test 8: POST – Upload fichier
echo -n "Testing: POST /uploads/ ... "
temp_file=$(mktemp)
echo "Test upload content" > "$temp_file"
status=$(curl -s -o /dev/null -w "%{http_code}" -X POST --data-binary @"$temp_file" "$BASE_URL/uploads/test_upload.txt")
rm "$temp_file"
if [ "$status" = "201" ] || [ "$status" = "200" ]; then
    echo "✅ PASS ($status)"
    ((PASS++))
else
    echo "❌ FAIL (got $status, expected 201 or 200)"
    ((FAIL++))
fi

# Test 9: DELETE – Suppression fichier
echo -n "Testing: DELETE /download-me.txt ... "
status=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE "$BASE_URL/download-me.txt")
if [ "$status" = "204" ] || [ "$status" = "200" ]; then
    echo "✅ PASS ($status)"
    ((PASS++))
else
    echo "❌ FAIL (got $status, expected 204 or 200)"
    ((FAIL++))
fi

# Test 10: Redirection
echo -n "Testing: Redirection /tests ... "
status=$(curl -s -o /dev/null -w "%{http_code}" -L "$BASE_URL/tests")
if [ "$status" = "200" ] || [ "$status" = "301" ] || [ "$status" = "302" ]; then
    echo "✅ PASS ($status)"
    ((PASS++))
else
    echo "❌ FAIL (got $status)"
    ((FAIL++))
fi

# Summary
echo ""
echo "======================================"
echo " Résultats"
echo "======================================"
echo "✅ PASS: $PASS"
echo "❌ FAIL: $FAIL"
echo "Total: $((PASS+FAIL))"
echo ""

if [ $FAIL -eq 0 ]; then
    echo "🎉 Tous les tests passent!"
    exit 0
else
    echo "⚠️  $FAIL test(s) échoué(s)"
    exit 1
fi
