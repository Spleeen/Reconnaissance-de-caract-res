#include "Matrixi.h"
#include <iostream>
using namespace std;

int main()
{
    Matrixi m (24,24);

    for (int i = 0; i < m.getNbCols(); ++i)
         for (int j = 0; j < m.getNbRows(); ++j)
            m(i,j) = i*m.getNbCols()+j;
    cout <<Matrixi::identity(4,4).pow(2)<<endl;
/*
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
*/
    //cout <<mat <<Matrixi::identity(4,4).pow(1243232323);
    
    return 0;
}
