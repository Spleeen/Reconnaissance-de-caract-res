#include "Matrixi.h"
#include "Utilities.h"
#include <omp.h>
#include <stdexcept>
#include <string.h>

using namespace std;

int Matrixi::COUTWIDTH = 8;

//Matrix initialize with null values
Matrixi::Matrixi (int nbrows, int nbcolumns, bool clear): _rows(nbrows), _cols(nbcolumns), _values(NULL)
{
    if (nbrows <= 0 || nbcolumns <= 0)
        throw domain_error("Matrix can't have a zero size");
    _values = new int[nbrows * nbcolumns];

    if (clear)
    {   
        for(int i=0 ; i < _rows; ++i)
            for(int j=0 ; j < _cols; ++j)
                this->set(i, j, 0);
    }
}

Matrixi::Matrixi(const Matrixi& mat) : _rows(mat.getNbRows()), _cols(mat.getNbCols()), _values(NULL)
{ 

    if (mat._values != NULL) {
            _values = new int[_rows*_cols];
            if (!_values)
                throw new std::bad_alloc();
            memcpy(_values,mat._values, _rows*_cols * sizeof(int));
    }
} 

Matrixi& Matrixi::operator= (const Matrixi& mat)
{
    if (this != &mat){

        Matrixi tmp (mat);

        std::swap(this->_rows, tmp._rows); 
        std::swap(this->_cols, tmp._cols); 
        std::swap(this->_values, tmp._values); 
    }
    return *this;
}

bool Matrixi::operator== (const Matrixi& mat)
{
    if ((_rows != mat._rows)
        || (_cols != mat._cols))
        return false;
    
    for(int i=0; i < mat._rows; ++i)
        for(int j=0; j < mat._cols; ++j)
                if (this->at(i,j) != mat(i,j))
                    return false;

    return true;
}

bool Matrixi::operator!= (const Matrixi& mat)
{
    return !(*this == mat);    
}

int& Matrixi::operator() (int row, int col)
{
    #ifndef NDEBUG
    if (row >= _rows || col >= _cols
        || row < 0 || col < 0)
        throw out_of_range("const Matrix subscript out of bounds");
    #endif  //NDEBUG
    return _values[_cols*row + col];
}

int Matrixi::operator() (int row, int col) const
{
    #ifndef NDEBUG
    if (row >= _rows || col >= _cols
        || row < 0 || col < 0)
        throw out_of_range("const Matrix subscript out of bounds");
    #endif  //NDEBUG
    return _values[_cols*row + col];
}

Matrixi operator+ (const Matrixi& mat1, const Matrixi& mat2)
{
    return mat1.add(mat2);
}

Matrixi& operator+= (Matrixi& mat1, const Matrixi& mat2)
{
    return (mat1 = mat1.add(mat2));
}

Matrixi operator* (const Matrixi& mat1, const Matrixi& mat2)
{
    return mat1.mult(mat2);
}

Matrixi& operator*= (Matrixi& mat1, const Matrixi& mat2)
{
    return (mat1 = mat1.mult(mat2));
}

ostream& operator << (ostream& out, const Matrixi& mat)
{
    out <<"Matrix " <<mat.getNbRows() <<"x" <<mat.getNbCols() <<endl;

    for(int i=0 ; i <= mat.getNbCols()*Matrixi::COUTWIDTH+1; ++i)
            out <<"=";
    out <<endl;   

    for(int i=0 ; i < mat.getNbRows(); ++i)
    {
        out <<"|";
        out.width(Matrixi::COUTWIDTH);
        out <<mat(i, 0);
    
        for(int j=1 ; j < mat.getNbCols(); ++j)
        {
            out.width(Matrixi::COUTWIDTH);
            out << std::right <<mat(i,j);
        }
        out <<"|" <<endl;
    }

    return out;
}

//Matrix transpose
void Matrixi::clear (int value)
{
    for(int i=0 ; i < _cols ; ++i)
        for(int j=0 ; j < _rows ; ++j)
            set(i,j,value);
}

//Matrix transpose
Matrixi Matrixi::transpose () const
{
    Matrixi transposed (_cols, _rows, true);

    for(int i=0 ; i < _cols ; ++i)
        for(int j=0 ; j < _rows ; ++j){
            transposed.set(i,j,at(j,i));
        }
    return transposed;
}


//Matrix multiplication
// Note la matrice mult doit forcément être initialisée à zero.
Matrixi Matrixi::mult (const Matrixi& mat) const
{
    if(_cols != mat._rows || _rows != mat._cols)
        throw domain_error("Matrix type not compatible for multiplication"); 

    Matrixi mult (_rows, _cols, true);

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < _rows; ++i)
        for(int k = 0; k < _rows; ++k)
            for(int j = 0; j < _cols; ++j)
                mult(i,j) += this->at(i,k) * mat(k,j);

    return mult;
}

//Matrix addition
Matrixi Matrixi::add (const Matrixi& mat) const
{
    if(_cols != mat._cols || _rows != mat._rows)
        throw domain_error("Matrix type not compatible for addition. Both matrices must have the same number of rows and columns"); 

    Matrixi add (_rows, _cols); 

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _cols; ++j)
            add(i,j) = this->at(i,j) + mat(i,j);

    return add;
}

Matrixi Matrixi::pow (const int p) const
{
    if(!this->isSquare())
        throw domain_error("Matrix type not compatible for pow operation. Matrix must be square"); 

    if (p == 0)
        return Matrixi::identity (_rows, _cols);
    else if (p == 1)
        return *this;
    else if (p%2 == 0){
        Matrixi powmat(_rows, _cols, false);
        powmat = this->pow(p/2);
        return powmat*powmat;
    }
    else if (p%2 == 1){
        Matrixi powmat(_rows, _cols, false);
        powmat = this->pow (p/2);
        return powmat*powmat**this;
    }
    else 
        throw domain_error("For the moment, this library don't manage negative or float pow. As soon as possible !"); 

    return *this;
}

Matrixi Matrixi::identity (const int rows, const int cols)
{
    if(rows != cols)
        throw domain_error("Matrix type not compatible for identity. Matrix must be square"); 

    Matrixi identity (rows, cols); 

    for (int i = 0; i < rows; ++i)
        identity.set(i, i, 1);

    return identity;
}

//Getters & Setters
int Matrixi::at (const int i, const int j) const
{
    return operator()(i,j);
}
    
void Matrixi::set (const int  i, const int j, int value)
{
    operator()(i,j) = value;
}

int Matrixi::getNbRows () const
{
    return _rows;
}

int Matrixi::getNbCols () const
{
    return _cols;
}

int Matrixi::getCoutWidth()
{
    return Matrixi::COUTWIDTH;
}

void Matrixi::setCoutWidth(const int nbDigit)
{
    Matrixi::COUTWIDTH = nbDigit+1;
}

bool Matrixi::isSquare () const
{
    return (_cols == _rows);
}

//Matrix display
void Matrixi::show (int coutwidth) const
{
    cout <<"Matrix " <<_rows <<"x" <<_cols <<endl;

    for(int i=0 ; i <= _cols*coutwidth+1; ++i)
            cout <<"=";
    cout <<endl;   

    for(int i=0 ; i < _rows; ++i)
    {
        cout <<"|";
        cout.width(coutwidth);
        cout <<this->at(i, 0);
    
        for(int j=1 ; j < _cols; ++j)
        {
            cout.width(coutwidth);
            cout << std::right <<this->at(i,j);
        }
        cout <<"|" <<endl;
    }
}

//Destructor
Matrixi::~Matrixi ()
{
    delete[] _values;
}