#pragma once

#include "matrix.h"
#include "rows_block_add.h"
#include "rows_block_swap.h"
#include "vector.h"
#include "../../base.h"
#include <vector>

// Solve Ax = b
inline bool BMatrixSolve(const MatrixBool& A, const VectorBool& b, VectorBool& output_x)
{
    assert((A.Rows() == b.Size()) && (A.Columns() == output_x.Size()));
    MatrixBool m(A);
    VectorBool v(b);
    unsigned r = 0;
    std::vector<unsigned> vj(m.Rows());
    for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j)
    {
        for (unsigned i = r; i < m.Rows(); ++i)
        {
            if (m.GetBit(i, j))
            {
                MatrixRowsBSwap(m, r, i, j / MatrixBool::bits_per_block);
                v.Swap(r, i);
                break;
            }
        }
        if (m.GetBit(r, j) == 0) continue;
        VectorBool::TBlockValue vrb = v.GetBit(r);
        for (unsigned i = r + 1; i < m.Rows(); ++i)
        {
            if (m.GetBit(i, j) == 0) continue;
            MatrixRowsBAdd(m, i, r, j / MatrixBool::bits_per_block);
            if (vrb)
                v.Complement(i);
        }
        vj[r++] = j;
    }
    for (unsigned i = r; i < m.Rows(); ++i)
    {
        if (v.GetBit(i))
            return false;
    }
    output_x.Clear();
    for (; r--; )
    {
        unsigned j = vj[r];
        if (v.GetBit(r) == 0) continue;
        for (unsigned i = 0; i < r; ++i)
        {
            if (m.GetBit(i, j))
                v.Complement(i);
        }
        output_x.Set(j, 1);
    }
    return true;
}