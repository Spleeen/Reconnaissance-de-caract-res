/*
*   Created by N.Richard
*   Date de création : 5 mai 2014
*   Date de version : 14 mai 2014
*   Version 1.0
*   Principe : Bibliothèque simple pour la gestion de matrices (forme coplien)
*/
 
#ifndef MATRIX_H
#define MATRIX_H

#include "Utilities.h"
#include <omp.h>
#include <iostream>
#include <stdexcept>
#include <string.h>

template <typename T>
class Matrix {

public:
    //Constructors
    Matrix (int nbrows, int nbcolumns): _rows(nbrows), _cols(nbcolumns), _values(NULL)
    {
        if (nbrows <= 0 || nbcolumns <= 0)
            throw std::domain_error("Matrix can't have a zero size");
        _values = new T[nbrows * nbcolumns]();
    }

    Matrix(const Matrix& mat) : _rows(mat.getNbRows()), _cols(mat.getNbCols()), _values(NULL)
    { 

        if (mat._values != NULL) {
                _values = new T[_rows*_cols]();
                memcpy(_values,mat._values, _rows*_cols * sizeof(T));
        }
    } 

    //Operators override
    Matrix& operator= (const Matrix &mat)
    {
        if (this != &mat){

            Matrix tmp (mat);

            std::swap(this->_rows, tmp._rows); 
            std::swap(this->_cols, tmp._cols); 
            std::swap(this->_values, tmp._values); 
        }
        return *this;
    }

    bool operator== (const Matrix& mat)
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

    bool operator!= (const Matrix& mat)
    {
        return !(*this == mat);    
    }

    T& operator() (int nbrows, int nbcolumns)
    {
        #ifndef NDEBUG
        if (nbrows >= _rows || nbcolumns >= _cols
            || nbrows < 0 || nbcolumns < 0)
            throw std::out_of_range("const Matrix subscript out of bounds");
        #endif  //NDEBUG
        return _values[_cols*nbrows + nbcolumns];
    }   

    T operator() (int nbrows, int nbcolumns) const
    {
        #ifndef NDEBUG
        if (nbrows >= _rows || nbcolumns >= _cols
            || nbrows < 0 || nbcolumns < 0)
            throw std::out_of_range("const Matrix subscript out of bounds");
        #endif  //NDEBUG
        return _values[_cols*nbrows + nbcolumns];
    }
    
    Matrix transpose() const
    {
        Matrix transposed (_cols, _rows);

        for(int i=0 ; i < _cols ; ++i)
            for(int j=0 ; j < _rows ; ++j)
                transposed.set(i,j,at(j,i));

        return transposed;
    }

    Matrix add(const Matrix& mat) const
    {
        if(_cols != mat._cols || _rows != mat._rows)
            throw std::domain_error("Matrix type not compatible for addition. Both matrices must have the same number of rows and columns"); 

        Matrix add (_rows, _cols); 

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                add(i,j) = this->at(i,j) + mat(i,j);

        return add;
    }

    Matrix mult(const Matrix& mat) const
    {
        if(_cols != mat._rows || _rows != mat._cols)
            throw std::domain_error("Matrix type not compatible for multiplication"); 

        Matrix mult (_rows, _cols);

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif
        for (int i = 0; i < _rows; ++i)
            for(int k = 0; k < _rows; ++k)
                for(int j = 0; j < _cols; ++j)
                    mult(i,j) += this->at(i,k) * mat(k,j);

        return mult;
    }

    template <typename NumericType> 
    Matrix mult (const NumericType op) const
    {
        Matrix mult (_rows, _cols);

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif
        for (int i = 0; i < _rows; ++i)
            for(int j = 0; j < _cols; ++j)
                mult(i,j) += this->at(i,j) * op;

        return mult;
    }

    Matrix pow (const int p) const
    {
        if(!this->isSquare())
            throw std::domain_error("Matrix type not compatible for pow operation. Matrix must be square"); 

        if (p == 0)
            return Matrix::identity (_rows, _cols);
        else if (p == 1)
            return *this;
        else if (p%2 == 0){
            Matrix powmat(_rows, _cols, false);
            powmat = this->pow(p/2);
            return powmat*powmat;
        }
        else if (p%2 == 1){
            Matrix powmat(_rows, _cols, false);
            powmat = this->pow (p/2);
            return powmat*powmat**this;
        }
        else 
            throw std::domain_error("For the moment, this library don't manage negative or float pow. As soon as possible !"); 

        return *this;
    }

    static Matrix identity(const int nbrows, const int nbcolumns)
    {
        if(nbrows != nbcolumns)
            throw std::domain_error("Matrix type not compatible for identity. Matrix must be square"); 

        Matrix identity (nbrows, nbcolumns); 

        for (int i = 0; i < nbrows; ++i)
            identity.set(i, i, static_cast<T>(1));

        return identity;
    }

    void clear(T value = 0)
    {
        for(int i=0 ; i < _cols ; ++i)
            for(int j=0 ; j < _rows ; ++j)
                set(i,j,value);
    }

    //Getters & Setters
    T at(int i, int j) const
    {
        return operator()(i,j);
    }

    void set(const int i, const int j, T value)
    {
        operator()(i,j) = value;
    }
    
    int getNbRows() const
    {
        return _rows;
    }

    int getNbCols() const
    {
        return _cols;
    }
    
    static int getCoutWidth()
    {
        return Matrix::COUTWIDTH;
    }
    
    static void setCoutWidth(const int width)
    {
        Matrix::COUTWIDTH = width+1;
    }

    bool isSquare () const
    {
        return (_cols == _rows);
    }

    bool isIdentity() const
    {
        if (!isDiagonal())
            return false;

        for(int i=0; i < _rows; ++i)
            if(!equal(at(i,i),1))
                return false;

        return true;
    }

    bool isDiagonal() const
    {
        if (!isSquare())
            return false;

        for(int i=0; i < _rows; ++i)
            for(int j=0; j < _cols; ++j)
                if (i!=j && !equal(at(i,j),0))
                    return false;

        return true;
    }

    bool isNull() const
    {
        for(int i=0; i < _rows; ++i)
            for(int j=0; j < _cols; ++j)
                if (!equal(at(i,j), 0))
                    return false;

        return true;
    }

    //Matrix display
    void show (int coutwidth = COUTWIDTH) const
    {
        std::cout <<"Matrix " <<_rows <<"x" <<_cols <<std::endl;

        for(int i=0 ; i <= _cols*coutwidth+1; ++i)
            std::cout <<"=";
        std::cout <<std::endl;   

        for(int i=0 ; i < _rows; ++i)
        {
            std::cout <<"|";
            std::cout.width(coutwidth);
            std::cout <<this->at(i, 0);
        
            for(int j=1 ; j < _cols; ++j)
            {
                std::cout.width(coutwidth);
                std::cout << std::right <<this->at(i,j);
            }
            std::cout <<"|" <<std::endl;
        }
    }

    //Destructor
    ~Matrix()
    {
        delete[] _values;
    }

    static int COUTWIDTH;

private:
    int _rows, _cols;
    T* _values;

};

template<typename T>
int Matrix<T>::COUTWIDTH = 8;

template<typename T, typename NumericType> 
Matrix<T> operator* (const NumericType op, const Matrix<T>& mat)
{
    return mat.mult(op);
}

template<typename T, typename NumericType> 
Matrix<T> operator* (const Matrix<T>& mat, const NumericType op)
{
    return mat.mult(op);
}

template<typename T>
Matrix<T> operator* (const Matrix<T>& mat1, const Matrix<T>& mat2)
{
    return mat1.mult(mat2);
}

template<typename T, typename NumericType> 
Matrix<T>& operator*= (const NumericType op, Matrix<T>& mat)
{
    return (mat = mat.mult(op));
}

template<typename T, typename NumericType> 
Matrix<T>& operator*= (Matrix<T>& mat,const NumericType op)
{
    return (mat = mat.mult(op));
}

template<typename T>
Matrix<T>& operator*= (Matrix<T>& mat1, const Matrix<T>& mat2)
{
    return (mat1 = mat1.mult(mat2));
}

template<typename T>
Matrix<T> operator+ (const Matrix<T>& mat1, const Matrix<T>& mat2)
{
    return mat1.add(mat2);
}

template<typename T>
Matrix<T>& operator+= (Matrix<T>& mat1, const Matrix<T>& mat2)
{
    return (mat1 = mat1.add(mat2));
}

template <typename T>
std::ostream& operator << (std::ostream& out, const Matrix<T>& mat)
{
    out <<"Matrix " <<mat.getNbRows() <<"x" <<mat.getNbCols() <<std::endl;

    for(int i=0 ; i <= mat.getNbCols()*Matrix<T>::COUTWIDTH+1; ++i)
            out <<"=";
    out <<std::endl;   

    for(int i=0 ; i < mat.getNbRows(); ++i)
    {
        out <<"|";
        out.width(Matrix<T>::COUTWIDTH);
        out <<mat(i, 0);
    
        for(int j=1 ; j < mat.getNbCols(); ++j)
        {
            out.width(Matrix<T>::COUTWIDTH);
            out << std::right <<mat(i,j);
        }
        out <<"|" <<std::endl;
    }

    return out;
}

#endif //MATRIX_H
