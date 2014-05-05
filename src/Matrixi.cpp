#include <iostream>
#include <omp.h>
#include <stdexcept>
#include "Matrixi.h"

using namespace std;

//Matrix initialize with null values
Matrixi::Matrixi(unsigned int nbrows, unsigned int nbcolumns, bool clear): _rows(nbrows), _cols(nbcolumns)
{
    if (nbrows <= 0 || nbcolumns <= 0)
        throw new domain_error("Matrix can't have a zero size");
    _values = new int[nbrows * nbcolumns];

    if (clear)
    {   
        for(unsigned int i=0 ; i < _rows; ++i)
            for(unsigned int j=0 ; j < _cols; ++j)
                this->setValue(i, j, 0);
    }
}

Matrixi& Matrixi::operator= (const Matrixi& mat1)
{
    if(*this != mat1){
        _rows = mat1._rows;
        _cols = mat1._cols;
        for(unsigned int i=0 ; i < _rows; ++i)
            for(unsigned int j=0 ; j < _cols; ++j)
                this->setValue(i,j, mat1(i,j));
    }

    return *this;
}

inline
bool Matrixi::operator== (const Matrixi& mat)
{
    if ((_rows != mat._rows)
        || (_cols != mat._cols))
        return false;
    
    for(unsigned int i=0 ; i < _rows; ++i)
        for(unsigned int j=0 ; j < _cols; ++j)
            if (this->at(i,j) != mat(i,j))
                return false;

    return true;    
}

inline
bool Matrixi::operator!= (const Matrixi& mat)
{
    return !(*this == mat);    
}

inline
int& Matrixi::operator() (unsigned int row, unsigned int col)
{
    #ifndef NDEBUG
    if (row >= _rows || col >= _cols
        || row < 0 || col < 0)
        throw out_of_range("const Matrix subscript out of bounds");
    #endif  //NDEBUG
    return _values[_cols*row + col];
}

inline
int Matrixi::operator() (unsigned int row, unsigned int col) const
{
    #ifndef NDEBUG
    if (row >= _rows || col >= _cols
        || row < 0 || col < 0)
        throw out_of_range("const Matrix subscript out of bounds");
    #endif  //NDEBUG
    return _values[_cols*row + col];
}

//Matrix transpose
Matrixi Matrixi::transpose()
{
    Matrixi transposed (_cols, _rows);

    for(unsigned int i=0 ; i < _cols ; ++i)
        for(unsigned int j=0 ; j < _rows ; ++j)
            transposed(i,j) = at(j,i);

    return transposed;
}

//Matrix multiplication
Matrixi Matrixi::mult(const Matrixi& mat)
{
    Matrixi mult (_rows, _cols);

    if(_cols != mat._rows)
        throw new domain_error("Matrix type not compatible for multiplication"); 

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (unsigned int i = 0; i < _rows; ++i)
        for (unsigned int j = 0; j < _cols; ++j)
            for (unsigned int k = 0; k < _rows; ++k)
                mult(i,j) += this->at(i,j) * mat(k,j);

    return mult;
}

//Matrix addition
Matrixi Matrixi::add(const Matrixi& mat)
{
    Matrixi add (_rows, _cols); 

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (unsigned int i = 0; i < _rows; ++i)
        for (unsigned int j = 0; j < _cols; ++j)
            add(i,j) = this->at(i,j) + mat(i,j);

    return add;
}

Matrixi Matrixi::toIdMatrix()
{
    Matrixi identity (_rows, _cols); 

    for (unsigned int i = 0; i < _rows; ++i){
        identity.setValue(i, i, 1);
    }

    return identity;
}


//Getters & Setters
int Matrixi::getValue(const unsigned int i, const unsigned int j)
{
    return this->at(i,j);
}
    
void Matrixi::setValue(const unsigned int  i, const unsigned int j, int value)
{
    this->setValue(i, j, value);
}

int Matrixi::at(unsigned int i, unsigned int j)
{
    return operator()(i,j);
}

unsigned int Matrixi::getNbRows()
{
    return _rows;
}

unsigned int Matrixi::getNbCols()
{
    return _cols;
}

//Matrix display
void Matrixi::show(int coutwidth)
{
    for(unsigned int i=0 ; i < _rows; ++i)
    {
        cout <<"[";
        cout.width(coutwidth);
        cout <<(*this)(i, 0);
    
        for(unsigned int j=1 ; j < _cols; ++j)
        {
            cout.width(coutwidth);
            cout << std::right <<this->at(i,j);
        }
        cout <<"]" <<endl;
    }
}

//Destructor
Matrixi::~Matrixi()
{
    delete[] _values;
}