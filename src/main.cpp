#include "Utilities.h"
#include "Layer.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace Utilities;


template <class T>
struct DeleteFunctor : public std::unary_function<T*,void> { void operator() ( T* ptr) const { delete ptr;} };

void loadLearningBase (const std::string filePath, vector<vector<float>>& matInputs, vector<float>& matResults, int nbrows, int nbcolumns);
void loadTest (const std::string filePath, vector<vector<float>>& matInputs, int nbrows, int nbcolumns);
void show (const vector<float>& mat, int nbrows, int nbcolumns);

//Paramètres par défaut
float PRECISION = 1e-6f;      // Précision pour la convergeance
#define INPUTS_FILE_LEARNING ("./ressources/inputs_learning.txt")
#define INPUTS_FILE_TEST ("./ressources/inputs_test.txt")
#define CHAR_WIDTH 5
#define CHAR_LEN 9
#define NUM_NEURON_INTER_LAYER CHAR_WIDTH*CHAR_LEN


class Network {

public:
    Network (Layer inter, Layer final, int id = 1, float err = 0.0) : interLayer(inter), finalLayer (final), idRound(id), error (err){}

    Layer interLayer;
    Layer finalLayer;
    int idRound;
    float error;  
} ;

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
    
    vector<Network*> networks;
    //Chargement de la base d'apprentissage
    vector <vector<float> > inputs_learning;
    vector <vector<float> > inputs_test;
    vector <float> results_learning;
    loadLearningBase(INPUTS_FILE_LEARNING, inputs_learning, results_learning, CHAR_LEN, CHAR_WIDTH);
    loadTest(INPUTS_FILE_TEST, inputs_test, CHAR_LEN, CHAR_WIDTH);
    std::vector <float> final_layer_value (1);

    vector<int> randomIndexes(inputs_learning.size());

    //On crée autant de réseaux de neurones que de caractères à apprendre
    for (int i = 0; i < inputs_learning.size(); ++i)
    {
        Network* newRN = new Network(Layer(inputs_learning[0].size(), NUM_NEURON_INTER_LAYER),
                    Layer(NUM_NEURON_INTER_LAYER, 1));
        networks.push_back(newRN);    
    }


    for (int i = 0; i < inputs_learning.size(); ++i)
    {
        for (int j = 0; j < networks.size(); ++j)
        {

            final_layer_value[0] = (j == results_learning[i])? 1 : 0;
            //Autant de réseaux de neurones que de lettres à apprendre
            for(int k=0 ; k<inputs_learning.size() ; k++)
                randomIndexes[k] = k;

            // Phase d'apprentissage
            // Tant qu'on a au moins une erreur par cycle
            do
            {
                networks[j]->error = 0.f;
                //Permutation des données d'entrées
                random_shuffle(randomIndexes.begin(), randomIndexes.end());

                // Pour chaque entrée de la base de données
                for(int l : randomIndexes)
                {
                    //Propagation avant 1
                    networks[l]->interLayer.process(inputs_learning[l]);
                    //Propagation avant 2
                    networks[l]->finalLayer.process(networks[l]->interLayer.results());

                    //Ajustement des poids synaptiques
                    networks[l]->finalLayer.adjust(networks[l]->interLayer.results(), final_layer_value);
                    networks[l]->interLayer.adjust(inputs_learning[l], networks[l]->finalLayer);
          
                    //On fait un cumul des erreurs E(h)
                    networks[l]->error += networks[l]->finalLayer.error(final_layer_value);
                }

                //Erreur totale moyenne
                networks[j]->error /= inputs_learning.size();

                //Pour ne pas en afficher trop
                //if(networks[j]->idRound % 1000 == 0)
                  // cout << "Erreur : " << networks[j]->error << " "<<j <<endl;

                networks[j]->idRound++;
            }
            while(networks[j]->error > PRECISION);

           // cout <<"* Nombre d'itérations pour l'apprentissage : " <<idRound <<endl;
            //cout <<"* Entrées --> résultat du perceptron : " <<endl;
            
        }
    }


    for(vector<float> inputs : inputs_test)
    {
        show (inputs, CHAR_LEN, CHAR_WIDTH);
        float proba_result = 0.f, ind_result;
        for (int i = 0; i < networks.size(); ++i)
        {
            networks[i]->interLayer.process(inputs);
            networks[i]->finalLayer.process(networks[i]->interLayer.results());
            if (proba_result < networks[i]->finalLayer.results()[0])
            {
                ind_result = i;
                proba_result = networks[i]->finalLayer.results()[0];
            } 
            cout << networks[i]->finalLayer.results()[0]<<endl;
        }
        cout <<ind_result << " avec une probabilité de : " <<proba_result<<endl;
    }

    std::for_each( networks.begin(), networks.end(), DeleteFunctor<Network>() );
    // swap trick pour vider le vector et liberer la memoire
    vector<Network*>().swap(networks);

    return 0;
}

void show (const vector<float>& mat, int nbrows, int nbcolumns)
{

    for (int i = 0; i < mat.size(); ++i)
    {
        cout.width(0);
        if (i%nbcolumns == 0)
            cout <<endl;

        if ((int)mat[i] == 0) cout <<".";
        else cout <<"X";
    }
    cout <<endl;

}
 
void loadLearningBase (const std::string filePath, vector<vector<float>>& matInputs, vector<float>& matResults, int nbrows, int nbcolumns)
{
    std::vector<vector<float>> v_learningBase;
    std::ifstream file;
    std::string line;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open (filePath.c_str());

        while(file.good())
        {
            vector<float> img (nbrows*nbcolumns);
            std::string tmpInput;
            int tmpRes;
            file >>tmpInput >>tmpRes;           
            for (int i = 0 ; i < tmpInput.length(); i++)
                img[i] = tmpInput.at(i) - '0';
         
            matInputs.push_back(img);
            matResults.push_back(tmpRes);
        }
        file.close();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Exception opening/reading/closing file : " <<filePath <<endl;
    }
}

void loadTest (const std::string filePath, vector<vector<float>>& matInputs, int nbrows, int nbcolumns)
{
    std::vector<vector<float>> v_learningBase;
    std::ifstream file;
    std::string line;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open (filePath.c_str());

        while(file.good())
        {
            vector<float> img (nbrows*nbcolumns);
            std::string tmpInput;
            file >>tmpInput;           
            for (int i = 0 ; i < tmpInput.length(); i++)
                img[i] = tmpInput.at(i) - '0';
         
            matInputs.push_back(img);
        }
        file.close();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Exception opening/reading/closing file\n";
    }
}