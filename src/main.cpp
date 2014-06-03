#include "Utilities.h"
#include "Layer.h"
#include "Pattern.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace Utilities;

//Paramètres par défaut
float PRECISION = 1e-5f;      // Précision pour la convergeance
#define NUM_NEURON_INTER_LAYER 10

int main(int argc, char* argv[])
{

    //Prise en charge des paramètres d'entrés
    if(argc == 1)
    {
        cout <<"* Utilisation d'un pas d'apprentissage de " <<Layer::STEP << " et d'une précision de "<<PRECISION << " (--help)" <<endl;
    }
    else if (argc == 3 && from_string(argv[1],Layer::STEP) && from_string(argv[2], PRECISION)){}
    else 
    {
        cout <<"Usage: neural-network PAS PRECISION" <<endl;
        cout <<"Veuillez indiquer le pas d'apprentissage (positive, souvent très proche de zéro) puis la précision (souvent très petite)" <<endl;
        return EXIT_FAILURE;
    }
    
    //Entrées normalisées
    std::vector <Pattern> patterns;

    const vector<vector<float>> inputsDatabase = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const vector<vector<float>> expectedResults = {{0}, {0}, {0}, {1}};
    vector<int> randomIndexes(inputsDatabase.size());
    Layer interLayer = Layer(inputsDatabase[0].size(), NUM_NEURON_INTER_LAYER);
    Layer finalLayer = Layer(NUM_NEURON_INTER_LAYER, expectedResults[0].size());
    int idRound = 1;
    float error;
    
    //Chargement des patterns
    for (int i = 0; i <= 9; ++i)
    {
         Pattern nvPat("./ressources/patterns.txt",i,9,5);

         patterns.push_back(nvPat);
    }

    t_start;
    for(int i=0 ; i<inputsDatabase.size() ; i++)
        randomIndexes[i] = i;

    // Phase d'apprentissage
    // Tant qu'on a au moins une erreur par cycle
    do
    {
        error = 0.f;
        //Permutation des données d'entrées
        random_shuffle(randomIndexes.begin(), randomIndexes.end());

        // Pour chaque entrée de la base de données
        for(int j : randomIndexes)
        {
            //Propagation avant 1
            interLayer.process(inputsDatabase[j]);
            //Propagation avant 2
            finalLayer.process(interLayer.results());

            //Ajustement des poids synaptiques
            finalLayer.adjust(interLayer.results(), expectedResults[j]);
            interLayer.adjust(inputsDatabase[j], finalLayer);
  
            //On fait un cumul des erreurs E(h)
            error += finalLayer.error(expectedResults[j]);
        }

        //Erreur totale moyenne
        error /= inputsDatabase.size();

        //Pour ne pas en afficher trop
        //if(idRound % 1000 == 0)
          //  cout << "Erreur : " << error << endl;

        idRound++;
    }
    while(error > PRECISION);
    t_end;
    cout <<"* Nombre d'itérations pour l'apprentissage : " <<idRound <<endl;
    cout <<"* Entrées --> résultat du perceptron : " <<endl;
    for(vector<float> inputs : inputsDatabase)
    {
        cout <<"\t";
        for(float v : inputs)
            cout <<v << " ";
        interLayer.process(inputs);
        finalLayer.process(interLayer.results());
        cout << "--> " << int(finalLayer.results()[0] + 0.5f) << endl;
        //cout << "--> " << finalLayer.results()[0] << endl;
    }
    t_show;
    return 0;
}
 
