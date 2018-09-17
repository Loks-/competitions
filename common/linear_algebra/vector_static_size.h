#pragma once

#include <array>

template<class TTValue, unsigned _size>
class VectorStaticSize
{
public:
	const static unsigned size = _size;

	using TValue = TTValue;
	using TData = std::array<TValue, size>;
	using TSelf = VectorStaticSize<TValue, size>;

	using iterator = TValue*;
	using const_iterator = const TValue*;

public:
	TData data;

	void Fill(const TValue& v) { data.fill(v); }
	void Clear() { Fill(TValue(0)); }
	unsigned Size() const { return size; }

	VectorStaticSize() { Clear(); }
	VectorStaticSize(const TValue& v) { Fill(v); }
	TSelf& operator=(const TValue& v) { Fill(v); return *this; }

	TValue& operator()(unsigned i) { return data[i]; }
	const TValue& operator()(unsigned i) const { return data[i]; }
	iterator begin() { return &data.front(); }
	const_iterator begin() const { return &data.front(); }
	iterator end() { return begin() + size; }
	const_iterator end() const { return begin() + size; }
	void swap(TSelf& r) { data.swap(r.data); }

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
};

template<unsigned size>
using TDVectorStaticSize = VectorStaticSize<double, size>;
