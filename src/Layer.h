/*
*   Created by N.Richard
*   Date de création : 17 novembre 2013
*   Date de version : 2 juin 2014
*   Version : 1.5
*   Spécialement conçu pour l'OCR
*/

#ifndef LAYER_H
#define LAYER_H

#include <vector>

class Layer
{
    public:

    //Constructeur de Couche
    Layer(int nbInputs, int nbOutput);
    //Propagation
    void process(const std::vector<float>& inputs);

    //Rétropropagation du gradient
    void adjust(const std::vector<float>& inputs, const std::vector<float>& expectedResults);

    //Ajustement des poids synaptiques
    void adjust(const std::vector<float>& inputs, const Layer& nextLayer);

    //Afficher les poids synaptiques finaux
    void show();

    //Calcul de l'erreur
    float error(const std::vector<float>& expectedResults);

    //Sortie 
    const std::vector<float>& results();

    static float STEP;            // Le pas d'apprentissage

    private:

    std::vector<std::vector<float> > _weights; // Poids synaptiques
    std::vector<float> _weightedSum;     // Utile pour la rétropropagation
    std::vector<float> _results;         // Sortie des neurones
    std::vector<float> _gradients;       // Utile pour la rétropropagation

    //Fonction d'activation sygmoïde
    float activationFunction(float x);
    
    //Dérivée de la fonction d'activation sygmoïde
    float activationFunctionDerivative(float x);    
};


#endif //LAYER_H
