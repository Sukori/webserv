# Parsing
- [x] Si le premier serveur a qch d'invalide, le second serveur n'est pas initialisé
- [x] Les chemins vers les fichiers ont besoin d'un prepend "." pour garantir leur accès
- [x] Définition "/" == "." prepend automatique, ne pas écrire dans la config explicitement
- [x] "return" n'est pas parsé
- [x] parse return (code, route)
- [x] "route" est parsé dans `Location` -> inutile !
- [x] limit_except doit être un set, pas vector

# Runtime
- [x] Si un serveur a pour nom ERR_STR, il est tout de même démarré
	>> Meilleure pratique d'utiliser une variable `bool` pour marquer la validité d'un serveur
- [x] Si une route est invalide, l'ignorer sans forcément rendre le serveur invalide ?
	>> Besoin de rechercher la meilleure pratique
	>> Décision, la route invalide n'est pas bloquante. Vérifier le flag `isValid()` dans req/resp
- [x] Ne pas bloquer le parsing du serveur 2 si serveur 1 avait une erreur
- [x] Gérer les `req/res` à l'aide des fonctions `http` (merge effectué)
	- [x] Client doit seulement lire l'ensemble de la communication sans separer header et body (request complete quand recv() == 0)
	- [x] Handle request doit créer un Http(Bytestring)
	- [x] Handle request va ensuite build response et la stocker dans un ByteString
	- [x] Finalement, stocker dans Client::_responseOut
	- [x] main loop Client::writeResponse
- [x] Si deux serveurs valides sont démarrés, l'un ne répond plus
- [x] Segfault si tous les serveurs sont skip (tous invalides)
- [x] Trouver la route pour handle request
- [x] Segfault

# CGI
- [x] Supprimer `cgi_pass` de la config ?
	>> oui
- [x] Supprimer `cgi_params` de la config ?
	>> oui
- [x] Remplacé par `cgi_bin` dans le serveur

# Merge
La branche `merge_buffer` regroupe tous les merges de toutes les branches en date du **18.03.2026 à 12:33**

# Conformité Sujet (audit 2026-04-06)

## CRITIQUE - Blocage soumission
- [ ] **I/O CGI non pilotées par poll**: Les pipes d'exécution CGI sont lues/écrites HORS boucle poll
	>> Http.cpp L.140, L.185-189: read_all() et exec_cgi() bloquent sans surveillance poll
	>> **Impact**: Grade 0 selon sujet § I/O requirements
	>> **Solution**: Intégrer les FDs de processus CGI au vecteur poll avant fork et lire les pipes via poll
	
- [ ] **DELETE non implémenté**: Méthode acceptée en config mais aucune logique de suppression de fichier
	>> validatorConfig.cpp L.97: "GET" || "POST" || "DELETE" validés
	>> memberFunctionsWebServer.cpp L.83: getResponseBody() ne distingue pas DELETE pour unlink()
	>> **Impact**: Évaluation échouera test DELETE
	>> **Solution**: Ajouter branche DELETE dans _handleRequest(), appeler unlink(file_path) via open/unlink
	
- [ ] **Upload fichier non implémenté**: upload_path parsé mais non utilisé au runtime
	>> Configuration.cpp L.48-49: getUploadPath() exposé mais jamais appelé en réponse
	>> **Impact**: POST vers location /uploads ne permiste aucun fichier
	>> **Solution**: Dans getResponseBody(), si POST + upload_path, ouvrir file avec O_WRONLY|O_CREAT et écrire _body

## MOYEN - Impact test fonctionnel
- [ ] **Redirections ignorées**: location return{...} parsées mais jamais appliquées en runtime
	>> ParserLocation.cpp L.73: parseReturn() accumule dans locStruct.locReturn
	>> memberFunctionsWebServer.cpp L.78-85: Aucune branche pour 301/302/303/307/308
	>> **Solution**: Dans _handleRequest(), vérifier Location::getReturn(), envoyer 301/302 + Location header
	
- [ ] **Directory listing désactivé**: autoindex parsé, jamais utilisé avec opendir/readdir
	>> Configuration.cpp L.44-45: getAutoIndex() retourne bool mais pas appelé
	>> **Solution**: Dans getResponseBody(), pour répertoire sans index, si autoindex=true, lister dossier HTML
	
- [ ] **client_max_body_size non contrôlé**: Limite parsée mais pas vérifiée à la réception
	>> ParserServer.cpp L.222: parseBodySize() OK
	>> Client.cpp L.45-53: readRequest() accumule sans limite
	>> **Solution**: Dans readRequest(), comparer _request.length() vs server.getMaxBodySize(), throw 413
	
- [ ] **Chunked transfer encoding absent**: Transfer-Encoding: chunked non décodé
	>> Http.cpp L.122-124: _parseBody() n'accepte que Content-Length
	>> **Impact**: POST chunké échoue ou corrompt le body
	>> **Solution**: Détecter Transfer-Encoding header, unchunker le body avant traitement CGI

## MINEURE - Design/robustesse
- [ ] **Route matching naïf**: Location::getLocation() utilise rfind() simple sans hiérarchie
	>> Configuration.cpp L.114-115: TODO "implement a proper route check ABSOLUTELY"
	>> Exemple: /uploads vs /upload peuvent matcher anomalement
	>> **Solution**: Implémenter longest-prefix match ou trier locations par longueur
	
- [ ] **Errno check interdit**: Le code peut checker errno après read/write (sujet l'interdit)
	>> À vérifier: rechercher errno usage après I/O pour confirmation
	
- [ ] **Signalisation hangup client**: Pas de détection fermeture volontaire client mid-requête
	>> Client.cpp L.45-52: bytesRead == 0 ferme mais pas d'état intermediate
	>> Solution: timeout inactivité sur poll(), cleanup FD client

# README.md
- [ ] aligner le `README.md` selon sujet (à la fin)
