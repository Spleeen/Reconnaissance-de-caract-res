#include "Matrix.h"
#include <iostream>
using namespace std;

typedef bool dd;

int main()
{
    Matrix<float> m (10,10);

     m = Matrix<float>::identity(10,10);
     m.set(4,5, float(1.2));
     cout <<m <<endl<<m.isLowerTriangular();
/*
    mat.show();
    std::cout <<std::endl;
    
    mat.transpose().show();

    std::cout <<std::endl;

    Matrix copy (mat);
    copy.show();
    std::cout <<std::endl;

    mat_result.show();
    std::cout <<std::endl;
    
    Matrix::setCoutWidth(9);
    std::cout <<mat.transpose() <<endl;
    std::cout <<mat.transpose()+mat.transpose().mult((double)121145.52424424244246225252525662424242).mult(2).add(mat.transpose()) <<endl;
*/
    //cout <<mat <<Matrix::identity(4,4).pow(1243232323);
    
    return 0;
}
