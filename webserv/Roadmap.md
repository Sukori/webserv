# Roadmap Conformité Webserv – Sujet HTTP

**Statut**: Audit 2026-04-06 → Plan de correction en 3 phases
**Objectif**: Passer de conformité partielle ~40% à soumission ~95%+

---

## Phase 1: CRITIQUE (Blocage Zéro)
**Durée estimée**: 6–8 heures
**Raison**: Sujet grade 0 si non-respecté; test eval-bot déterministe sur ces points

### 1.1 – I/O CGI sous poll (Priorité P0)
**Problème**: CGI pipes lues/écrites blocking hors boucle poll → **Grade 0**

**Fichiers affectés**:
- `core/http/Http.cpp` (L.140, L.185–189) – `read_all()`, `exec_cgi()`
- `core/server/mainLoopWebServer.cpp` (L.47) – poll() ne surveille sockets + CGI

**Étapes de correction**:
1. Modifier `WebServer` pour stocker map `<pid, cgi_state>` (socket FD, pipe FD, buffer_out, buffer_in)
2. Dans boucle poll, ajouter FDs pipe CGI au vecteur `_fds`
3. Créer `_handleCGIOutput()` pour lire pipe via POLLIN
4. Créer `_handleCGIInput()` pour écrire au pipe via POLLOUT
5. Adapter boucle `mainLoopWebServer.cpp` pour dispatcher vers CGI handler
6. Nettoyer state CGI et fermer processes via `waitpid()`

**Ressources clés**: `core/cgi/Cgi.cpp`, `core/server/WebServer.hpp` members

---

### 1.2 – DELETE implémentation (Priorité P1)
**Problème**: Méthode annoncée valide mais logique absente → échouera test requête DELETE

**Fichiers affectés**:
- `core/server/memberFunctionsWebServer.cpp` (L.83) – branche manquante pour DELETE
- `core/http/Http.cpp` (L.xxx) – pas de logique unlink()

**Étapes de correction**:
1. Dans `_handleRequest()`, après `verifyMethod()`, tester `req.getStartLine().method == "DELETE"`
2. Pour DELETE: `Http::deleteFile(file_path)` → `unlink(file_path.c_str())`
3. Retourner 204 No Content si OK, 404 si fichier n'existe pas, 403 si permission refusée
4. Ajouter fonction statique `Http::deleteFile()` qui retourne le code d'erreur

**Ressources clés**: `core/server/memberFunctionsWebServer.cpp` L.66–90

---

### 1.3 – Upload fichier (Priorité P1)
**Problème**: POST vers `/uploads` ne persiste rien → fonctionnalité clé du sujet

**Fichiers affectés**:
- `core/http/Http.cpp` (L.170–189) – `getResponseBody()` branche POST
- `config/Configuration.hpp/cpp` – `getUploadPath()` exposé mais non utilisé

**Étapes de correction**:
1. Dans `getResponseBody()`, si `_startline.method == "POST"` ET route a `upload_path`:
   - Construire chemin de destination: `upload_path + "/" + nom_fichier`
   - Ouvrir FD en écriture: `open(dest_path, O_WRONLY | O_CREAT | O_EXCL, 0644)`
   - Écrire `_body` complètement: `write(fd, _body.data(), _body.length())`
   - Retourner 201 Created ou 200 OK
2. Gérer collision de nom: renommer dynamiquement (upload0.txt, upload1.txt, etc.)

**Ressources clés**: `core/http/Http.cpp` L.145–190, `config/Configuration.cpp` L.48

---

## Phase 2: MOYEN (Fonctionnalités Obligatoires)
**Durée estimée**: 4–6 heures
**Raison**: Évaluation pénalisera sévèrement absence de fonctionnalités config annoncées

### 2.1 – Redirections HTTP (301/302)
**Fichiers affectés**: `core/server/memberFunctionsWebServer.cpp`, `core/http/Http.cpp`

**Étapes**:
1. Dans `_handleRequest()`, après route match, vérifier `Location::getReturn()` non-vide
2. Si return{code, path} présent → construire réponse 301/302 + `Location:` header
3. Implémenter statique `Http::buildRedirectResponse()` qui retourne réponse avec status code + headers

---

### 2.2 – Directory Listing (autoindex)
**Fichiers affectés**: `core/http/Http.cpp` L.145–160

**Étapes**:
1. Déterminer si path est répertoire (utiliser `stat()`)
2. Si répertoire sans index fichier ET `location.getAutoIndex() == true`:
   - Appeler `Http::buildDirectoryListing(dir_path)` → génère HTML liste fichiers
   - Utiliser `opendir()`, `readdir()`, `closedir()`
3. Retourner HTML avec 200 OK

---

### 2.3 – client_max_body_size enforcement
**Fichiers affectés**: `core/client/Client.cpp` L.40–60

**Étapes**:
1. Passer `const Server*` référence à `Client::readRequest()`
2. Avant chaque `append()`, vérifier: `_request.length() < server->getMaxBodySize()`
3. Si dépassement → lever exception 413 Payload Too Large
4. Adapter mainLoop pour passer server au readRequest

---

### 2.4 – Transfer-Encoding: chunked
**Fichiers affectés**: `core/http/Http.cpp` `_parseBody()`

**Étapes**:
1. Vérifier header `Transfer-Encoding` (pas Content-Length)
2. Implémenter unchunking: boucle lire "size\r\n", données, "\r\n"
3. Accumuler dans `_body` jusqu'à chunk size 0
4. Passer body dechunké au CGI

---

## Phase 3: MINEURE (Robustesse)
**Durée estimée**: 2–3 heures
**Raison**: Évaluation n'échouera pas mais note baissera si absents

### 3.1 – Route matching hiérarchique
**Fichiers affectés**: `config/Configuration.cpp` L.110–120

**Étapes**:
1. Trier `_locations` par longueur de route (DESC)
2. Matcher première location qui répond à longest-prefix
3. Exemple: `/api/users` match avant `/api`

---

### 3.2 – Timeout inactivité client
**Fichiers affectés**: `core/server/mainLoopWebServer.cpp` L.47

**Étapes**:
1. Changer `poll(-1, ...)` par `poll(timeout=30000)` (30 sec)
2. Si timeout → nettoyer clients inactifs depuis >120 sec
3. Éviter DoS requête incomplete

---

### 3.3 – Audit errno après I/O
**Fichiers affectés**: grep récursif `errno` usage

**Étapes**:
1. Vérifier sujet § "Checking errno strictly forbidden"
2. Remplacer vérifications post-read/write par signaux poll (revents)

---

## Checklist Test Manuel

Après Phase 1, avant Phase 2:
```bash
# Build
make clean && make

# Démarrer
./webserv config/config_files/default.conf

# Test DELETE
curl -X DELETE http://localhost:8081/www/test.txt

# Test Upload POST
curl -X POST --data-binary @file.bin http://localhost:8081/uploads/

# Test GET/POST (doit fonctionner)
curl http://localhost:8081/
```

---

## Estimé Global
- **Phase 1**: 6–8 h (MUST)
- **Phase 2**: 4–6 h (SHOULD)
- **Phase 3**: 2–3 h (NICE)
- **Tests + debug**: 2–3 h
- **Buffer**: ~15–20 h total pour 95% conformité

---

## Ordre recommandé
1. **Jour 1**: Phase 1.1 (I/O CGI poll) – impact le plus grave
2. **Jour 1–2**: Phase 1.2–1.3 (DELETE, Upload)
3. **Jour 2–3**: Phase 2 (Redirects, autoindex, body size, chunked)
4. **Jour 3**: Phase 3 (Robustesse)
5. **Buffer**: Tests eval + fixes dernier minuit
