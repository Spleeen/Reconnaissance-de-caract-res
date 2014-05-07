/*
*   Created by N.Richard
*   Date de création : 5 mai 2014
*   Date de version : 5 mai 2014
*   Version 0.1
*   Principe : Bibliothèque simple pour la gestion de matrices (forme coplien)
*/

#ifndef MATRIXI_H
#define MATRIXI_H

#include <iostream>

class Matrixi {

public:
    //Constructors
    Matrixi(int nbrows, int nbcolumns, bool clear = true);
    Matrixi(const Matrixi& mat);

    //Operators override
    Matrixi& operator= (const Matrixi &mat1); 
    bool operator== (const Matrixi& mat);
    bool operator!= (const Matrixi& mat);
    int& operator() (int nbrows, int nbcolumns);
    int operator() (int nbrows, int nbcolumns) const;
    Matrixi operator+ (const Matrixi& mat);
    Matrixi& operator+= (const Matrixi& mat);
    Matrixi operator* (const Matrixi& mat);
    template<typename NumericType> NumericType operator* (const NumericType op);
    Matrixi& operator*= (const Matrixi& mat);
    template<typename NumericType> NumericType& operator*= (const NumericType op);
    friend std::ostream& operator << (std::ostream& out, const Matrixi& mat);
    //Matrix clear
    void clear(int value = 0);

    //Matrix transpose
    Matrixi transpose();

    //Matrix addition
    Matrixi add(const Matrixi& mat);

    //Matrix multiplication
    Matrixi mult(const Matrixi& mat);
    template<typename NumericType> Matrixi mult (const NumericType op);

    static Matrixi identity(int rows, int cols);
    static Matrixi identity(const Matrixi& mat);

    //Getters & Setters
    int at(int i, int j);
    void set(const int i, const int j, int value);
    int getNbRows() const;
    int getNbCols() const;
    static int getCoutWidth();
    static void setCoutWidth(const int width);

    //Matrix display
    void show (int coutwidth = COUTWIDTH);  
    //Destructor
    ~Matrixi();

private:
    int _rows, _cols;
    int* _values;

    static int COUTWIDTH;

};

template<typename NumericType> 
NumericType  Matrixi::operator* (const NumericType op)
{
    return this->mult(op);
}

template<typename NumericType> 
NumericType& Matrixi::operator*= (const NumericType op)
{
    return (*this = this->mult(op));
}

template<typename NumericType> 
Matrixi Matrixi::mult (const NumericType op)
{
    Matrixi mult (_rows, _cols, true);

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < _rows; ++i)
            for(int j = 0; j < _cols; ++j)
                mult(i,j) += (int)(this->at(i,j) * op);

    return mult;
}

#endif //MATRIXI_H
