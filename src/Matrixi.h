/*
*   Created by N.Richard
*   Date de création : 5 mai 2014
*   Date de version : 10 mai 2014
*   Version 0.4
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
    friend std::ostream& operator << (std::ostream& out, const Matrixi& mat);

    Matrixi transpose() const;
    Matrixi add(const Matrixi& mat) const;
    Matrixi mult(const Matrixi& mat) const;
    template<typename NumericType> Matrixi mult (const NumericType op) const;
    Matrixi pow (const int p) const;

    static Matrixi identity(const int rows, const int cols);
    void clear(int value = 0);

    //Getters & Setters
    int at(int i, int j) const;
    void set(const int i, const int j, int value);
    int getNbRows() const;
    int getNbCols() const;
    static int getCoutWidth();
    static void setCoutWidth(const int width);

    bool isSquare () const;

    //Matrix display
    void show (int coutwidth = COUTWIDTH) const;  
    //Destructor
    ~Matrixi();

private:
    int _rows, _cols;
    int* _values;

    static int COUTWIDTH;
};

template<typename NumericType> 
Matrixi operator* (const NumericType op, const Matrixi& mat)
{
    return mat.mult(op);
}

template<typename NumericType> 
Matrixi operator* (const Matrixi& mat, const NumericType op)
{
    return mat.mult(op);
}

template<typename NumericType> 
Matrixi& operator*= (const NumericType op, Matrixi& mat)
{
    return (mat = mat.mult(op));
}


template<typename NumericType> 
Matrixi& operator*= (Matrixi& mat,const NumericType op)
{
    return (mat = mat.mult(op));
}


template<typename NumericType> 
Matrixi Matrixi::mult (const NumericType op) const
{
    Matrixi mult (_rows, _cols, true);

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (int i = 0; i < _rows; ++i)
            for(int j = 0; j < _cols; ++j)
                mult(i,j) += this->at(i,j) * op;

    return mult;
}

#endif //MATRIXI_H
