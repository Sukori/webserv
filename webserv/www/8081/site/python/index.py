#!/usr/bin/env python3
"""
Webserv CGI Test Page – Python

This file tests Python-CGI execution through webserv
Accessible via: http://localhost:8081/index.py
"""

import os
import sys

# Print HTTP headers
print("Content-Type: text/html")

# HTML Output
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Webserv Python CGI Test</title>")
print("<style>")
print("body { font-family: Arial, sans-serif; margin: 2rem; background: #f5f5f5; }")
print("div { background: white; padding: 1rem; margin: 1rem 0; border-radius: 8px; }")
print("h1 { color: #667eea; }")
print("code { background: #f0f0f0; padding: 0.2rem 0.4rem; border-radius: 3px; }")
print("pre { background: #f0f0f0; padding: 1rem; overflow-x: auto; }")
print("</style>")
print("</head>")
print("<body>")

print("<h1>✅ Python CGI Execution Test</h1>")
print("<p>This page is being served through <strong>CGI execution</strong>, not as a static file.</p>")

# Server Information
print("<div>")
print("<h2>Server Information</h2>")
print("<ul>")
print(f"<li><strong>Python Version:</strong> {sys.version.split()[0]}</li>")
if 'SERVER_SOFTWARE' in os.environ:
    print(f"<li><strong>SERVER_SOFTWARE:</strong> {os.environ['SERVER_SOFTWARE']}</li>")
if 'REQUEST_METHOD' in os.environ:
    print(f"<li><strong>REQUEST_METHOD:</strong> {os.environ['REQUEST_METHOD']}</li>")
if 'SCRIPT_FILENAME' in os.environ:
    print(f"<li><strong>SCRIPT_FILENAME:</strong> {os.environ['SCRIPT_FILENAME']}</li>")
if 'QUERY_STRING' in os.environ:
    print(f"<li><strong>QUERY_STRING:</strong> {os.environ['QUERY_STRING']}</li>")
print("</ul>")
print("</div>")

# Environment Variables
print("<div>")
print("<h2>CGI Environment Variables</h2>")
print("<pre>")
for key in sorted(os.environ.keys()):
    value = os.environ[key]
    print(f"{key} = {value}")
print("</pre>")
print("</div>")

# Body (if POST)
if 'CONTENT_LENGTH' in os.environ:
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    if content_length > 0:
        print("<div>")
        print("<h2>Request Body</h2>")
        print("<pre>")
        body = sys.stdin.read(content_length)
        print(body)
        print("</pre>")
        print("</div>")

# Test Links
print("<div>")
print("<h2>Test Links</h2>")
print("<ul>")
print('<li><a href="/index.py?test=1&name=webserv">Test with GET parameters</a></li>')
print('<li><a href="/">Back to home</a></li>')
print("</ul>")
print("</div>")

print("<footer><p>© 2026 Webserv Development</p></footer>")
print("</body>")
print("</html>")
