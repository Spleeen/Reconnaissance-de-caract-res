#include "Matrixi.h"
#include <iostream>
using namespace std;

int main()
{
    Matrixi mat (10, 10);
    Matrixi mat_result (8, 12);

    for(unsigned int i=0 ; i<mat.getNbRows() ; i++)
        for(unsigned int j=0 ; j<mat.getNbCols(); j++)
            mat.set(i, j, i*mat.getNbCols()+j);

    mat.show();
    std::cout <<std::endl;
    
    mat.transpose().show();

    std::cout <<std::endl;

    Matrixi copy (mat);
    copy.show();
    std::cout <<std::endl;

    mat_result = Matrixi::identity(10,10);
    mat_result.show();
    std::cout <<std::endl;

    mat_result.mult(mat).show();
    std::cout <<std::endl;
    return 0;
}
