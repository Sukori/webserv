# Test Site – Webserv Conformité Sujet

## Structure

```
www/html/
├── index.html           (page d'accueil avec guide test)
├── about.html           (page statique simple)
├── form.html            (formulaire POST)
├── download-me.txt      (fichier GET/DELETE)
├── assets/
│   └── css/
│       └── style.css    (feuille de style)
└── files/
    ├── test1.txt
    ├── test2.txt
    └── document.pdf     (test fichier binaire)

www/uploads/             (dossier vide, récipient pour uploads)
```

## Points Testables

### 1. GET – Documents Statiques
```bash
curl http://localhost:8081/
curl http://localhost:8081/about.html
curl http://localhost:8081/assets/css/style.css
```

### 2. POST – Upload Fichier
```bash
# Simple
echo "Test content" | curl -X POST --data-binary @- http://localhost:8081/uploads/test.txt

# Form multipart
curl -F "file=@/path/to/file" http://localhost:8081/uploads/

# Vérifier
curl http://localhost:8081/uploads/
```

### 3. DELETE – Suppression Fichier
```bash
# Supprimer
curl -X DELETE http://localhost:8081/download-me.txt

# Vérifier (404 attendu)
curl http://localhost:8081/download-me.txt
```

### 4. Autoindex – Directory Listing
```bash
# Dossier avec autoindex=true
curl http://localhost:8081/files/

# Dossier uploads doit aussi afficher les uploads récents
curl http://localhost:8081/uploads/
```

### 5. Erreurs HTTP
```bash
# 404 – Fichier inexistant
curl http://localhost:8081/nonexistent.html

# 405 – Méthode non autorisée
curl -X PUT http://localhost:8081/about.html

# 403 – Pas de permission (test manuel)
touch /tmp/forbidden.txt && chmod 000 /tmp/forbidden.txt
curl http://localhost:8081/../forbidden.txt
```

### 6. Redirections (location /tests)
```bash
curl -v http://localhost:8081/tests
# Doit afficher: HTTP/1.1 301/302 + Location: https://matias.me/nsfw/
```

### 7. CGI – Scripts Dynamiques
```bash
# PHP (si disponible)
curl http://localhost:8081/index.php

# Python (si disponible)
curl http://localhost:8081/index.py
```

### 8. Multi-Port
```bash
curl http://localhost:8080/
curl http://localhost:8081/
curl http://localhost:8082/
```

## Configuration Correspondante

Dans `default.conf`:
```nginx
server {
    listen 8081
    root /
    
    location / {
        root /www/html
        limit_except GET POST;
        autoindex true
    }
    
    location /uploads {
        limit_except POST;
        upload_path /uploads
    }
    
    location /files {
        # Héritée de / si pas d'override
        autoindex true
    }
    
    location /tests {
        return 301 https://matias.me/nsfw/;
    }
}
```

## Checklist Conformité

- [ ] GET retourne 200 + contenu
- [ ] POST vers /uploads crée fichier
- [ ] DELETE supprime fichier (204 No Content)
- [ ] /files/ affiche listing HTML
- [ ] /uploads/ affiche listing HTML
- [ ] /tests redirige (301/302)
- [ ] Erreur 404 sur fichier inexistant
- [ ] Erreur 405 sur méthode non permise
- [ ] client_max_body_size = 50M respecté ou erreur 413
- [ ] CGI PHP et Python exécutes (si binaires présents)

## Notes

- La config `default.conf` utilise `root /` au niveau serveur, puis override dans locations
- Les chemins sont relatifs au `root` global ou de location
- Tous les fichiers CSS/JS/images dans `/assets/` sont servis statiquement
