#include "Utilities.h"

/*****************************************
	Gestion du temps (multiplateforme)
*****************************************/

#ifdef WIN32
int gettimeofday (struct timeval *tp, void *tz){
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

void initRandomEngine (){
	srand((unsigned int) time(NULL));
}

float getRand(float inter_a, float inter_b) {
	return (inter_a < inter_b)?
		((inter_b - inter_a) * ((float)rand() / (float)RAND_MAX)) + inter_a:
	((inter_a - inter_b) * ((float)rand() / (float)RAND_MAX)) + inter_b;

}


/*****************************
Transtypage
*****************************/

template<typename T>
bool from_string( const std::string & Str, T & Dest )
{
	std::istringstream iss(Str);
	return iss >> Dest != 0;
}

template<typename T>
bool to_string( const T & Orig, const std::string & Str)
{
	std::ostringstream oss(Str);
	return oss << Orig != 0;
}

template<typename T>
T& from_string( const std::string & Str)
{
	T Dest;
	std::istringstream iss(Str);
	iss >> Dest;
	return Dest;
}

template<typename T>
const std::string to_string( const T & Orig)
{
	std::ostringstream oss;
	oss << Orig;
	return oss.str();
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

bool isReadable (const std::string& filePath)
{
	std::ifstream file (filePath.c_str());
	return !file.fail();
}

