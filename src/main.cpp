#include "Matrixi.h"
#include <iostream>
using namespace std;

int main()
{
    Matrixi mat (8, 8);
    Matrixi mat_result (8, 8);

    for(unsigned int i=0 ; i<mat.getNbRows() ; i++)
        for(unsigned int j=0 ; j<mat.getNbCols(); j++)
            mat.set(i, j, i * mat.getNbCols() + j);

    mat.show();
    std::cout <<std::endl;
    mat_result = mat.transpose();
    mat_result.show();
    std::cout <<std::endl;

    mat_result = mat;
    mat_result.show();
    std::cout <<std::endl;

    mat_result = mat.mult(mat);
    mat_result.show();
    std::cout <<std::endl;

    mat_result = mat.add(mat);
    mat_result.show();
    std::cout <<std::endl;

    mat_result = Matrixi::identity(mat);
    mat_result.show();
    std::cout <<std::endl;
    return 0;
}
