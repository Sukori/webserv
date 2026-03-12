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
- Classe WebServer avec : _config (Configuration), _fds (vector<pollfd>), _clients (map<int, Client>), _socket (int), _socketAddress (sockaddr_in).
- Méthodes : run(), _initServer(), _closeServer(), _acceptConnection(), _handleRequest(Client&), _findBestConfig(host, port).
- Dépendances : Configuration (../../config/), Client (../client/).
- Constantes : BUFFER_SIZE=4096, BACKLOG=1024.

WebServer.cpp :
- Constructeur : configure sockaddr_in à partir du premier listen du premier serveur (front().front()), crée le socket et bind dans _initServer().
- run() : listen(), crée un pollfd pour le socket serveur, boucle infinie avec poll().
  - Si fd == _socket → acceptConnection() → nouveau client ajouté à _clients et _fds (POLLIN).
  - Si POLLIN → readRequest() ; si read_bytes <= 0, fermeture du client ; si requête complète, _handleRequest() puis bascule vers POLLOUT.
  - Si POLLOUT → writeResponse() ; si terminé, fermeture du client. Commentaire personnel de l'humain : envisage de garder la connexion ouverte (keep-alive) au lieu de fermer.
- _acceptConnection() : accept() + fcntl(F_SETFL, O_NONBLOCK) sur le nouveau socket.
- _handleRequest() : stub qui renvoie une réponse HTML "Hello" en dur.

Points identifiés (non résolus, pas d'action prise) :
1. Mono-serveur/mono-listen : seul front() est utilisé, pas de boucle sur les serveurs ni les listens.
2. **(en cours)** inet_addr() à remplacer par getaddrinfo — voir décisions ci-dessous.
3. Le socket serveur (_socket) n'est pas mis en non-bloquant (requis par le sujet).
4. _closeServer() appelle exit(0), ce qui empêche le déroulement normal des destructeurs.
5. _acceptConnection() passe &_socketAddress à accept(), ce qui écrase l'adresse du serveur avec celle du client entrant.

Travail sur le remplacement de inet_addr() par getaddrinfo() — décisions prises :
- getaddrinfo(hostname, service, hints, &res) remplace inet_addr + remplissage manuel de sockaddr_in.
- hostname = ip depuis la config (.c_str()).
- service = port depuis la config, converti en string via std::ostringstream::str().c_str() (le port est un int dans la config, service attend un const char*).
- hints : struct addrinfo avec ai_family=AF_INET, ai_socktype=SOCK_STREAM, ai_flags=AI_PASSIVE (serveur destiné à bind). ai_protocol peut rester à 0 (SOCK_STREAM implique TCP) ou utiliser IPPROTO_TCP pour être explicite.
- res : liste chaînée de résultats (différentes adresses possibles pour un même hostname, pas les clients). Itérer sur la liste pour trouver une adresse valide.
- Libérer la liste avec freeaddrinfo(res) après usage.
- Le sockaddr retourné dans res->ai_addr est directement utilisable pour bind() → le membre _socketAddress (sockaddr_in) de la classe sera remplacé/adapté, car getaddrinfo fournit déjà le sockaddr prêt à l'emploi.
- Gestion d'erreur : getaddrinfo retourne 0 en cas de succès, sinon utiliser gai_strerror() pour le message d'erreur.
Aucun code n'a encore été modifié, uniquement de la réflexion.
<br/>

