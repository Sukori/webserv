# 1. GET statiques
curl http://localhost:8081/
curl http://localhost:8081/about.html

# 2. POST upload (une fois implémenté)
curl -X POST --data-binary @file.txt http://localhost:8081/uploads/

# 3. DELETE (une fois implémenté)
curl -X DELETE http://localhost:8081/download-me.txt

# 4. Directory Listing (avec autoindex=true dans config)
curl http://localhost:8081/files/

# 5. Erreurs HTTP
curl http://localhost:8081/nonexistent.html    # 404
curl -X PUT http://localhost:8081/about.html   # 405

# 6. Redirections (config location /tests)
curl -v http://localhost:8081/tests

# 7. CGI PHP + Python
curl http://localhost:8081/index.php
curl http://localhost:8081/index.py

# 8. Test automatisé
./test_suite.sh localhost 8081