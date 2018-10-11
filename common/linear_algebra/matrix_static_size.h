#pragma once

#include "vector_static_size.h"
#include <algorithm>

template<class TTValue, unsigned _rows, unsigned _columns>
class MatrixStaticSize : public VectorStaticSize<TTValue, _rows * _columns>
{
public:
	const static unsigned rows = _rows;
	const static unsigned columns = _columns;
	const static bool is_square = (rows == columns);

	using TValue = TTValue;
	using TSelf = MatrixStaticSize<TValue, rows, columns>;
	using TBase = VectorStaticSize<TValue, rows * columns>;

public:
	MatrixStaticSize() { TBase::Clear(); }
	MatrixStaticSize(const TValue& v) { TBase::Fill(v); }
	TSelf& operator=(const TValue& v) { TBase::Fill(v); return *this; }

	TValue& operator()(unsigned i, unsigned j) { return TBase::data[i * columns + j]; }
	const TValue& operator()(unsigned i, unsigned j) const { return TBase::data[i * columns + j]; }
	void swap(TSelf& r) { TBase::swap(r); }

	void SetDiagonal(const TValue& v)
	{
		const unsigned diagonal_length = std::min(rows, columns);
		unsigned shift = columns + 1;
		for (TValue* p = TBase::begin(), *pend = p + diagonal_length * shift; p < pend; p += shift) *p = v;
	}

	TSelf& operator+=(const TSelf& v) { TBase::operator+=(v); return *this; }
	TSelf& operator-=(const TSelf& v) { TBase::operator-=(v); return *this; }
	TSelf operator+(const TSelf& v) const { TSelf t(*this); t += v; return t; }
	TSelf operator-(const TSelf& v) const { TSelf t(*this); t -= v; return t; }

	template<unsigned columns2>
	void Mult(const MatrixStaticSize<TValue, columns, columns2>& v, MatrixStaticSize<TValue, rows, columns2>& output) const
	{
		output.Clear();
		const TValue* pA = TBase::begin();
		for (unsigned i = 0; i < rows; ++i)
		{
			const TValue* pB = v.begin();
			for (unsigned j = 0; j < columns; ++j)
			{
				const TValue& vA = *pA++;
				for (TValue* pC = output.begin() + i * columns2, *pCend = pC + columns2; pC < pCend; )
					*pC++ += *pB++ * vA;
			}
		}
	}

	template<unsigned columns2>
	MatrixStaticSize<TValue, rows, columns2> operator*(const MatrixStaticSize<TValue, columns, columns2>& v) const
	{
		MatrixStaticSize<TValue, rows, columns2> t; Mult(v, t); return t;
	}

	// Operations with square matrix
	TSelf& operator*=(const TSelf& v) 
	{ 
		static_assert(is_square, "matrix should be square");
		TSelf t; Mult(v, t); swap(t); return *this;
	}

	TSelf PowU(uint64_t pow) const
	{
		static_assert(is_square, "matrix should be square");
		TSelf ans; ans.SetDiagonal(TValue(1));
		TSelf x = *this;
		for (; pow; pow >>= 1)
		{
			if (pow & 1)
				ans *= x;
			x *= x;
		}
		return ans;
	}
};

template<unsigned rows, unsigned columns>
using TDMatrixStaticSize = MatrixStaticSize<double, rows, columns>;
template<class TValue, unsigned size>
using TSquareMatrixStaticSize = MatrixStaticSize<TValue, size, size>;
template<unsigned size>
using TDSquareMatrixStaticSize = MatrixStaticSize<double, size, size>;
