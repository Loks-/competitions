#pragma once

template<class TValue = double>
class StatM1
{
public:
    using TSelf = StatM1<TValue>;

protected:
    TValue s0, s1;

public:
    StatM1() { Clear(); }

    void Clear() { s0 = s1 = TValue(0); }
    void AddSample(const TValue& x, const TValue& w = 1) { s0 += w; s1 += x * w; }

    void AddValue(const TValue& x) { s1 += x * s0; }
    void MultWeight(const TValue& x) { s0 *= x; s1 *= x; }
    void AddStat(const TSelf& r) { s0 += r.s0; s1 += r.s1; }

    TSelf& operator+=(const TValue& x) { AddValue(x); return *this; }
    TSelf& operator-=(const TValue& x) { AddValue(-x); return *this; }
    TSelf& operator*=(const TValue& x) { MultWeight(x); return *this; }
    TSelf& operator+=(const TSelf& r) { AddStat(r); return *this; }

    TSelf operator+(const TValue& x) { TSelf t(*this); t += x; return t; }
    TSelf operator-(const TValue& x) { TSelf t(*this); t -= x; return t; }
    TSelf operator*(const TValue& x) { TSelf t(*this); t *= x; return t; }
    TSelf operator+(const TSelf& r) { TSelf t(*this); t += r; return t; }

    TValue RawMoment1() const { return s1 / s0; }
    TValue Mean() const { return RawMoment1(); }
};
