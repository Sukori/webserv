# direction basique
1. Ouvrir un socket
2. Bind le socket à une adresse
3. Une fois le bind réussi, listen pour des requêtes client
4. Listen a une loop infinie
5. Dans la loop, on commence par accepter la demande entrante et on la stocke dans un nouveau socket.
6. Pour plus de robustesse et de parallélisation, il faudrait un container de sockets. (minimum 255 pour supporter siege !)
7. read/write - recv/send on lit la requête et on renvoie une réponse, page ou erreur
8. Attention au format du header de la string pour correspondre au standard web (voir branche http !)
9. fermer le socket

