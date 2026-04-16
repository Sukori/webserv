<?php
/**
 * Webserv CGI Test Page – PHP
 * 
 * This file tests PHP-CGI execution through webserv
 * Accessible via: http://localhost:8081/index.php
 */

echo "<!DOCTYPE html>";
echo "<html><head>";
echo "<title>Webserv PHP CGI Test</title>";
echo "<style>";
echo "body { font-family: Arial, sans-serif; margin: 2rem; background: #f5f5f5; }";
echo "div { background: white; padding: 1rem; margin: 1rem 0; border-radius: 8px; }";
echo "h1 { color: #667eea; }";
echo "code { background: #f0f0f0; padding: 0.2rem 0.4rem; border-radius: 3px; }";
echo "</style></head><body>";

echo "<h1>✅ PHP-CGI Execution Test</h1>";
echo "<p>This page is being served through <strong>CGI execution</strong>, not as a static file.</p>";

echo "<div>";
echo "<h2>Server Information</h2>";
echo "<ul>";
echo "<li><strong>PHP Version:</strong> " . phpversion() . "</li>";
echo "<li><strong>SERVER_SOFTWARE:</strong> " . $_SERVER['SERVER_SOFTWARE'] . "</li>";
echo "<li><strong>REQUEST_METHOD:</strong> " . $_SERVER['REQUEST_METHOD'] . "</li>";
echo "<li><strong>SCRIPT_FILENAME:</strong> " . $_SERVER['SCRIPT_FILENAME'] . "</li>";
echo "<li><strong>QUERY_STRING:</strong> " . ($_SERVER['QUERY_STRING'] ?? 'N/A') . "</li>";
echo "</ul>";
echo "</div>";

echo "<div>";
echo "<h2>Environment Variables</h2>";
echo "<pre>";
foreach ($_SERVER as $key => $value) {
    if (is_string($value)) {
        echo htmlspecialchars($key) . " = " . htmlspecialchars($value) . "";
    }
}
echo "</pre>";
echo "</div>";

echo "<div>";
echo "<h2>GET Parameters</h2>";
if (count($_GET) > 0) {
    echo "<ul>";
    foreach ($_GET as $key => $value) {
        echo "<li>" . htmlspecialchars($key) . " = " . htmlspecialchars($value) . "</li>";
    }
    echo "</ul>";
} else {
    echo "<p>No GET parameters.</p>";
}
echo "</div>";

echo "<div>";
echo "<h2>Test Links</h2>";
echo "<ul>";
echo "<li><a href=\"/index.php?test=1&name=webserv\">Test with GET parameters</a></li>";
echo "<li><a href=\"/\">Back to home</a></li>";
echo "</ul>";
echo "</div>";

echo "<footer><p>© 2026 Webserv Development</p></footer>";
echo "</body></html>";
?>


