# Parsing
- [ ] Si le premier serveur a qch d'invalide, le second serveur n'est pas initialisé
- [ ] Les chemins vers les fichiers ont besoin d'un prepend "." pour garantir leur accès
- [ ] Définition "/" == "." prepend automatique, ne pas écrire dans la config explicitement
- [ ] "return" n'est pas parsé
- [ ] "route" est parsé dans `Location` -> inutile !

# Runtime
- [ ] Si un serveur a pour nom "ERROR", il est tout de même démarré
	>> Meilleure pratique d'utiliser une variable `bool` pour marquer la validité d'un serveur
- [ ] Si une route est invalide, l'ignorer sans forcément rendre le serveur invalide ?
	>> Besoin de rechercher la meilleure pratique
- [ ] Ne pas bloquer le parsing du serveur 2 si serveur 1 avait une erreur
- [ ] Gérer les `req/res` à l'aide des fonctions `http` (merge effectué)

# CGI
- [ ] Supprimer `cgi_pass` de la config ?
- [ ] Supprimer `cgi_params` de la config ?

# Merge
La branche `merge_buffer` regroupe tous les merges de toutes les branches en date du **18.03.2026 à 12:33**

# README.md
- [ ] aligner le `README.md` selon sujet (à la fin)
