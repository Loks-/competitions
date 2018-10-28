#pragma once

#include "vector.h"
// #include "../../numeric/utils/bits_count.h"
#include <algorithm>

class MatrixBool : public VectorBool
{
public:
    using TBase = VectorBool;
	using TValue = TBase::TValue;
    using TDataValue = TBase::TDataValue;
	using TSelf = MatrixBool;
    const unsigned bits_per_value = VectorBool::bits_per_value;

	using diterator = TBase::diterator;
	using const_diterator = TBase::const_diterator;

protected:
	unsigned rows;
	unsigned columns;
    unsigned data_columns, total_columns;

public:
    unsigned Rows() const { return rows; }
    unsigned Columns() const { return columns; }
    unsigned DataColumns() const { return data_columns; }
    unsigned TotalColumns() const { return total_columns; }

	MatrixBool(unsigned _rows, unsigned _columns) : 
        TBase(_rows * ((_columns + bits_per_value - 1) & ~(bits_per_value - 1))), 
        rows(_rows), columns(_columns),
        data_columns((_columns + bits_per_value - 1) / bits_per_value), total_columns(data_columns * bits_per_value) {}
    MatrixBool(unsigned size) : MatrixBool(size, size) {}
	MatrixBool(unsigned _rows, unsigned _columns, const TValue& v) : 
        MatrixBool(_rows, _columns) { TBase::Fill(v); }
	TSelf& operator=(const TValue& v) { TBase::Fill(v); return *this; }
	
    void swap(TSelf& r)
    {
        TBase::swap(r);
        std::swap(rows, r.rows);
        std::swap(columns, r.columns);
        std::swap(data_columns, r.data_columns);
        std::swap(total_columns, r.total_columns);
    }

	TValue Get(unsigned i, unsigned j) const { return TBase::Get(i * total_columns + j); }
    void Set(unsigned i, unsigned j, TValue v) { TBase::Set(i * total_columns + j, v); }
    TValue operator()(unsigned i, unsigned j) const { return Get(i, j); }

	diterator GetDP(unsigned i, unsigned dj) { return TBase::GetDP(i * data_columns + dj);}
	const_diterator GetDP(unsigned i, unsigned dj) const { return TBase::GetDP(i * data_columns + dj);}

	void SetDiagonal(const TValue& v)
	{
		unsigned diagonal_length = std::min(rows, columns);
        for (unsigned i = 0; i < diagonal_length; ++i)
            Set(i, i, v);
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
		for (unsigned i = 0; i < rows; ++i)
		{
			for (unsigned j = 0; j < columns; ++j)
			{
				if (Get(i, j).Get())
                {
                    const_diterator pB = v.GetDP(j, 0);
                    for (diterator pC = output.GetDP(i, 0), pCend = pC + data_columns; pC < pCend; )
                        *pC++ ^= *pB++;
                }
			}
		}
	}

    // void MultAx(const TBase& x, TBase& output) const
    // {
    //     assert((x.Size() == columns) && (output.Size() == rows));
    //     output.Clear();
	// 	const_diterator pA = TBase::DBegin(), pBbegin = x.DBegin(), pBend1 = x.DEnd() - 1;
	// 	for (unsigned i = 0; i < rows; ++i)
	// 	{
    //         TDataValue t = 0;
    //         for (const_diterator pB = pBbegin; pB < pBend1; )
    //             t ^= *pA++ & *pB++;
    //         t ^= *pA++ & *pBend1 & ((1ull << (((columns - 1) % bits_per_value) + 1)) - 1);
    //         output.Set(i, TValue(BitsCount(t)));
	// 	}
    // }

    // TBase operator*(const TBase& x) const { TBase t(rows); MultAx(x, t); return t; }
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
