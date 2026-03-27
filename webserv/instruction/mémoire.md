# Mémoire de travail

Veuillez respecter le format de log en indiquant la date au début de chaque nouvelle session de travail.
La date est au format [AAAMMJJ].
Le log est au format:
```
[AAAAMMJJ]
logs

<br/>
```

[20260214]
élaboration du document d'instructions par l'humain.
Aucune relecture par le Copilot lors de cette session.
Travail uniquement de tête.
écriture sur VIM, Fedora linux 43.
Ce niveau de détail n'est pas primordial, mais donne un exemple de format.
<br/>

[20260310]
Phase de démarrage — prise de connaissance du projet par le Copilot.
Documents lus : `instruction/agent.md`, `instruction/README.md`, `instruction/mémoire.md`, `flowchart.txt`.
Fichiers présents dans le workspace : `WebServer.cpp`, `WebServer.hpp`, `run.cpp`, `Makefile`.

Contexte projet :
- Serveur HTTP en C++98, POSIX-compliant, cible Linux Ubuntu.
- I/O non-bloquante pilotée par un seul poll() (ou équivalent). Pas de read/write sans poll() préalable (sauf fichiers disque).
- Méthodes requises : GET, POST, DELETE. Support CGI, upload fichiers, sites statiques.
- Fonctions externes autorisées listées dans le README (socket, poll, bind, listen, accept, send, recv, etc.).
- Format de config inspiré de NGINX mais personnalisé (syntaxe propre, plus facile à parser). Pas encore disponible dans le workspace.

Réflexion en cours (flowchart.txt) :
- Flux principal esquissé : WebServ(config) → initServer() (socket + bind) → run() (listen, pollfd, boucle poll).
- Question ouverte de l'humain : gestion multi-serveurs (boucle sur tous les serveurs).
- Point d'attention repéré par l'humain sur acceptConnection().

Focus de la session : le serveur et la boucle principale (poll, accept, gestion des connexions).
Aucun code n'a été écrit ou modifié durant cette phase de démarrage.

État du code — analyse des sources :

run.cpp (main de la branche de test) :
- Crée une Configuration en dur : 1 serveur, 1 listen (127.0.0.1:8080), 1 location ("/").
- Instancie WebServer(config) puis appelle webserv.run().
- La branche principale utilise un Parser pour produire la Configuration, le flux reste identique.

WebServer.hpp :
- Classe WebServer avec : _config (Configuration), _fds (vector<pollfd>), _clients (map<int, Client>), _serverSockets (int), _serverSocketsAddress (sockaddr_in).
- Méthodes : run(), _initServer(), _closeServer(), _acceptConnection(), _handleRequest(Client&), _findBestConfig(host, port).
- Dépendances : Configuration (../../config/), Client (../client/).
- Constantes : BUFFER_SIZE=4096, BACKLOG=1024.

WebServer.cpp :
- Constructeur : configure sockaddr_in à partir du premier listen du premier serveur (front().front()), crée le socket et bind dans _initServer().
- run() : listen(), crée un pollfd pour le socket serveur, boucle infinie avec poll().
  - Si fd == _serverSockets → acceptConnection() → nouveau client ajouté à _clients et _fds (POLLIN).
  - Si POLLIN → readRequest() ; si read_bytes <= 0, fermeture du client ; si requête complète, _handleRequest() puis bascule vers POLLOUT.
  - Si POLLOUT → writeResponse() ; si terminé, fermeture du client. Commentaire personnel de l'humain : envisage de garder la connexion ouverte (keep-alive) au lieu de fermer.
- _acceptConnection() : accept() + fcntl(F_SETFL, O_NONBLOCK) sur le nouveau socket.
- _handleRequest() : stub qui renvoie une réponse HTML "Hello" en dur.

Points identifiés (non résolus, pas d'action prise) :
1. Mono-serveur/mono-listen : seul front() est utilisé, pas de boucle sur les serveurs ni les listens.
2. **(en cours)** inet_addr() à remplacer par getaddrinfo — voir décisions ci-dessous.
3. Le socket serveur (_serverSockets) n'est pas mis en non-bloquant (requis par le sujet).
4. _closeServer() appelle exit(0), ce qui empêche le déroulement normal des destructeurs.
5. _acceptConnection() passe &_serverSocketsAddress à accept(), ce qui écrase l'adresse du serveur avec celle du client entrant.

Travail sur le remplacement de inet_addr() par getaddrinfo() — décisions prises :
- getaddrinfo(hostname, service, hints, &res) remplace inet_addr + remplissage manuel de sockaddr_in.
- hostname = ip depuis la config (.c_str()).
- service = port depuis la config, converti en string via std::ostringstream::str().c_str() (le port est un int dans la config, service attend un const char*).
- hints : struct addrinfo avec ai_family=AF_INET, ai_socktype=SOCK_STREAM, ai_flags=AI_PASSIVE (serveur destiné à bind). ai_protocol peut rester à 0 (SOCK_STREAM implique TCP) ou utiliser IPPROTO_TCP pour être explicite.
- res : liste chaînée de résultats (différentes adresses possibles pour un même hostname, pas les clients). Itérer sur la liste pour trouver une adresse valide.
- Libérer la liste avec freeaddrinfo(res) après usage.
- Le sockaddr retourné dans res->ai_addr est directement utilisable pour bind() → le membre _serverSocketsAddress (sockaddr_in) de la classe sera remplacé/adapté, car getaddrinfo fournit déjà le sockaddr prêt à l'emploi.
- Gestion d'erreur : getaddrinfo retourne 0 en cas de succès, sinon utiliser gai_strerror() pour le message d'erreur.
Aucun code n'a encore été modifié, uniquement de la réflexion.
<br/>

[20260312]
Session de travail — refactoring du constructeur, `_initServer()` et `_closeServer()`.
Documents lus en début de session : `instruction/agent.md`, `instruction/README.md`, `instruction/mémoire.md`.

Modifications réalisées dans `WebServer.cpp` et `WebServer.hpp` :

1. **getaddrinfo implémenté** — remplacement effectif de `inet_addr()`.
   - `hints` est maintenant une variable locale séparée (struct addrinfo sur la stack).
   - `_addrinfo` n'est plus un membre de la classe ; `getaddrinfo` est appelé dans la boucle du constructeur, et `freeaddrinfo` libère à chaque itération.
   - Le `new(struct addrinfo)` initial a été supprimé (getaddrinfo alloue lui-même).

2. **Support multi-serveurs dans le constructeur** :
   - Le constructeur boucle sur `_config.getServers()` au lieu de n'utiliser que `front()`.
   - Chaque itération : `getaddrinfo()` → `_initServer(addrinfo, &server)` → `freeaddrinfo()`.

3. **`_serverSockets` est maintenant `std::map<int, const Server*>`** (anciennement `int _socket`) :
   - Clé = fd du listening socket, valeur = pointeur const vers le `Server` associé.
   - Pointeurs stables car `Configuration` (et son vector de `Server`) est const et ne change jamais.

4. **`_initServer()` refactoré** :
   - Signature : `int _initServer(const struct addrinfo* addrinfo, const Server* server)`.
   - Crée le socket, vérifie l'erreur sur le fd, insère dans `_serverSockets`, puis bind avec `addrinfo->ai_addr` et `addrinfo->ai_addrlen`.

5. **`_closeServer()` corrigé** :
   - `exit(0)` supprimé (point 4 de la session précédente résolu).
   - Itération avec `std::map::iterator` en C++98 pour fermer tous les listening sockets.
   - Type de retour à changer de `int` vers `void` (identifié, pas encore appliqué).

6. **`sizeof` corrigé dans `bind()`** : utilisation de `addrinfo->ai_addrlen` au lieu de `sizeof(pointeur)`.

Points résolus (par rapport à la session précédente) :
- Point 1 : multi-serveurs dans le constructeur, _initServer, run, _acceptConnection ✅
- Point 2 : inet_addr → getaddrinfo ✅
- Point 3 : listening sockets mis en non-bloquant via fcntl(F_SETFL, O_NONBLOCK) dans _initServer ✅
- Point 4 : exit(0) dans _closeServer supprimé ✅
- Point 5 : _socketAddress supprimé, accept() utilise NULL, NULL ✅

Corrections supplémentaires appliquées durant la session :
- `_closeServer()` : type de retour changé de `int` à `void`.
- Designated initializers (`{.ai_family = ...}`) remplacés par assignation membre par membre (C++98).
- `service.str("")` ajouté en fin de boucle du constructeur pour réinitialiser l'ostringstream.
- `_acceptConnection(int fd)` : reçoit le fd du listening socket, accept(fd, NULL, NULL), fcntl non-bloquant.
- `run()` : boucle d'init listen + pollfd sur tous les listening sockets. `_serverSockets.find()` distingue listening socket vs client dans la main loop.

Points à traiter avant/pendant le merge :
- ~~**freeaddrinfo()**~~ : ajouté dans la boucle du constructeur après chaque _initServer ✅
- ~~**setsockopt(SO_REUSEADDR)**~~ : ajouté dans _initServer, avant bind, avec check d'erreur ✅
- ~~**fcntl F_GETFL**~~ : corrigé — double appel F_GETFL puis F_SETFL avec OR bitwise, chacun avec check d'erreur ✅
- **Gestion d'erreurs** : exitWithError appelle exit(1) partout. Si un socket sur N échoue, les autres restent ouverts sans cleanup. À améliorer à terme.

Séquence complète de _initServer (état final) :
1. socket(AF_INET, SOCK_STREAM, 0)
2. fcntl(F_GETFL) → fcntl(F_SETFL, flags | O_NONBLOCK)
3. setsockopt(SOL_SOCKET, SO_REUSEADDR, &1, sizeof(int))
4. insert dans _serverSockets map
5. bind(sockBuff, addrinfo->ai_addr, addrinfo->ai_addrlen)
Chaque étape a son check d'erreur avec close(sockBuff) en cas d'échec.

Prochaine étape : merge de la branche server et test en conditions réelles.
<br/>

[20260324]
l'humain a corrigé :
- correction du leak de données en fin de main `delete(config)`
- correction du bug de création de socket corrompu (vidange du flux après utilisation dans `getaddrinfo()`)
- fin de programme si aucun socket n'est créé avec succès
- Default.conf ajusté pour refléter la config valide avec tous les champs possibles et minimaux
- validation de return dans validator
- location invalide est non bloquante pour démarrer un serveur
	>> la route est marquée invalide et peut être ignorée en cas de tentative d'accès
- parse return devrait être une fonction à part (code, route)
	>> recherche de bonne pratique
	>> choix de valider URL seule (default 302), code seul (204, 4xx, 5xx) ou paire code URL (3xx, URL)
