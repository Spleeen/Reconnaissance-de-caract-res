/*
*   Created by N.Richard
*   Date de création : 2 juin 2014
*   Date de version : 2 juin 2014
*   Version : 0.3
*/

#ifndef PATTERN_H
#define PATTERN_H

#include "Matrix.h"
#include <algorithm>


class Pattern : public Matrix<int> {

    public :
    Pattern(int nbrows, int nbcolumns): Matrix<int>(nbrows,nbcolumns){};
    Pattern(const Pattern& pat): Matrix<int>(pat){};
    Pattern(const int* tab, int nbrows, int nbcolumns): Matrix<int>(tab, nbrows, nbcolumns){};
    Pattern(const std::string filePath, int nbLine, int nbrows, int nbcolumns): Matrix<int>(nbrows, nbcolumns)
    {
        std::ifstream file;
        std::string line;
        file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
        try {
            file.open (filePath.c_str());
            for (int i = 0; getline(file, line); ++i)
            {
                if (i == nbLine){
                    for (int i = 0 ; i < line.length(); i++)
                    {
                        m_values[i] = line.at(i) - '0';
                    }
                    break;
                }
            }
            file.close();
        }
        catch (std::ifstream::failure e) {
            std::cerr << "Exception opening/reading/closing file\n";
        }
    };

    static int COUTWIDTH;
};

int Pattern::COUTWIDTH = 0;

std::ostream& operator << (std::ostream& out, const Pattern& pat)
{
    for(int i=0 ; i < pat.nbRows(); ++i)
    {
        out.width(Pattern::COUTWIDTH);
        if (pat(i, 0) == 0)
            out <<".";
        else out <<"X";
    
        for(int j=1 ; j < pat.nbCols(); ++j)
        {
            out.width(Pattern::COUTWIDTH);
            if (pat(i, j) == 0)
                out <<".";
            else out <<"X";
        }
        out <<std::endl;
    }

    return out;
}

#endif //PATTERN_H
