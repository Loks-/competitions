#pragma once

#include "../../matrix.h"
#include "../../rows/sub_m.h"
#include "../../rows/swap.h"

template<class TValue>
inline unsigned FMatrixRank(const Matrix<TValue>& matrix)
{
    unsigned r = 0;
    Matrix<TValue> m(matrix);
    for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j)
    {
        for (unsigned i = r; i < m.Rows(); ++i)
        {
            if (m(i, j) != 0)
            {
                MatrixRowsSwap(m, r, i, j);
                break;
            }
        }
        if (m(r, j) == 0) continue;
        for (unsigned i = r + 1; i < m.Rows(); ++i)
        {
            if (m(i, j) == 0) continue;
            TValue rm = m(r, j) / m(i, j);
            MatrixRowsSubM(m, i, r, rm, j);
        }
        ++r;
    }
    return r;
}
