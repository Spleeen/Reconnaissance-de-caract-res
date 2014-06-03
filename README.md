Reconnaissance de caractères
============================

 [CHECK] Chargement des patterns
 - Il ne restera plus qu'à adapter les entrées (précédemment des vectors) aux matrices



Avancée sur les matrices
========================
 [CHECK] Ajout du constructeur de copie
 [CHECK] Ajout des opérateurs surchargés + -
 [ABANDON] Proposer une transposition inplace qui ne crée pas de nouvelle matrice pour aller plus vite
 [CHECK] Templater pour que l'ensemble fonctionne avec n'importe quel type numérique
 [CHECK] Templater la multiplication avec n'importe quel type numérique
 [CHECK] Puissance d'une matrice
 [CHECK] Soustraction 
 - Division (inversibilité obligatoire)
 - Produit scalaire et vectoriel
 - Inverse d'une matrice
 - Déterminant d'une matrice
 - Prévoir une version SSE pour gain de perf
 - bool isSubMatrix()
 - Trouver une façon facile de remplir la matrice (constructeur plus aisé)
 - Faire une méthode static transposed (Matrix)
 - L’implémentation d’operator= pourrait vérifier que le buffer actuel est assez grand pour contenir l’autre matrice et ne pas réallouer dans ce cas
 - Les opérateurs binaires devraient être libres (même si pour les matrices il n'y devrait pas y avoir de grosse différence), et const-correct!
 - Les tests de bornes devraient être des assertions. Elles sont faites pour tester les erreurs de programmation. Elles sont bien plus efficaces à ce jeu que la programmation défensive.
 - Invariant de la classe avec (m!)._values jamais nul.
 - Proposer un class SmallMatrix, LargeMatrix hérité d'un Matrix avec juste une diff sur la gestion de m_values (dynamique ou statique)
 - Permettre : 'matf m = f * d; (qui doit être possible et retourner une matd) OU 'matd(f) * d;'
 - Proposer les quaternions à l'implémentation