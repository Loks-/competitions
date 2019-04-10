#pragma once

#include "common/linear_algebra/bool/vector.h"
// #include "common/numeric/utils/bits_count.h"
#include <algorithm>

class MatrixBool : public VectorBool {
 public:
  using TBase = VectorBool;
  using TValue = TBase::TValue;
  using TBlockValue = TBase::TBlockValue;
  using TSelf = MatrixBool;
  const static unsigned bits_per_block = VectorBool::bits_per_block;

  using biterator = TBase::biterator;
  using const_biterator = TBase::const_biterator;

 protected:
  unsigned rows;
  unsigned columns;
  unsigned blocks_per_row, bits_per_row;

 public:
  unsigned Rows() const { return rows; }
  unsigned Columns() const { return columns; }
  unsigned BlocksPerRow() const { return blocks_per_row; }
  unsigned BitsPerRow() const { return bits_per_row; }

  MatrixBool(unsigned _rows, unsigned _columns)
      : rows(_rows), columns(_columns) {
    blocks_per_row = TBase::MinBlockSize(columns);
    bits_per_row = blocks_per_row * bits_per_block;
    TBase::Resize(rows * bits_per_row);
  }

  MatrixBool(unsigned size) : MatrixBool(size, size) {}

  MatrixBool(unsigned _rows, unsigned _columns, const TValue& v)
      : MatrixBool(_rows, _columns) {
    if (v.Get()) TBase::Fill(v);
  }

  TSelf& operator=(const TValue& v) {
    TBase::Fill(v);
    return *this;
  }

  void swap(TSelf& r) {
    TBase::swap(r);
    std::swap(rows, r.rows);
    std::swap(columns, r.columns);
    std::swap(blocks_per_row, r.blocks_per_row);
    std::swap(bits_per_row, r.bits_per_row);
  }

  TValue Get(unsigned i, unsigned j) const {
    return TBase::Get(i * bits_per_row + j);
  }

  TBlockValue GetBit(unsigned i, unsigned j) const {
    return TBase::GetBit(i * bits_per_row + j);
  }

  void Set(unsigned i, unsigned j, TValue v) {
    TBase::Set(i * bits_per_row + j, v);
  }

  biterator GetBP(unsigned row, unsigned block_index) {
    return TBase::GetBP(row * blocks_per_row + block_index);
  }

  const_biterator GetBP(unsigned row, unsigned block_index) const {
    return TBase::GetBP(row * blocks_per_row + block_index);
  }

  void SetDiagonal(const TValue& v) {
    unsigned diagonal_length = std::min(rows, columns);
    for (unsigned i = 0; i < diagonal_length; ++i) Set(i, i, v);
  }

  TSelf& operator+=(const TSelf& v) {
    TBase::operator+=(v);
    return *this;
  }

  TSelf& operator-=(const TSelf& v) {
    TBase::operator-=(v);
    return *this;
  }

  TSelf operator+(const TSelf& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  TSelf operator-(const TSelf& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  void Mult(const TSelf& v, TSelf& output) const {
    assert((v.rows == columns) && (output.rows == rows) &&
           (output.columns == v.columns));
    unsigned columns2 = output.columns;
    output.Clear();
    for (unsigned i = 0; i < rows; ++i) {
      for (unsigned j = 0; j < columns; ++j) {
        if (GetBit(i, j)) {
          const_biterator pB = v.GetBP(j, 0);
          for (biterator pC = output.GetBP(i, 0), pCend = pC + blocks_per_row;
               pC < pCend;)
            *pC++ ^= *pB++;
        }
      }
    }
  }

  // void MultAx(const TBase& x, TBase& output) const
  // {
  //     assert((x.Size() == columns) && (output.Size() == rows));
  //     output.Clear();
  // 	const_biterator pA = TBase::BBegin(), pBbegin = x.BBegin(), pBend1 =
  // x.BEnd() - 1; 	for (unsigned i = 0; i < rows; ++i)
  // 	{
  //         TBlockValue t = 0;
  //         for (const_biterator pB = pBbegin; pB < pBend1; )
  //             t ^= *pA++ & *pB++;
  //         t ^= *pA++ & *pBend1 & ((1ull << (((columns - 1) % bits_per_value)
  //         + 1)) - 1); output.Set(i, TValue(BitsCount(t)));
  // 	}
  // }

  // TBase operator*(const TBase& x) const { TBase t(rows); MultAx(x, t); return
  // t; }

  TSelf operator*(const TSelf& v) const {
    TSelf t(rows, v.columns);
    Mult(v, t);
    return t;
  }

  TSelf& operator*=(const TSelf& v) {
    TSelf t(rows, v.columns);
    Mult(v, t);
    swap(t);
    return *this;
  }

  TSelf PowU(uint64_t pow) const {
    assert(rows == columns);
    TSelf ans(rows, columns);
    ans.SetDiagonal(TValue(1));
    TSelf x = *this;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans *= x;
      x *= x;
    }
    return ans;
  }
};
