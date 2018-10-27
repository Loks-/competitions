#pragma once

#include <iostream>

template<class TMatrix>
inline void WriteMatrix(const TMatrix& m)
{
    for (unsigned i = 0; i < m.Rows(); ++i)
    {
        for (unsigned j = 0; j < m.Columns(); ++j)
            std::cout << m(i, j) << " ";
        std::cout << std::endl;
    }
}
