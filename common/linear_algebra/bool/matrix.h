#pragma once

#include "common/linear_algebra/bool/vector.h"
// #include "common/numeric/utils/bits_count.h"
#include <algorithm>

namespace la {
class MatrixBool : public VectorBool {
 public:
  using TBase = VectorBool;
  using TValue = TBase::TValue;
  using TBlockValue = TBase::TBlockValue;
  using TSelf = MatrixBool;

  using biterator = TBase::biterator;
  using const_biterator = TBase::const_biterator;

 public:
  static constexpr unsigned bits_per_block = VectorBool::bits_per_block;

 protected:
  unsigned rows;
  unsigned columns;
  unsigned blocks_per_row, bits_per_row;

 public:
  constexpr unsigned Rows() const { return rows; }

  constexpr unsigned Columns() const { return columns; }

  constexpr unsigned BlocksPerRow() const { return blocks_per_row; }

  constexpr unsigned BitsPerRow() const { return bits_per_row; }

  constexpr MatrixBool(unsigned _rows, unsigned _columns)
      : rows(_rows), columns(_columns) {
    blocks_per_row = TBase::MinBlockSize(columns);
    bits_per_row = blocks_per_row * bits_per_block;
    TBase::Resize(rows * bits_per_row);
  }

  constexpr explicit MatrixBool(unsigned size) : MatrixBool(size, size) {}

  constexpr MatrixBool(unsigned _rows, unsigned _columns, const TValue& v)
      : MatrixBool(_rows, _columns) {
    if (v.Get()) TBase::Fill(v);
  }

  constexpr TSelf& operator=(const TValue& v) {
    TBase::Fill(v);
    return *this;
  }

  constexpr void swap(TSelf& r) {
    TBase::swap(r);
    std::swap(rows, r.rows);
    std::swap(columns, r.columns);
    std::swap(blocks_per_row, r.blocks_per_row);
    std::swap(bits_per_row, r.bits_per_row);
  }

  constexpr TValue Get(unsigned i, unsigned j) const {
    return TBase::Get(i * bits_per_row + j);
  }

  constexpr TBlockValue GetBit(unsigned i, unsigned j) const {
    return TBase::GetBit(i * bits_per_row + j);
  }

  constexpr void Set(unsigned i, unsigned j, TValue v) {
    TBase::Set(i * bits_per_row + j, v);
  }

  constexpr biterator GetBP(unsigned row, unsigned block_index) {
    return TBase::GetBP(row * blocks_per_row + block_index);
  }

  constexpr const_biterator GetBP(unsigned row, unsigned block_index) const {
    return TBase::GetBP(row * blocks_per_row + block_index);
  }

  constexpr void SetDiagonal(const TValue& v) {
    const unsigned diagonal_length = std::min(rows, columns);
    for (unsigned i = 0; i < diagonal_length; ++i) Set(i, i, v);
  }

  constexpr TSelf& operator+=(const TSelf& v) {
    TBase::operator+=(v);
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& v) {
    TBase::operator-=(v);
    return *this;
  }

  constexpr TSelf operator+(const TSelf& v) const {
    TSelf t(*this);
    t += v;
    return t;
  }

  constexpr TSelf operator-(const TSelf& v) const {
    TSelf t(*this);
    t -= v;
    return t;
  }

  constexpr void Mult(const TSelf& v, TSelf& output) const {
    assert((v.rows == columns) && (output.rows == rows) &&
           (output.columns == v.columns));
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

  // void MultAx(const TBase& x, TBase& output) const {
  //   assert((x.Size() == columns) && (output.Size() == rows));
  //   output.Clear();
  //   const_biterator pA = TBase::BBegin(), pBbegin = x.BBegin(),
  //                   pBend1 = x.BEnd() - 1;
  //   for (unsigned i = 0; i < rows; ++i) {
  //     TBlockValue t = 0;
  //     for (const_biterator pB = pBbegin; pB < pBend1;) t ^= *pA++ & *pB++;
  //     t ^= *pA++ & *pBend1 &
  //          ((1ull << (((columns - 1) % bits_per_value) + 1)) - 1);
  //     output.Set(i, TValue(BitsCount(t)));
  //   }
  // }

  // TBase operator*(const TBase& x) const {
  //   TBase t(rows);
  //   MultAx(x, t);
  //   return t;
  // }

  constexpr TSelf operator*(const TSelf& v) const {
    TSelf t(rows, v.columns);
    Mult(v, t);
    return t;
  }

  constexpr TSelf& operator*=(const TSelf& v) {
    TSelf t(rows, v.columns);
    Mult(v, t);
    swap(t);
    return *this;
  }

  constexpr TSelf PowU(uint64_t pow) const {
    assert(rows == columns);
    TSelf ans(rows, columns);
    ans.SetDiagonal(TValue::True());
    TSelf x = *this;
    for (; pow; pow >>= 1) {
      if (pow & 1) ans *= x;
      x *= x;
    }
    return ans;
  }
};
}  // namespace la
