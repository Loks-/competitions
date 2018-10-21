#pragma once

#include "common/base.h"
#include "common/linear_algebra/matrix.h"

template<class TMatrix>
inline void MatrixMultPointers(const TMatrix& A, const TMatrix& B, TMatrix& output)
{
	using TValue = typename TMatrix::TValue;
	assert((B.Rows() == A.Columns()) && (output.Rows() == A.Rows()) && (output.Columns() == B.Columns()));
	unsigned rows = A.Rows(), columns = A.Columns(), columns2 = output.Columns();
	output.Clear();
	const TValue* pA = A.begin();
	for (unsigned i = 0; i < rows; ++i)
	{
		const TValue* pB = B.begin();
		for (unsigned j = 0; j < columns; ++j)
		{
			const TValue& vA = *pA++;
			for (TValue* pC = output.begin() + i * columns2, *pCend = pC + columns2; pC < pCend; )
				*pC++ += *pB++ * vA;
		}
	}
}

template<class TMatrix>
inline void MatrixMultLoops(const TMatrix& A, const TMatrix& B, TMatrix& output)
{
	assert((B.Rows() == A.Columns()) && (output.Rows() == A.Rows()) && (output.Columns() == B.Columns()));
	output.Clear();
	for (unsigned i = 0; i < A.Rows(); ++i)
	{
		for (unsigned j = 0; j < A.Columns(); ++j)
		{
            for (unsigned k = 0; k < B.Columns(); ++k)
                output(i, k) += A(i, j) * B(j, k);
		}
	}
}
