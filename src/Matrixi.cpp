#include <iostream>
#include <omp.h>
#include <stdexcept>
#include "Matrixi.h"

// - Ne pas mettre de unsigned partout:
//      - Dù à un bug de gcc sse n'est pas utilisé avec des unsigned, le code n'estdonc pas optimisé avec des unsigned
//      - C'est chiant a utilisé et cause des bugs
// - Ne pas mettre le mot clef inline dans les cpp car par définition inline implique que le contenu des fonctions se trouve dans le .h
// - Proposer une transposition inplace qui ne crée pas de nouvelle matrice pour aller plus vite
// - Mettre certaine fonction en inline dans le .h

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
                this->set(i, j, 0);
    }
}

Matrixi& Matrixi::operator= (const Matrixi& mat1)
{
    if(*this != mat1){
        _rows = mat1._rows;
        _cols = mat1._cols;
        for(unsigned int i=0 ; i < _rows; ++i)
            for(unsigned int j=0 ; j < _cols; ++j)
                this->set(i,j, mat1(i,j));
    }

    return *this;
}

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

bool Matrixi::operator!= (const Matrixi& mat)
{
    return !(*this == mat);    
}

int& Matrixi::operator() (unsigned int row, unsigned int col)
{
    #ifndef NDEBUG
    if (row >= _rows || col >= _cols
        || row < 0 || col < 0)
        throw out_of_range("const Matrix subscript out of bounds");
    #endif  //NDEBUG
    return _values[_cols*row + col];
}

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
void Matrixi::clear(int value)
{
    for(unsigned int i=0 ; i < _cols ; ++i)
        for(unsigned int j=0 ; j < _rows ; ++j)
            set(i,j,value);
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
// Note la matrice mult doit forcément être initialisée à zero.
Matrixi Matrixi::mult(const Matrixi& mat)
{
    Matrixi mult (_rows, _cols, true);

    if(_cols != mat._rows)
        throw new domain_error("Matrix type not compatible for multiplication"); 

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (unsigned int i = 0; i < _rows; ++i)
        for (unsigned int k = 0; k < _rows; ++k)
            for (unsigned int j = 0; j < _cols; ++j)
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

Matrixi Matrixi::identity(unsigned int rows, unsigned int cols)
{
    Matrixi identity (rows, cols); 

    for (unsigned int i = 0; i < rows; ++i)
        identity.set(i, i, 1);

    return identity;
}

Matrixi Matrixi::identity(const Matrixi& mat)
{
    Matrixi identity (mat._rows, mat._cols); 

    for (unsigned int i = 0; i < mat._rows; ++i)
        identity.set(i, i, 1);

    return identity;
}


//Getters & Setters
int Matrixi::at(const unsigned int i, const unsigned int j)
{
    return operator()(i,j);
}
    
void Matrixi::set(const unsigned int  i, const unsigned int j, int value)
{
    operator()(i,j) = value;
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