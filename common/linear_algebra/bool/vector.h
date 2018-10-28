#pragma once

#include "../../base.h"
#include "../../modular/modular_bool.h"
#include <algorithm>
#include <vector>

class VectorBool
{
public:
	using TValue = ModularBool;
    using TDataValue = uint64_t;
	using TData = std::vector<TDataValue>;
	using TSelf = VectorBool;
    const unsigned bits_per_value = 64;

protected:
    unsigned size;
	TData data;

public:
	using diterator = TDataValue*;
	using const_diterator = const TDataValue*;

	diterator DBegin() { return &data.front(); }
	const_diterator DBegin() const { return &data.front(); }
	diterator DEnd() { return DBegin() + DSize(); }
	const_diterator DEnd() const { return DBegin() + DSize(); }
	diterator GetDP(unsigned i) { return DBegin() + i;}
	const_diterator GetDP(unsigned i) const { return DBegin() + i;}

	void Fill(const TValue& v) { std::fill(data.begin(), data.end(), v.Get() ? ~0ull : 0ull); }
	void Clear() { Fill(TValue(0)); }
    void Complement() { for (diterator p = DBegin(), pend = DEnd(); p < pend; ++p) *p = ~*p; }
	unsigned Size() const { return size; }
    unsigned DSize() const { return unsigned(data.size()); }
	const TData& GetData() const { return data; }

	VectorBool(unsigned _size) : size(_size), data((size + bits_per_value - 1) / bits_per_value) {}
	VectorBool(unsigned _size, const TValue& v) : VectorBool(_size) { Fill(v); }
	TSelf& operator=(const TValue& v) { Fill(v); return *this; }
	void swap(TSelf& r) { data.swap(r.data); }

	TValue Get(unsigned i) const { TValue t; t.SetU(data[i / bits_per_value] >> (i % bits_per_value)); return t; }
    void Set(unsigned i, TValue v) { TDataValue& r = data[i / bits_per_value]; unsigned shift = i % bits_per_value; r = (v.Get() ? (r | (1ull << shift)) : (r & (~(1ull << shift)))); }
	TValue operator()(unsigned i) const { return Get(i); }

	TSelf& operator+=(const TValue& v) { if (v.Get()) Complement(); return *this; }
	TSelf& operator-=(const TValue& v) { if (v.Get()) Complement(); return *this; }
	TSelf& operator*=(const TValue& v) { if (!v.Get()) Clear(); return *this; }
	TSelf& operator/=(const TValue& v) { assert(v.Get()); return *this; }
	TSelf operator+(const TValue& v) const { if (v.Get()) { TSelf t(*this); t.Complement(); return t; } return *this; }
	TSelf operator-(const TValue& v) const { if (v.Get()) { TSelf t(*this); t.Complement(); return t; } return *this; }
	TSelf operator*(const TValue& v) const { return (v.Get() ? *this : TSelf(size)); }
	TSelf operator/(const TValue& v) const { assert(v.Get()); return *this; }
	TSelf operator-() const { return *this; }

	TSelf& operator+=(const TSelf& v) { assert(Size() == v.Size()); const_diterator pv = v.DBegin(); for (diterator p = DBegin(), pend = DEnd(); p < pend; ) *p++ ^= *pv++; return *this; }
	TSelf& operator-=(const TSelf& v) { assert(Size() == v.Size()); const_diterator pv = v.DBegin(); for (diterator p = DBegin(), pend = DEnd(); p < pend; ) *p++ ^= *pv++; return *this; }
	TSelf operator+(const TSelf& v) const { TSelf t(*this); t += v; return t; }
	TSelf operator-(const TSelf& v) const { TSelf t(*this); t -= v; return t; }
};
