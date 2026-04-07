<?php
/**
 * Webserv CGI Test Page – PHP
 * 
 * This file tests PHP-CGI execution through webserv
 * Accessible via: http://localhost:8081/index.php
 */

echo "<!DOCTYPE html>\n";
echo "<html>\n<head>\n";
echo "<title>Webserv PHP CGI Test</title>\n";
echo "<style>\n";
echo "body { font-family: Arial, sans-serif; margin: 2rem; background: #f5f5f5; }\n";
echo "div { background: white; padding: 1rem; margin: 1rem 0; border-radius: 8px; }\n";
echo "h1 { color: #667eea; }\n";
echo "code { background: #f0f0f0; padding: 0.2rem 0.4rem; border-radius: 3px; }\n";
echo "</style>\n</head>\n<body>\n";

echo "<h1>✅ PHP-CGI Execution Test</h1>\n";
echo "<p>This page is being served through <strong>CGI execution</strong>, not as a static file.</p>\n";

echo "<div>\n";
echo "<h2>Server Information</h2>\n";
echo "<ul>\n";
echo "<li><strong>PHP Version:</strong> " . phpversion() . "</li>\n";
echo "<li><strong>SERVER_SOFTWARE:</strong> " . $_SERVER['SERVER_SOFTWARE'] . "</li>\n";
echo "<li><strong>REQUEST_METHOD:</strong> " . $_SERVER['REQUEST_METHOD'] . "</li>\n";
echo "<li><strong>SCRIPT_FILENAME:</strong> " . $_SERVER['SCRIPT_FILENAME'] . "</li>\n";
echo "<li><strong>QUERY_STRING:</strong> " . ($_SERVER['QUERY_STRING'] ?? 'N/A') . "</li>\n";
echo "</ul>\n";
echo "</div>\n";

echo "<div>\n";
echo "<h2>Environment Variables</h2>\n";
echo "<pre>\n";
foreach ($_SERVER as $key => $value) {
    if (is_string($value)) {
        echo htmlspecialchars($key) . " = " . htmlspecialchars($value) . "\n";
    }
}
echo "</pre>\n";
echo "</div>\n";

echo "<div>\n";
echo "<h2>GET Parameters</h2>\n";
if (count($_GET) > 0) {
    echo "<ul>\n";
    foreach ($_GET as $key => $value) {
        echo "<li>" . htmlspecialchars($key) . " = " . htmlspecialchars($value) . "</li>\n";
    }
    echo "</ul>\n";
} else {
    echo "<p>No GET parameters.</p>\n";
}
echo "</div>\n";

echo "<div>\n";
echo "<h2>Test Links</h2>\n";
echo "<ul>\n";
echo "<li><a href=\"/index.php?test=1&name=webserv\">Test with GET parameters</a></li>\n";
echo "<li><a href=\"/\">Back to home</a></li>\n";
echo "</ul>\n";
echo "</div>\n";

echo "<footer><p>© 2026 Webserv Development</p></footer>\n";
echo "</body>\n</html>\n";
?>


