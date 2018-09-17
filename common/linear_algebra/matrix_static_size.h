#pragma once

#include <algorithm>
#include <array>

template<class TTValue, unsigned _rows, unsigned _columns>
class MatrixStaticSize
{
public:
	const static unsigned rows = _rows;
	const static unsigned columns = _columns;
	const static unsigned size = rows * columns;
	const static bool is_square = (rows == columns);

	using TValue = TTValue;
	using TData = std::array<TValue, rows * columns>;
	using TSelf = MatrixStaticSize<TTValue, rows, columns>;

	using iterator = TValue*;
	using const_iterator = const TValue*;

public:
	TData data;

	void Clear() { data.fill(TValue(0)); }
	MatrixStaticSize() { Clear(); }
	MatrixStaticSize(const TValue& v) { data.fill(v); }
	TSelf& operator=(const TValue& v) { data.fill(v); return *this; }

	TValue& operator()(unsigned i, unsigned j) { return data[i * columns + j]; }
	const TValue& operator()(unsigned i, unsigned j) const { return data[i * columns + j]; }
	iterator begin() { return &data.front(); }
	const_iterator begin() const { return &data.front(); }
	iterator end() { return begin() + size; }
	const_iterator end() const { return begin() + size; }
	void swap(TSelf& r) { data.swap(r.data); }

	void SetDiagonal(const TValue& v)
	{
		const unsigned diagonal_length = std::min(rows, columns);
		unsigned shift = columns + 1;
		for (TValue* p = begin(), *pend = p + diagonal_length * shift; p < pend; p += shift) *p = v;
	}

	TSelf& operator+=(const TValue& v) { for (TValue *p = begin(), *pend = end(); p < pend; ) *p++ += v; return *this; }
	TSelf& operator-=(const TValue& v) { for (TValue *p = begin(), *pend = end(); p < pend; ) *p++ -= v; return *this; }
	TSelf& operator*=(const TValue& v) { for (TValue *p = begin(), *pend = end(); p < pend; ) *p++ *= v; return *this; }
	TSelf& operator/=(const TValue& v) { for (TValue *p = begin(), *pend = end(); p < pend; ) *p++ /= v; return *this; }
	TSelf operator+(const TValue& v) const { TSelf t(*this); t += v; return t; }
	TSelf operator-(const TValue& v) const { TSelf t(*this); t -= v; return t; }
	TSelf operator*(const TValue& v) const { TSelf t(*this); t *= v; return t; }
	TSelf operator/(const TValue& v) const { TSelf t(*this); t /= v; return t; }
	TSelf operator-() const { TSelf t(*this); for (TValue *p = begin(), *pend = end(); p < pend; ++p) *p = -*p;  return t; }

	TSelf& operator+=(const TSelf& v) { const TValue* pv = v.begin(); for (TValue *p = begin(), *pend = end(); p < pend; ) *p++ += *pv++; return *this; }
	TSelf& operator-=(const TSelf& v) { const TValue* pv = v.begin(); for (TValue *p = begin(), *pend = end(); p < pend; ) *p++ -= *pv++; return *this; }
	TSelf operator+(const TSelf& v) const { TSelf t(*this); t += v; return t; }
	TSelf operator-(const TSelf& v) const { TSelf t(*this); t -= v; return t; }

	template<unsigned columns2>
	void Mult(const MatrixStaticSize<TValue, columns, columns2>& v, MatrixStaticSize<TValue, rows, columns2>& output) const
	{
		output.Clear();
		const TValue* pA = begin();
		for (unsigned i = 0; i < rows; ++i)
		{
			const TValue* pB = v.begin();
			for (unsigned j = 0; j < columns; ++j)
			{
				const TValue& vA = *pA++;
				auto itC = output.begin() + i * columns2;
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
		TSelf t; Mult(v, t); data.swap(t.data); return *this;
	}

	TSelf PowU(uint64_t pow) const
	{
		static_assert(is_square, "matrix should be square");
		TSelf ans; ans.SetDiagonal(1);
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
