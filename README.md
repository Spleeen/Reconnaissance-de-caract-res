Reconnaissance de caractères
============================

 [CHECK] Ajout du constructeur de copie
 [CHECK] Ajout des opérateurs surchargés + -
 [ABANDON] Proposer une transposition inplace qui ne crée pas de nouvelle matrice pour aller plus vite
 [CHECK] Templater pour que l'ensemble fonctionne avec n'importe quel type numérique
 [CHECK] Templater la multiplication avec n'importe quel type numérique
 [CHECK] Puissance d'une matrice
 - Division & soustraction
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


 typedef Matrix<float> matf;
typedef Matrix<double> matd;
 

int main()
{
    matf f(4, 4);
    matd d(4, 4);
 
    matf m = f * d; // devrait être possible et retourner une matd
    matd m = matd(f) * d; // ou éventuellement comme ça