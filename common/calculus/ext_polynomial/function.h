#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm = TValueF,
          class TTerm = Term<TValueF, TValueTerm>>
class Function {
 public:
  using TSelf = Function<TValueF, TValueTerm, TTerm>;

  std::vector<TTerm> terms;

  Function() {}
  Function(const TValueF& x) {
    if (x != TValueF(0)) terms.emplace_back(TTerm(x));
  }
  Function(const TTerm& t) {
    if (t.a != TValueF(0)) terms.emplace_back(t);
  }

  bool Empty() const { return terms.empty(); }
  size_t Size() const { return terms.size(); }
  TTerm& operator()(size_t index) { return terms[index]; }
  const TTerm& operator()(size_t index) const { return terms[index]; }
  void swap(TSelf& r) { terms.swap(r.terms); }

  bool IsConstant() const {
    for (auto& t : terms) {
      if (!t.IsConstant()) return false;
    }
    return true;
  }

  bool IsPolynomial() const {
    for (auto& t : terms) {
      if (!t.IsPolynomial()) return false;
    }
    return true;
  }

  // After these functions terms became unsorted
  void AddTermUnsafe(const TTerm& t) { terms.emplace_back(t); }
  void AddTermsUnsafe(const TSelf& f) {
    for (auto& t : f.terms) AddTermUnsafe(t);
  }

  bool AreTermsSorted() const {
    for (size_t i = 1; i < terms.size(); ++i) {
      if (terms[i].tp < terms[i - 1].tp) return false;
    }
    return true;
  }

  bool AreTermsCompressed() const {
    for (size_t i = 1; i < terms.size(); ++i) {
      if (!(terms[i - 1].tp < terms[i].tp)) return false;
    }
    return true;
  }

  void SortTerms() {
    if (!AreTermsSorted()) {
      std::sort(terms.begin(), terms.end(),
                [](auto& x, auto& y) { return x.tp < y.tp; });
    }
  }

  void Compress() {
    if (!AreTermsCompressed()) {
      SortTerms();
      TSelf f;
      for (size_t i = 0, j = 0; i < terms.size(); i = j) {
        for (j = i + 1; (j < terms.size()) && (terms[i].tp == terms[j].tp); ++j)
          terms[i].a += terms[j].a;
        if (terms[i].a != TValueF(0)) f.AddTermUnsafe(terms[i]);
      }
      swap(f);
    }
  }

  TValueF Get(const TValueTerm& x) const {
    TValueF s(0);
    for (const auto& t : terms) s += t.Get(x);
    return s;
  }

  bool operator==(const TSelf& r) const { return terms == r.terms; }
  bool operator!=(const TSelf& r) const { return !(terms == r.terms); }

  TSelf operator-() const {
    TSelf f;
    for (auto& t : terms) f.AddTermUnsafe(-t);
    return f;
  }

  TSelf operator+(const TSelf& r) const {
    TSelf f;
    size_t i = 0, j = 0;
    for (; (i < Size()) && (j < r.Size());) {
      auto& t1 = terms[i];
      auto& t2 = r(j);
      if (t1.tp < t2.tp) {
        f.AddTermUnsafe(t1);
        ++i;
      } else if (t2.tp < t1.tp) {
        f.AddTermUnsafe(t2);
        ++j;
      } else if (t1.tp == t2.tp) {
        TValueF a = t1.a + t2.a;
        if (a != TValueF(0)) f.AddTermUnsafe(TTerm(a, t1.tp));
        ++i;
        ++j;
      } else {
        assert(false);
      }
    }
    for (; i < Size(); ++i) f.AddTermUnsafe(terms[i]);
    for (; j < r.Size(); ++j) f.AddTermUnsafe(r(j));
    return f;
  }

  TSelf operator+(const TValueF& r) const { return (*this) + TSelf(r); }
  TSelf operator+(const TTerm& r) const { return (*this) + TSelf(r); }

  TSelf& operator+=(const TValueF& r) { return *this = (*this + r); }
  TSelf& operator+=(const TTerm& r) { return *this = (*this + r); }
  TSelf& operator+=(const TSelf& r) { return *this = (*this + r); }

  TSelf operator-(const TSelf& r) const {
    TSelf f;
    size_t i = 0, j = 0;
    for (; (i < Size()) && (j < r.Size());) {
      auto& t1 = terms[i];
      auto& t2 = r(j);
      if (t1.tp < t2.tp) {
        f.AddTermUnsafe(t1);
        ++i;
      } else if (t2.tp < t1.tp) {
        f.AddTermUnsafe(-t2);
        ++j;
      } else if (t1.tp == t2.tp) {
        TValueF a = t1.a - t2.a;
        if (a != TValueF(0)) f.AddTermUnsafe(TTerm(a, t1.tp));
        ++i;
        ++j;
      } else {
        assert(false);
      }
    }
    for (; i < Size(); ++i) f.AddTermUnsafe(terms[i]);
    for (; j < r.Size(); ++j) f.AddTermUnsafe(-r(j));
    return f;
  }

  TSelf operator-(const TValueF& r) const { return (*this) - TSelf(r); }
  TSelf operator-(const TTerm& r) const { return (*this) - TSelf(r); }

  TSelf& operator-=(const TValueF& r) { return *this = (*this - r); }
  TSelf& operator-=(const TTerm& r) { return *this = (*this - r); }
  TSelf& operator-=(const TSelf& r) { return *this = (*this - r); }

  TSelf& operator*=(const TValueF& r) {
    for (auto& t : terms) t *= r;
    return *this;
  }

  TSelf operator*(const TValueF& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  TSelf& operator/=(const TValueF& r) {
    for (auto& t : terms) t /= r;
    return *this;
  }

  TSelf operator/(const TValueF& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }
};

template <class TValueF, class TValueTerm, class TTerm>
inline Function<TValueF, TValueTerm, TTerm> operator+(
    const TValueF& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return r + l;
}

template <class TValueF, class TValueTerm, class TTerm>
inline Function<TValueF, TValueTerm, TTerm> operator+(
    const TTerm& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return r + l;
}

template <class TValueF, class TValueTerm, class TTerm>
inline Function<TValueF, TValueTerm, TTerm> operator-(
    const TValueF& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return Function<TValueF, TValueTerm, TTerm>(l) - r;
}

template <class TValueF, class TValueTerm, class TTerm>
inline Function<TValueF, TValueTerm, TTerm> operator-(
    const TTerm& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return Function<TValueF, TValueTerm, TTerm>(l) - r;
}

template <class TValueF, class TValueTerm, class TTerm>
inline Function<TValueF, TValueTerm, TTerm> operator*(
    const TValueF& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return r * l;
}

using DFunction = Function<double, double, DTerm>;
}  // namespace ext_polynomial
}  // namespace calculus
