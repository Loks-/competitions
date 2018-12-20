#pragma once

#include "../../base.h"
#include "../../modular/static/modular_bool.h"
#include <algorithm>
#include <vector>

class VectorBool {
 public:
  using TValue = ModularBool;
  using TBlockValue = uint64_t;
  using TData = std::vector<TBlockValue>;
  using TSelf = VectorBool;

  const static unsigned bits_per_block = 8 * sizeof(TBlockValue);

  static unsigned MinBlockSize(unsigned size) {
    return (size + bits_per_block - 1) / bits_per_block;
  }

 protected:
  unsigned size;
  TData data;

  VectorBool() : size(0) {}
  void Resize(unsigned new_size) {
    size = new_size;
    data.resize(MinBlockSize(size));
  }

 public:
  using biterator = TBlockValue*;
  using const_biterator = const TBlockValue*;

  biterator BBegin() { return &data.front(); }
  const_biterator BBegin() const { return &data.front(); }
  biterator BEnd() { return BBegin() + DSize(); }
  const_biterator BEnd() const { return BBegin() + DSize(); }
  biterator GetBP(unsigned block_index) { return BBegin() + block_index; }
  const_biterator GetBP(unsigned block_index) const {
    return BBegin() + block_index;
  }

  void Fill(const TValue& v) {
    std::fill(data.begin(), data.end(), v.Get() ? ~0ull : 0ull);
  }

  void Clear() { Fill(TValue(0)); }

  void Complement() {
    for (biterator p = BBegin(), pend = BEnd(); p < pend; ++p) *p = ~*p;
  }
  unsigned Size() const { return size; }
  unsigned DSize() const { return unsigned(data.size()); }
  const TData& GetData() const { return data; }

  VectorBool(unsigned _size) { Resize(_size); }

  VectorBool(unsigned _size, const TValue& v) {
    Resize(_size);
    if (v.Get()) Fill(v);
  }

  TSelf& operator=(const TValue& v) {
    Fill(v);
    return *this;
  }

  void swap(TSelf& r) { data.swap(r.data); }

  TValue Get(unsigned i) const {
    TValue t;
    t.SetU(data[i / bits_per_block] >> (i % bits_per_block));
    return t;
  }

  TBlockValue GetBit(unsigned i) const {
    return (data[i / bits_per_block] >> (i % bits_per_block)) & 1ull;
  }

  void Set(unsigned i, TValue v) {
    TBlockValue& r = data[i / bits_per_block];
    unsigned shift = i % bits_per_block;
    r = (v.Get() ? (r | (1ull << shift)) : (r & (~(1ull << shift))));
  }

  void Complement(unsigned i) {
    TBlockValue& r = data[i / bits_per_block];
    unsigned shift = i % bits_per_block;
    r ^= (1ull << shift);
  }

  void Swap(unsigned i, unsigned j) {
    if (GetBit(i) ^ GetBit(j)) {
      Complement(i);
      Complement(j);
    }
  }

  TSelf& operator+=(const TValue& v) {
    if (v.Get()) Complement();
    return *this;
  }

  TSelf& operator-=(const TValue& v) {
    if (v.Get()) Complement();
    return *this;
  }

  TSelf& operator*=(const TValue& v) {
    if (!v.Get()) Clear();
    return *this;
  }

  TSelf& operator/=(const TValue& v) {
    assert(v.Get());
    return *this;
  }

  TSelf operator+(const TValue& v) const {
    if (v.Get()) {
      TSelf t(*this);
      t.Complement();
      return t;
    }
    return *this;
  }

  TSelf operator-(const TValue& v) const {
    if (v.Get()) {
      TSelf t(*this);
      t.Complement();
      return t;
    }
    return *this;
  }

  TSelf operator*(const TValue& v) const {
    return (v.Get() ? *this : TSelf(size));
  }

  TSelf operator/(const TValue& v) const {
    assert(v.Get());
    return *this;
  }

  TSelf operator-() const { return *this; }

  TSelf& operator+=(const TSelf& v) {
    assert(Size() == v.Size());
    const_biterator pv = v.BBegin();
    for (biterator p = BBegin(), pend = BEnd(); p < pend;) *p++ ^= *pv++;
    return *this;
  }

  TSelf& operator-=(const TSelf& v) {
    assert(Size() == v.Size());
    const_biterator pv = v.BBegin();
    for (biterator p = BBegin(), pend = BEnd(); p < pend;) *p++ ^= *pv++;
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
};
