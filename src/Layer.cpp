#include "Layer.h"
#include <algorithm>
#include <iostream>

using namespace std;

float Layer::STEP = 0.5f;

//Constructeur de Couche
Layer::Layer(int nbInputs, int nbOutput)
    : _weights(nbOutput, std::vector<float>(nbInputs)),
    _weightedSum(nbOutput, 0.f),
    _results(nbOutput, 0.f),
    _gradients(nbOutput, 0.f)
{
    random_device random;
    uniform_real_distribution<float> distribution(-0.5, 0.5);

    for(int i=0 ; i<nbOutput ; i++)
        for(int j=0 ; j<nbInputs ; j++)
            _weights[i][j] = distribution(random);
}

//Propagation
void Layer::process(const std::vector<float>& inputs)
{
    for(unsigned int i=0 ; i<_results.size() ; i++)
    {
        float weightedSum = 0.f;

        for(unsigned int j=0 ; j<inputs.size() ; j++)
            weightedSum += inputs[j] * _weights[i][j];

        _weightedSum[i] = weightedSum;
        _results[i] = activationFunction(weightedSum);
    }
}

//Rétropropagation du gradient
void Layer::adjust(const std::vector<float>& inputs, const std::vector<float>& expectedResults)
{
    for(unsigned int i=0 ; i<_results.size() ; i++)
    {
        _gradients[i] = (expectedResults[i] - _results[i]) * activationFunctionDerivative(_weightedSum[i]);

        for(unsigned int j=0 ; j<_weights[i].size() ; j++)
            _weights[i][j] += STEP * _gradients[i] * inputs[j];
    }
}

//Ajustement des poids synaptiques
void Layer::adjust(const std::vector<float>& inputs, const Layer& nextLayer)
{
    for(unsigned int i=0 ; i<_results.size() ; i++)
    {
        float localSum = 0.f;

        for(unsigned int j=0 ; j<nextLayer._gradients.size() ; j++)
            localSum += nextLayer._gradients[j] * nextLayer._weights[j][i];

        _gradients[i] = activationFunctionDerivative(_weightedSum[i]) * localSum;

        for(unsigned int j=0 ; j<_weights[i].size() ; j++)
            _weights[i][j] += STEP * _gradients[i] * inputs[j];
    }
}

//Afficher les poids synaptiques finaux
void Layer::show()
{
    for(unsigned int i=0 ; i<_weights.size() ; i++)
    {
        cout << "Neuron " << i+1 << ":" << endl;

        for(unsigned int j=0 ; j<_weights[i].size() ; j++)
            cout << "    poids(" << j+1 << ") = " << _weights[i][j] << endl;

        cout << "    last_result = " << _results[i] << endl;
    }
}

//Calcul de l'erreur
float Layer::error(const std::vector<float>& expectedResults)
{
    float localSum = 0.f;

    for(unsigned int i=0 ; i<_results.size() ; i++)
        localSum += static_cast<float>(pow(expectedResults[i] - _results[i], 2));

    return localSum / 2.0f;
}

//Sortie 
const std::vector<float>& Layer::results()
{
    return _results;
}

//Fonction d'activation sygmoïde
float Layer::activationFunction(float x)
{
    return 1.f / (1.f + exp(-x));
}

//Dérivée de la fonction d'activation sygmoïde
float Layer::activationFunctionDerivative(float x)
{
    return static_cast<float>(exp(x) / static_cast<float>(pow(1.f + exp(x), 2)));
}