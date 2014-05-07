#include "Matrixi.h"
#include <iostream>
using namespace std;

int main()
{
    Matrixi mat (19, 56);
    Matrixi mat_result (8, 12);

    for(int i=0 ; i<mat.getNbRows() ; i++)
        for(int j=0 ; j<mat.getNbCols(); j++)
            mat.set(i, j, i*mat.getNbCols()+j);

    mat.show();
    std::cout <<std::endl;
    
    mat.transpose().show();

    std::cout <<std::endl;

    Matrixi copy (mat);
    copy.show();
    std::cout <<std::endl;

    mat_result.show();
    std::cout <<std::endl;

    Matrixi::setCoutWidth(9);
    std::cout <<mat.transpose() <<endl;
    std::cout <<mat.transpose()+mat.transpose().mult((double)121145.52424424244246225252525662424242).mult(2).add(mat.transpose()) <<endl;

    return 0;
}
