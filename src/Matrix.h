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
#ifdef _OPENMP
    #include <omp.h>
#endif //_OPENMP
#include <iostream>
#include <stdexcept>
#include <string.h>
 
template <typename T>
class Matrix {

public:
    //Constructors
    Matrix (int nbrows, int nbcolumns): m_rows(nbrows), m_cols(nbcolumns), m_values(NULL)
    {
        if (nbrows <= 0 || nbcolumns <= 0)
            throw std::domain_error("Matrix can't have a zero size");
        m_values = new T[nbrows * nbcolumns]();
    }

    Matrix(const Matrix& mat) : m_rows(mat.nbRows()), m_cols(mat.nbCols()), m_values(NULL)
    { 

        if (mat.m_values != NULL) {
                m_values = new T[m_rows*m_cols]();
                memcpy(m_values,mat.m_values, m_rows*m_cols * sizeof(T));
        }
    } 

    //Operators override
    Matrix& operator= (const Matrix &mat)
    {
        #ifndef NDEBUG
        if (this != &mat){
        #endif  //NDEBUG
            Matrix tmp (mat);

            m_rows = tmp.m_rows; 
            m_cols = tmp.m_cols; 
            std::swap(m_values, tmp.m_values); 
        #ifndef NDEBUG
        }
        #endif  //NDEBUG
        
        return *this;
    }

    bool operator== (const Matrix& mat)
    {
        if ((m_rows != mat.m_rows)
            || (m_cols != mat.m_cols))
            return false;
        
        if (memcmp (m_values, mat.m_values, sizeof(*this)) != 0)
            return false;

        return true;
    }

    bool operator!= (const Matrix& mat) const
    {
        return !(*this == mat);    
    }

    bool operator!() const 
    {
        return isNull();
    }
    
    Matrix operator~() const 
    {
        return transpose();
    }

    T& operator() (int row, int col)
    {
        #ifndef NDEBUG
        if (row >= m_rows || col >= m_cols
            || row < 0 || col < 0)
            throw std::out_of_range("const Matrix subscript out of bounds");
        #endif  //NDEBUG
        return m_values[m_cols*row + col];
    }   

    T operator() (int row, int col) const
    {
        #ifndef NDEBUG
        if (row >= m_rows || col >= m_cols
            || row < 0 || col < 0)
            throw std::out_of_range("const Matrix subscript out of bounds");
        #endif  //NDEBUG
        return m_values[m_cols*row + col];
    }
    
    Matrix transpose() const
    {
        Matrix transposeMat (m_cols, m_rows);

        for(int i=0 ; i < m_cols ; ++i)
            for(int j=0 ; j < m_rows ; ++j)
                transposeMat.set(i,j,at(j,i));

        return transposeMat;
    }

    Matrix add(const Matrix& mat) const
    {
        if(m_cols != mat.m_cols || m_rows != mat.m_rows)
            throw std::domain_error("Matrix type not compatible for addition. Both matrices must have the same number of rows and columns"); 

        Matrix addMat (m_rows, m_cols); 

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif //_OPENMP
        for (int i = 0; i < m_rows; ++i)
            for (int j = 0; j < m_cols; ++j)
                addMat(i,j) = at(i,j) + mat(i,j);

        return addMat;
    }

    Matrix mult(const Matrix& mat) const
    {
        if(m_cols != mat.m_rows || m_rows != mat.m_cols)
            throw std::domain_error("Matrix type not compatible for multiplication"); 

        Matrix multMat (m_rows, m_cols);

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif //_OPENMP
        for (int i = 0; i < m_rows; ++i)
            for(int k = 0; k < m_rows; ++k)
                for(int j = 0; j < m_cols; ++j)
                    multMat(i,j) += at(i,k) * mat(k,j);

        return multMat;
    }   
 
    template <typename NumericType> 
    Matrix mult (const NumericType scale) const
    {
        Matrix multMat (m_rows, m_cols);

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif //_OPENMP
        for (int i = 0; i < m_rows; ++i)
            for(int j = 0; j < m_cols; ++j)
                multMat(i,j) += at(i,j) * scale;

        return multMat;  
    }
   
    Matrix pow (const int p) const
    {  
        if(!isSquare())
            throw std::domain_error("Matrix type not compatible for pow operation. Matrix must be square"); 
        if (p == 0)
            return Matrix::identity (m_rows);
        else if (p == 1)
            return *this; 
        else if (p%2 == 0){
            Matrix powmat(m_rows, m_cols);
            powmat = pow(p/2);
            return powmat*powmat;
        }
        else if (p%2 == 1){
            Matrix powmat(m_rows, m_cols);
            powmat = pow (p/2);
            return powmat*powmat**this;
        }    
        else 
            throw std::domain_error("For the moment, this library don't manage negative or float pow. As soon as possible !"); 

        return *this;
    } 

    static Matrix identity(const int size)
    {
        Matrix identityMat (size, size); 

        for (int i = 0; i < size; ++i)
            identityMat.set(i, i, static_cast<T>(1));

        return identityMat;
    }

    void clear(T value = 0)
    {
        for(int i=0 ; i < m_cols ; ++i)
            for(int j=0 ; j < m_rows ; ++j)
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
    
    int nbRows() const
    {
        return m_rows;
    }

    int nbCols() const
    {
        return m_cols;
    }

    int nbElements() const
    {
        return m_cols*m_rows;
    }
    
    static int coutWidth()
    {
        return Matrix::COUTWIDTH;
    }
    
    static void setCoutWidth(const int width)
    {
        Matrix::COUTWIDTH = width+1;
    }

    bool isSquare () const
    {
        return (m_cols == m_rows);
    }

    bool isIdentity() const
    {
        if (!isDiagonal())
            return false;

        for(int i=0; i < m_rows; ++i)
            if(!equal(at(i,i),1))
                return false;

        return true;
    }

    bool isDiagonal() const
    {  
        if (!isSquare())
            throw std::domain_error("Matrix type not compatible for pow operation. Matrix must be square"); 

        for(int i=0; i < m_rows; ++i)
            for(int j=0; j < m_cols; ++j)
                if (i!=j && !equal(at(i,j),0))
                    return false;

        return true;
    }

    bool isNull() const
    {
        for(int i=0; i < m_rows; ++i)
            for(int j=0; j < m_cols; ++j)
                if (!equal(at(i,j), 0))
                    return false;

        return true;
    }

    bool isUpperTriangular() const
    {
        if (!isSquare())
            return false;

        for(int i = 1; i < m_rows; ++i)
            for (int j = 0; i != j && j < m_cols ; ++j)
                if (!equal(at(i,j), 0))
                    return false;

        return true;
    }

    bool isLowerTriangular() const
    {
        if (!isSquare())
            return false;

        for(int i = 0; i < m_rows; ++i)
            for (int j = i+1; j < m_cols ; ++j)
                if (!equal(at(i,j), 0))
                    return false;
                
        return true;
    }

    //Matrix display
    void show () const
    {
        std::cout <<*this;    
    }

    //Destructor
    ~Matrix()
    {
        delete[] m_values;
    }

    static int COUTWIDTH;

private:
    int m_rows, m_cols;
    T* m_values;

};

template<typename T>
int Matrix<T>::COUTWIDTH = 8;

template<typename T, typename NumericType> 
Matrix<T> operator* (const NumericType scale, const Matrix<T>& mat)
{
    return mat.mult(scale);
}

template<typename T, typename NumericType> 
Matrix<T> operator* (const Matrix<T>& mat, const NumericType scale)
{
    return mat.mult(scale);
}

template<typename T, typename U>
Matrix<T> operator* (const Matrix<T>& mat1, const Matrix<U>& mat2)
{
    return mat1.mult(mat2);
}

template<typename T, typename NumericType> 
Matrix<T>& operator*= (const NumericType scale, Matrix<T>& mat)
{
    return (mat = mat.mult(scale));
}

template<typename T, typename NumericType> 
Matrix<T>& operator*= (Matrix<T>& mat,const NumericType scale)
{
    return (mat = mat.mult(scale));
}

template<typename T, typename U>
Matrix<T>& operator*= (Matrix<T>& mat1, const Matrix<U>& mat2)
{
    return (mat1 = mat1.mult(mat2));
}

template<typename T, typename U>
Matrix<T> operator+ (const Matrix<T>& mat1, const Matrix<U>& mat2)
{
    return mat1.add(mat2);
}

template<typename T, typename U>
Matrix<T>& operator+= (Matrix<T>& mat1, const Matrix<U>& mat2)
{
    return (mat1 = mat1.add(mat2));
}

template <typename T>
std::ostream& operator << (std::ostream& out, const Matrix<T>& mat)
{
    out <<"Matrix " <<mat.nbRows() <<"x" <<mat.nbCols() <<std::endl;

    for(int i=0 ; i <= mat.nbCols()*Matrix<T>::COUTWIDTH+1; ++i)
            out <<"=";
    out <<std::endl;   

    for(int i=0 ; i < mat.nbRows(); ++i)
    {
        out <<"|";
        out.width(Matrix<T>::COUTWIDTH);
        out <<mat(i, 0);
    
        for(int j=1 ; j < mat.nbCols(); ++j)
        {
            out.width(Matrix<T>::COUTWIDTH);
            out << std::right <<mat(i,j);
        }
        out <<"|" <<std::endl;
    }

    return out;
}

#endif //MATRIX_H
