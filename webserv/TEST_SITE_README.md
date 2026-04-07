# Test Site Web Statique – Webserv

## 📁 Structure de Test

```
www/
├── html/                          # Site statique principal (root /www/html)
│   ├── index.html                 # Accueil avec guide de test
│   ├── about.html                 # Page statique simple
│   ├── form.html                  # Formulaire POST
│   ├── download-me.txt            # Fichier GET/DELETE
│   ├── README.md                  # Documentation locale
│   ├── assets/
│   │   └── css/
│   │       └── style.css          # Feuille de style commune
│   └── files/                     # Dossier avec autoindex
│       ├── test1.txt
│       ├── test2.txt
│       └── document.pdf
├── uploads/                       # Dossier pour uploads POST (vide)
├── php/
│   └── index.php                  # Test CGI PHP
└── python/
    └── index.py                   # Test CGI Python
```

## 🚀 Quick Start

### 1. Compiler le serveur
```bash
cd /code/webserv/webserv
make clean && make
```

### 2. Lancer le serveur
```bash
./webserv config/config_files/default.conf
```

### 3. Tester dans un autre terminal
```bash
# Via navigateur
open http://localhost:8081/

# Ou en ligne de commande
curl http://localhost:8081/
```

## ✅ Points Testables

| Cas | Commande | Attendu |
|-----|----------|---------|
| **GET** | `curl http://localhost:8081/` | 200 OK + HTML |
| **GET statique** | `curl http://localhost:8081/about.html` | 200 OK |
| **GET CSS** | `curl http://localhost:8081/assets/css/style.css` | 200 OK |
| **POST upload** | `curl -X POST --data-binary @file.txt http://localhost:8081/uploads/` | 201 Created ou 200 OK |
| **DELETE** | `curl -X DELETE http://localhost:8081/download-me.txt` | 204 No Content ou 200 OK |
| **Autoindex** | `curl http://localhost:8081/files/` | HTML list fichiers |
| **404** | `curl http://localhost:8081/nonexistent` | 404 Not Found |
| **405** | `curl -X PUT http://localhost:8081/about.html` | 405 Method Not Allowed |
| **Redirection** | `curl -v http://localhost:8081/tests` | 301/302 + Location header |
| **CGI PHP** | `curl http://localhost:8081/index.php` | 200 OK + PHP output |
| **CGI Python** | `curl http://localhost:8081/index.py` | 200 OK + Python output |

## 📋 Configuration Correspondante

La config `config/config_files/default.conf` est configurée pour servir ce contenu:

```nginx
server {
    listen 8081
    server_name weebserv
    root /
    
    location / {
        root /www/html
        limit_except GET POST;
        autoindex true
    }
    
    location /uploads {
        limit_except POST;
        root /uploads
        upload_path /uploads
    }
    
    location /www {
        root /www
        # CGI pour .php et .py
    }
    
    location /tests {
        return 301 https://matias.me/nsfw/;
    }
    
    error_pages {
        404 /www/error_pages/404.html;
        405 /www/error_pages/405.html;
    }
}
```

## 🧪 Utiliser le Test Suite

Un script `test_suite.sh` est fourni pour automatiser les tests:

```bash
# Rendre le script exécutable
chmod +x test_suite.sh

# Lancer les tests
./test_suite.sh localhost 8081

# Résultat attendu:
# ====================================
#  Webserv Conformité Test Suite
# ====================================
# Testing: http://localhost:8081
# 
# Testing: GET / ... ✅ PASS
# Testing: GET /about.html ... ✅ PASS
# ...
# ====================================
#  Résultats
# ====================================
# ✅ PASS: 10
# ❌ FAIL: 0
# Total: 10
# 
# 🎉 Tous les tests passent!
```

## 🔍 Cas de Test Détaillés

### 1. GET – Pages Statiques
```bash
curl http://localhost:8081/                # index.html
curl http://localhost:8081/about.html      # about.html
curl http://localhost:8081/form.html       # formulaire
curl http://localhost:8081/assets/css/style.css
```

### 2. POST – Upload Fichier
```bash
# Créer un fichier test
echo "Contenu de test" > /tmp/upload_test.txt

# Uploader
curl -X POST --data-binary @/tmp/upload_test.txt \
    http://localhost:8081/uploads/upload_test.txt

# Vérifier que le fichier existe
curl http://localhost:8081/uploads/upload_test.txt
```

### 3. DELETE – Suppression Fichier
```bash
# Supprimer download-me.txt
curl -X DELETE http://localhost:8081/download-me.txt

# Vérifier que le fichier est supprimé (404 attendu)
curl http://localhost:8081/download-me.txt
```

### 4. Directory Listing (Autoindex)
```bash
# Dossier /files/ avec autoindex=true
curl http://localhost:8081/files/

# Résultat: HTML contenant la liste des fichiers
# test1.txt, test2.txt, document.pdf
```

### 5. Erreurs HTTP
```bash
# 404 – Fichier inexistant
curl -v http://localhost:8081/nonexistent.html

# 405 – Méthode non autorisée
curl -X POST http://localhost:8081/about.html   # GET-only

# 413 – Body trop gros (si client_max_body_size dépassé)
dd if=/dev/zero bs=1M count=100 | curl -X POST --data-binary @- \
    http://localhost:8081/uploads/
```

### 6. Redirections HTTP
```bash
# Voir la redirection (suivre avec -L)
curl -L http://localhost:8081/tests

# Ou afficher les headers bruts
curl -v http://localhost:8081/tests
# Expected output:
# HTTP/1.1 301 Moved Permanently
# Location: https://matias.me/nsfw/
```

### 7. CGI – Scripts Dynamiques
```bash
# PHP
curl http://localhost:8081/index.php

# Python
curl http://localhost:8081/index.py

# Avec paramètres GET
curl http://localhost:8081/index.php?name=test&value=123
```

### 8. Multi-Port
```bash
# Port 8080 (minimal)
curl http://localhost:8080/

# Port 8081 (principal)
curl http://localhost:8081/

# Port 8082 (minimal)
curl http://localhost:8082/
```

## 📊 Tableau de Couverture Sujet

| Fonctionnalité | Fichiers | Statut |
|---|---|---|
| GET statique | `html/*.html`, `assets/css/*` | ✅ Prêt |
| POST upload | `form.html`, dossier `uploads/` | ✅ Prêt |
| DELETE | `download-me.txt` | ✅ Prêt |
| Autoindex | `files/` | ✅ Prêt |
| Erreurs 404/405 | `error_pages/` | ✅ Prêt |
| Redirections | Config location /tests | ✅ Prêt |
| CGI PHP | `php/index.php` | ✅ Prêt |
| CGI Python | `python/index.py` | ✅ Prêt |
| client_max_body_size | Config server | ✅ Prêt |
| Directory listing | Dossier `files/` | ✅ Prêt |

## 🛠️ Dépannage

### Le serveur ne démarre pas
```bash
# Vérifier la compilation
make clean && make

# Vérifier que le port est libre
lsof -i :8081

# Vérifier la syntaxe du config
./webserv --check config/config_files/default.conf
```

### Les fichiers ne se chargent pas
```bash
# Vérifier les permissions
ls -la www/html/
chmod -R 755 www/

# Vérifier les chemins dans la config
cat config/config_files/default.conf | grep root
```

### CGI ne s'exécute pas
```bash
# Vérifier que php-cgi et python3 sont installés
which php-cgi
which python3

# Vérifier les droits d'exécution
ls -la www/php/index.php
chmod +x www/python/index.py
```

## 📝 Notes

- Tous les fichiers HTML utilisent la même feuille de style `assets/css/style.css`
- Les fichiers sont en UTF-8, compatible avec les navigateurs modernes
- Le fichier `download-me.txt` est destiné à être supprimé lors du test DELETE
- Le dossier `uploads/` se remplit au fur et à mesure des uploads POST
- La configuration `default.conf` mappe les chemins correctement vers `/www/`

---

**Créé**: 2026-04-06  
**Pour**: Webserv Conformité Sujet HTTP C++98
