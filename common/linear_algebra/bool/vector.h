#pragma once

#include "common/base.h"
#include "common/modular/static/bool.h"
#include "common/template.h"
#include "common/templates/bits.h"

#include <algorithm>
#include <vector>

namespace la {
class VectorBool {
 public:
  using TValue = ModularBool;
  using TBlockValue = uint64_t;
  using TData = std::vector<TBlockValue>;
  using TSelf = VectorBool;

 public:
  static constexpr unsigned bits_per_block =
      templates::SizeInBits<TBlockValue>();

  static constexpr unsigned MinBlockSize(unsigned size) {
    return (size + bits_per_block - 1) / bits_per_block;
  }

 protected:
  unsigned size;
  TData data;

  constexpr VectorBool() : size(0) {}

  constexpr void Resize(unsigned new_size) {
    size = new_size;
    data.resize(MinBlockSize(size));
  }

 public:
  using biterator = TBlockValue*;
  using const_biterator = const TBlockValue*;

  constexpr biterator BBegin() { return &data.front(); }

  constexpr const_biterator BBegin() const { return &data.front(); }

  constexpr biterator BEnd() { return BBegin() + DSize(); }

  constexpr const_biterator BEnd() const { return BBegin() + DSize(); }

  constexpr biterator GetBP(unsigned block_index) {
    return BBegin() + block_index;
  }

  constexpr const_biterator GetBP(unsigned block_index) const {
    return BBegin() + block_index;
  }

  constexpr void Fill(const TValue& v) {
    std::fill(data.begin(), data.end(), v.Get() ? ~0ull : 0ull);
  }

  constexpr void Clear() { Fill(TValue()); }

  constexpr void Complement() {
    for (biterator p = BBegin(), pend = BEnd(); p < pend; ++p) *p = ~*p;
  }

  constexpr unsigned Size() const { return size; }

  constexpr unsigned DSize() const { return unsigned(data.size()); }

  constexpr const TData& GetData() const { return data; }

  constexpr explicit VectorBool(unsigned _size) { Resize(_size); }

  constexpr VectorBool(unsigned _size, const TValue& v) {
    Resize(_size);
    if (v.Get()) Fill(v);
  }

  constexpr TSelf& operator=(const TValue& v) {
    Fill(v);
    return *this;
  }

  constexpr void swap(TSelf& r) { data.swap(r.data); }

  constexpr TValue Get(unsigned i) const {
    TValue t;
    t.SetU(data[i / bits_per_block] >> (i % bits_per_block));
    return t;
  }

  constexpr TBlockValue GetBit(unsigned i) const {
    return (data[i / bits_per_block] >> (i % bits_per_block)) & 1ull;
  }

  constexpr void Set(unsigned i, TValue v) {
    TBlockValue& r = data[i / bits_per_block];
    const unsigned shift = i % bits_per_block;
    r = (v.Get() ? (r | (1ull << shift)) : (r & (~(1ull << shift))));
  }

  constexpr void Complement(unsigned i) {
    TBlockValue& r = data[i / bits_per_block];
    const unsigned shift = i % bits_per_block;
    r ^= (1ull << shift);
  }

  constexpr void Swap(unsigned i, unsigned j) {
    if (GetBit(i) ^ GetBit(j)) {
      Complement(i);
      Complement(j);
    }
  }

  constexpr TSelf& operator+=(const TValue& v) {
    if (v.Get()) Complement();
    return *this;
  }

  constexpr TSelf& operator-=(const TValue& v) {
    if (v.Get()) Complement();
    return *this;
  }

  constexpr TSelf& operator*=(const TValue& v) {
    if (!v.Get()) Clear();
    return *this;
  }

  constexpr TSelf& operator/=(const TValue& v) {
    MetaUse(v);
    assert(v.Get());
    return *this;
  }

  constexpr TSelf operator+(const TValue& v) const {
    if (v.Get()) {
      TSelf t(*this);
      t.Complement();
      return t;
    }
    return *this;
  }

  constexpr TSelf operator-(const TValue& v) const {
    if (v.Get()) {
      TSelf t(*this);
      t.Complement();
      return t;
    }
    return *this;
  }

  constexpr TSelf operator*(const TValue& v) const {
    return (v.Get() ? *this : TSelf(size));
  }

  constexpr TSelf operator/(const TValue& v) const {
    MetaUse(v);
    assert(v.Get());
    return *this;
  }

  constexpr TSelf operator-() const { return *this; }

  constexpr TSelf& operator+=(const TSelf& v) {
    assert(Size() == v.Size());
    const_biterator pv = v.BBegin();
    for (biterator p = BBegin(), pend = BEnd(); p < pend;) *p++ ^= *pv++;
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& v) {
    assert(Size() == v.Size());
    const_biterator pv = v.BBegin();
    for (biterator p = BBegin(), pend = BEnd(); p < pend;) *p++ ^= *pv++;
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
};
}  // namespace la
