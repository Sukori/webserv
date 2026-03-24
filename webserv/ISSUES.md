# Parsing
- [x] Si le premier serveur a qch d'invalide, le second serveur n'est pas initialisé
- [x] Les chemins vers les fichiers ont besoin d'un prepend "." pour garantir leur accès
- [x] Définition "/" == "." prepend automatique, ne pas écrire dans la config explicitement
- [x] "return" n'est pas parsé
- [ ] parse return (code, route)
- [x] "route" est parsé dans `Location` -> inutile !

# Runtime
- [x] Si un serveur a pour nom ERR_STR, il est tout de même démarré
	>> Meilleure pratique d'utiliser une variable `bool` pour marquer la validité d'un serveur
- [ ] Si une route est invalide, l'ignorer sans forcément rendre le serveur invalide ?
	>> Besoin de rechercher la meilleure pratique
- [x] Ne pas bloquer le parsing du serveur 2 si serveur 1 avait une erreur
- [ ] Gérer les `req/res` à l'aide des fonctions `http` (merge effectué)
- [x] Si deux serveurs valides sont démarrés, l'un ne répond plus
- [x] Segfault si tous les serveurs sont skip (tous invalides)

# CGI
- [ ] Supprimer `cgi_pass` de la config ?
	>> pas pour le moment
- [ ] Supprimer `cgi_params` de la config ?
	>> pas pour le moment

# Merge
La branche `merge_buffer` regroupe tous les merges de toutes les branches en date du **18.03.2026 à 12:33**

# README.md
- [ ] aligner le `README.md` selon sujet (à la fin)
