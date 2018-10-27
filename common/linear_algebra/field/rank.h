#pragma once

#include "../rows/sub_m.h"
#include "../rows/swap.h"

template<class TMatrix>
inline unsigned FMatrixRank(const TMatrix& matrix)
{
    using TValue = typename TMatrix::TValue;
    TMatrix m(matrix);
    unsigned r = 0;
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
