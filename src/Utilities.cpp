#include "Utilities.h"

using namespace std; 


/*****************************************
	Gestion du temps (multiplateforme)
*****************************************/

#ifdef WIN32
int Utilities::gettimeofday (struct timeval *tp, void *tz){
	struct _timeb timebuffer;
	_ftime (&timebuffer);
	tp->tv_sec = timebuffer.time;
	tp->tv_usec = timebuffer.millitm * 1000;
	return 0;
}
#endif

/****************************************
Moteur de nombres aléatoires
****************************************/

void Utilities::initRandomEngine (){
	srand(static_cast<unsigned int> (time(NULL)));
}

float Utilities::getRand(float inter_a, float inter_b) {
	return (inter_a < inter_b)?
		((inter_b - inter_a) * (static_cast<float> (rand()) / static_cast<float> (RAND_MAX))) + inter_a:
	((inter_a - inter_b) * (static_cast<float> (rand()) / static_cast<float> (RAND_MAX))) + inter_b;

}

/***********************************
Ajout de la tabulation en C++
***********************************/

namespace std {
	const char* tab = "\t";
}

/***********************************
Manipulation de fichiers
***********************************/

bool Utilities::isReadable (const string& filePath)
{
	ifstream file (filePath.c_str());
	return !file.fail();
}

