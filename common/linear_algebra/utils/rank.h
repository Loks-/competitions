#pragma once

#include "../matrix.h"
#include <algorithm>
#include <iostream>

template<class TValue>
unsigned MatrixRank(const Matrix<TValue>& matrix)
{
    unsigned r = 0;
    Matrix<TValue> m(matrix);
    for (unsigned j = 0; (r < m.Rows()) && (j < m.Columns()); ++j)
    {
        for (unsigned i = r; i < m.Rows(); ++i)
        {
            if (m(i, j) != 0)
            {
                if (i != r)
                {
                    for (unsigned k = j; k < m.Columns(); ++k)
                        std::swap(m(r, k), m(i, k));
                    // TValue *pr = &(m(r, j)), *pr_end = pr + (m.Rows() - j), *pi = &(m(i, j));
                    // for (; pr < pr_end; ++pi, ++pr) std::swap(*pi, *pr);
                }
                break;
            }
        }
        if (m(r, j) == 0) continue;
        for (unsigned i = r + 1; i < m.Rows(); ++i)
        {
            if (m(i, j) == 0) continue;
            TValue rm = m(r, j) / m(i, j);
            for (unsigned k = j; k < m.Columns(); ++k)
                m(i, k) -= rm * m(r, k);
            // TValue *pr = &(m(r, j)), *pr_end = pr + (m.Rows() - j), *pi = &(m(i, j));
            // std::cout << "\t" << i << "\t" << pr << "\t" << pr_end << "\t" << pi << std::endl;
            // for (; pr < pr_end; ) *pi++ -= rm * *pr++;
        }
        ++r;
    }
    return r;
}
