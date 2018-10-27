#pragma once

#include <algorithm>

template<class TMatrix>
inline void MatrixRowsSwap(TMatrix& matrix, unsigned row_i, unsigned row_j, unsigned row_first_element = 0)
{
    if (row_i == row_j) return;
    typename TMatrix::iterator it_i = matrix.GetP(row_i, row_first_element), it_j = matrix.GetP(row_j, row_first_element), it_i_end = matrix.GetP(row_i + 1, 0);
    for (; it_i < it_i_end; ) std::swap(*it_i++, *it_j++);
}
