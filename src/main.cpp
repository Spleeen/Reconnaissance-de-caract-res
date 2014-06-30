#include "Utilities.h"
#include "Layer.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>
#ifdef _OPENMP
    #include <omp.h>
#endif //_OPENMP

using namespace std;
using namespace Utilities;


template <class T>
struct DeleteFunctor : public std::unary_function<T*,void> { void operator() ( T* ptr) const { delete ptr;} };

void loadLearningBase (const std::string filePath, vector<vector<float>>& matInputs, vector<float>& matResults, int nbrows, int nbcolumns);
void loadTest (const std::string filePath, vector<vector<float>>& matInputs, int nbrows, int nbcolumns);
void show (const vector<float>& mat, int nbrows, int nbcolumns);
void increase_noise (vector<vector<float>>& data, float sigma);


//Paramètres par défaut
float PRECISION = 1e-5;      // Précision pour la convergeance
float NOISE = 0.15f;      //Pourcentage de bruit
#define INPUTS_FILE_LEARNING ("./ressources/inputs_learning.txt")
#define INPUTS_FILE_TEST ("./ressources/inputs_test.txt")
#define CHAR_WIDTH 5
#define CHAR_LEN 9
int NUM_NEURON_INTER_LAYER = 5;


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
        cout <<"* Utilisation d'un pas d'apprentissage de " <<Layer::STEP << ", d'une précision de "<<PRECISION << " et d'un bruit de " <<NOISE <<" (--help)" <<endl;
    }
    else if (argc != 4 || !from_string(argv[1],Layer::STEP) || !from_string(argv[2], PRECISION) || !from_string(argv[3], NOISE))
    {
        cout <<"Usage: neural-network PAS PRECISION NOISE" <<endl;
        cout <<"Veuillez indiquer le pas d'apprentissage (positive, souvent très proche de zéro), la précision (souvent très petite) et le pourcentage de bruit (entre 0 et 1)" <<endl;
        return EXIT_FAILURE;
    }
    
    //std::vector<float> proba_result (8);

    //for (int p = 0; p < 8; p++)
    {

        vector<Network*> networks;
        //Chargement de la base d'apprentissage
        vector <vector<float> > inputs_learning;
        vector <float> results_learning;
        loadLearningBase(INPUTS_FILE_LEARNING, inputs_learning, results_learning, CHAR_LEN, CHAR_WIDTH);
        //loadTest(INPUTS_FILE_TEST, inputs_test, CHAR_LEN, CHAR_WIDTH);
        std::vector <float> final_layer_value (1);

        vector<int> randomIndexes(inputs_learning.size());

        vector <vector<float> > inputs_test(inputs_learning);
        increase_noise(inputs_test, NOISE);

        //On crée autant de réseaux de neurones que de caractères à apprendre
        for (int i = 0; i < inputs_learning.size(); ++i)
        {
            Network* newRN = new Network(Layer(inputs_learning[0].size(), NUM_NEURON_INTER_LAYER),
                        Layer(NUM_NEURON_INTER_LAYER, 1));
            networks.push_back(newRN);    
        }

       
        for(int k=0 ; k<10 ; k++)
            for (int i = 0; i < inputs_learning.size(); ++i)
            {
                for (int j = 0; j < networks.size(); ++j)

                {
                    final_layer_value[0] = (j == results_learning[i])? 1 : 0;
                    //Autant de réseaux de neurones que de lettres à apprendre
                    //for(int k=0 ; k<inputs_learning.size() ; k++)
                        //randomIndexes[k] = k;

                    //cout << j << ' ' << final_layer_value[0] << endl;

                    // Phase d'apprentissage
                    // Tant qu'on a au moins une erreur par cycle
                    do
                    {
                        //Permutation des données d'entrées
                        //random_shuffle(randomIndexes.begin(), randomIndexes.end());

                        // Pour chaque entrée de la base de données
                        //for(int l : randomIndexes)
                        //{
                            //Propagation avant 1
                            networks[j]->interLayer.process(inputs_learning[i]);
                            //Propagation avant 2
                            networks[j]->finalLayer.process(networks[j]->interLayer.results());

                            //Ajustement des poids synaptiques
                            networks[j]->finalLayer.adjust(networks[j]->interLayer.results(), final_layer_value);
                            networks[j]->interLayer.adjust(inputs_learning[i], networks[j]->finalLayer);
                  
                            //On fait un cumul des erreurs E(h)
                            networks[j]->error = networks[j]->finalLayer.error(final_layer_value);
                        //}

                        //Erreur totale moyenne
                        //networks[j]->error /= inputs_learning.size();

                        networks[j]->idRound++;
                    }
                    while(networks[j]->error > PRECISION);

                   // cout <<"* Nombre d'itérations pour l'apprentissage : " <<idRound <<endl;
                    //cout <<"* Entrées --> résultat du perceptron : " <<endl;
                    
                }
            }

            float proba = 0.0;
            int kl = 0;
            int ind = 0;
        for(vector<float> inputs : inputs_test)
        {

            show (inputs, CHAR_LEN, CHAR_WIDTH);
            float current_proba = 0.0;
            for (int i = 0; i < networks.size(); ++i)
            {
                networks[i]->interLayer.process(inputs);
                networks[i]->finalLayer.process(networks[i]->interLayer.results());

                if (current_proba < networks[i]->finalLayer.results()[0] )
                {
                    current_proba = networks[i]->finalLayer.results()[0];
                    //cout <<i<<"\t" <<current_proba<<endl;
                    ind = i;
                } 
            }

            if (ind == kl)
            {
                
                if(current_proba < 0.5)
                {
                    cout <<"\033[0;33mIl hésite avec " << ind << " avec une probabilité de " << (int)(current_proba*100)<<"%\e[0m" <<endl; 
                } 
                else
                {               //proba += current_proba; 
                    cout <<"\033[0;92mTrouvé ! C'est " << ind << " avec une probabilité de " <<(int)(current_proba*100)<<"%\e[0m"<<endl; 
                }
            }  
            else 
                cout <<"\033[0;91mIl n'a pas réussi à déterminer le chiffre\e[0m" <<endl;
            kl++;
        }
        //proba_result[p] = proba / inputs_test.size();

        std::for_each( networks.begin(), networks.end(), DeleteFunctor<Network>() );
        // swap trick pour vider le vector et liberer la memoire
        vector<Network*>().swap(networks);
    }

   /* for (int m = 0; m < proba_result.size(); ++m)
    {
        cout <<proba_result[m]*100<<" "<<m <<endl;
    }*/
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

void increase_noise (vector<vector<float>>& data, float sigma)
{
    std::default_random_engine generator ((unsigned int)time(0));
    uniform_real_distribution<float> distribution(0.0,1.0);
     
    for (vector<float>& d : data)
    {
        for (float& e : d)
        {
            float random = distribution(generator);
            if (random  < sigma){
                e = (e == 1)? 0 : 1;
            }
        }    
    }
}