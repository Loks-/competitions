#pragma once

template<class TMatrix>
inline void MatrixRowsSubM(TMatrix& matrix, unsigned row_i, unsigned row_j, typename TMatrix::TValue m, unsigned row_first_element = 0)
{
    typename TMatrix::iterator it_i = matrix.GetP(row_i, row_first_element), it_j = matrix.GetP(row_j, row_first_element), it_i_end = matrix.GetP(row_i + 1, 0);
    for (; it_i < it_i_end; ) *it_i++ -= m * *it_j++;
}
