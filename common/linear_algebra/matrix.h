#pragma once

#include "vector.h"
#include <algorithm>

template<class TTValue>
class Matrix : public Vector<TTValue>
{
public:
	using TValue = TTValue;
	using TSelf = Matrix<TValue>;
	using TBase = Vector<TValue>;

protected:
	unsigned rows;
	unsigned columns;

public:
    unsigned Rows() const { return rows; }
    unsigned Columns() const { return columns; }

	Matrix(unsigned size) : TBase(size * size), rows(size), columns(size) {}
	Matrix(unsigned _rows, unsigned _columns) : TBase(_rows * _columns), rows(_rows), columns(_columns) {}
	Matrix(unsigned _rows, unsigned _columns, const TValue& v) : TBase(_rows * _columns, v), rows(_rows), columns(_columns) {}
	TSelf& operator=(const TValue& v) { TBase::Fill(v); return *this; }

	TValue& operator()(unsigned i, unsigned j) { return TBase::data[i * columns + j]; }
	const TValue& operator()(unsigned i, unsigned j) const { return TBase::data[i * columns + j]; }
	void swap(TSelf& r) { TBase::swap(r); std::swap(rows, r.rows); std::swap(columns, r.columns); }

	void SetDiagonal(const TValue& v)
	{
		unsigned diagonal_length = std::min(rows, columns);
		unsigned shift = columns + 1;
		for (TValue* p = TBase::begin(), *pend = p + diagonal_length * shift; p < pend; p += shift) *p = v;
	}

	TSelf& operator+=(const TSelf& v) { TBase::operator+=(v); return *this; }
	TSelf& operator-=(const TSelf& v) { TBase::operator-=(v); return *this; }
	TSelf operator+(const TSelf& v) const { TSelf t(*this); t += v; return t; }
	TSelf operator-(const TSelf& v) const { TSelf t(*this); t -= v; return t; }

	void Mult(const TSelf& v, TSelf& output) const
	{
        assert((v.rows == columns) && (output.rows == rows) && (output.columns == v.columns));
        unsigned columns2 = output.columns;
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

    void MultAx(const TBase& x, TBase& output) const
    {
        assert((x.Size() == columns) && (output.Size() == rows));
        output.Clear();
		const TValue *pA = TBase::begin(), *pBbegin = x.begin(), *pBend = x.end();
        TValue* pO = output.begin();
		for (unsigned i = 0; i < rows; ++i, ++pO)
		{
            for (const TValue* pB = pBbegin; pB < pBend; )
                *pO += *pA++ * *pB++;
		}
    }

    TBase operator*(const TBase& x) const { TBase t(rows); MultAx(x, t); return t; }
	TSelf operator*(const TSelf& v) const { TSelf t(rows, v.columns); Mult(v, t); return t; }
	TSelf& operator*=(const TSelf& v) { TSelf t(rows, v.columns); Mult(v, t); swap(t); return *this; }

	TSelf PowU(uint64_t pow) const
	{
        assert(rows == columns);
		TSelf ans(rows, columns); ans.SetDiagonal(TValue(1));
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

using TDMatrix = Matrix<double>;