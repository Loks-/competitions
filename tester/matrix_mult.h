#pragma once

#include "common/base.h"
#include "common/linear_algebra/matrix.h"

template<class TValue>
inline void MatrixMultPointers(const Matrix<TValue>& A, const Matrix<TValue>& B, Matrix<TValue>& output)
{
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

template<class TValue>
inline void MatrixMultLoops(const Matrix<TValue>& A, const Matrix<TValue>& B, Matrix<TValue>& output)
{
	assert((B.Rows() == A.Columns()) && (output.Rows() == A.Rows()) && (output.Columns() == B.Columns()));
	unsigned rows = A.Rows(), columns = A.Columns(), columns2 = output.Columns();
	output.Clear();
	for (unsigned i = 0; i < rows; ++i)
	{
		for (unsigned j = 0; j < columns; ++j)
		{
            for (unsigned k = 0; k < columns2; ++k)
                output(i, k) += A(i, j) * B(j, k);
		}
	}
}
