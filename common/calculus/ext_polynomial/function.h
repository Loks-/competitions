#pragma once

#include "common/base.h"
#include "common/calculus/ext_polynomial/term.h"
#include "common/vector/sorted.h"

#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValueF, class TValueTerm = TValueF,
          class TTerm = Term<TValueF, TValueTerm>>
class Function {
 public:
  using TSelf = Function<TValueF, TValueTerm, TTerm>;

 public:
  std::vector<TTerm> terms;

 public:
  constexpr Function() {}

  constexpr Function(const TValueF& x) {
    if (x != TValueF(0)) terms.emplace_back(TTerm(x));
  }

  constexpr Function(const TTerm& t) {
    if (t.a != TValueF(0)) terms.emplace_back(t);
  }

  constexpr bool Empty() const { return terms.empty(); }

  constexpr size_t Size() const { return terms.size(); }

  constexpr TTerm& operator()(size_t index) { return terms[index]; }

  constexpr const TTerm& operator()(size_t index) const { return terms[index]; }

  constexpr void swap(TSelf& r) { terms.swap(r.terms); }

  constexpr bool IsConstant() const {
    for (auto& t : terms) {
      if (!t.IsConstant()) return false;
    }
    return true;
  }

  constexpr bool IsPolynomial() const {
    for (auto& t : terms) {
      if (!t.IsPolynomial()) return false;
    }
    return true;
  }

  constexpr TValueF ToValue() const {
    assert(IsConstant());
    return Empty() ? TValueF(0) : terms[0].a;
  }

  // After these functions terms became unsorted
  constexpr void AddTermUnsafe(const TTerm& t) { terms.emplace_back(t); }

  constexpr void AddTermsUnsafe(const TSelf& f) {
    for (auto& t : f.terms) AddTermUnsafe(t);
  }

  constexpr bool AreTermsSorted() const {
    return nvector::IsWeakSorted(
        terms, [](const TTerm& x, const TTerm& y) { return x.tp < y.tp; });
  }

  constexpr bool AreTermsCompressed() const {
    return nvector::IsStrongSorted(
        terms, [](const TTerm& x, const TTerm& y) { return x.tp < y.tp; });
  }

  constexpr void SortTerms() {
    if (!AreTermsSorted()) {
      std::sort(terms.begin(), terms.end(),
                [](auto& x, auto& y) { return x.tp < y.tp; });
    }
  }

  constexpr void Compress() {
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

  constexpr TValueF Get(const TValueTerm& x) const {
    TValueF s(0);
    for (const auto& t : terms) s += t.Get(x);
    return s;
  }

  constexpr bool operator==(const TSelf& r) const { return terms == r.terms; }

  constexpr bool operator!=(const TSelf& r) const {
    return !(terms == r.terms);
  }

  constexpr TSelf operator-() const {
    TSelf f;
    for (auto& t : terms) f.AddTermUnsafe(-t);
    return f;
  }

  constexpr TSelf operator+(const TSelf& r) const {
    TSelf f;
    size_t i = 0, j = 0;
    for (; (i < Size()) && (j < r.Size());) {
      const auto& t1 = terms[i];
      const auto& t2 = r(j);
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

  constexpr TSelf operator+(const TValueF& r) const {
    return (*this) + TSelf(r);
  }

  constexpr TSelf operator+(const TTerm& r) const { return (*this) + TSelf(r); }

  constexpr TSelf& operator+=(const TValueF& r) { return *this = (*this + r); }

  constexpr TSelf& operator+=(const TTerm& r) { return *this = (*this + r); }

  constexpr TSelf& operator+=(const TSelf& r) { return *this = (*this + r); }

  constexpr TSelf operator-(const TSelf& r) const {
    TSelf f;
    size_t i = 0, j = 0;
    for (; (i < Size()) && (j < r.Size());) {
      const auto& t1 = terms[i];
      const auto& t2 = r(j);
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

  constexpr TSelf operator-(const TValueF& r) const {
    return (*this) - TSelf(r);
  }

  constexpr TSelf operator-(const TTerm& r) const { return (*this) - TSelf(r); }

  constexpr TSelf& operator-=(const TValueF& r) { return *this = (*this - r); }

  constexpr TSelf& operator-=(const TTerm& r) { return *this = (*this - r); }

  constexpr TSelf& operator-=(const TSelf& r) { return *this = (*this - r); }

  constexpr TSelf& operator*=(const TValueF& r) {
    for (auto& t : terms) t *= r;
    return *this;
  }

  constexpr TSelf operator*(const TValueF& r) const {
    TSelf t(*this);
    t *= r;
    return t;
  }

  constexpr TSelf& operator/=(const TValueF& r) {
    for (auto& t : terms) t /= r;
    return *this;
  }

  constexpr TSelf operator/(const TValueF& r) const {
    TSelf t(*this);
    t /= r;
    return t;
  }
};

template <class TValueF, class TValueTerm, class TTerm>
constexpr Function<TValueF, TValueTerm, TTerm> operator+(
    const TValueF& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return r + l;
}

template <class TValueF, class TValueTerm, class TTerm>
constexpr Function<TValueF, TValueTerm, TTerm> operator+(
    const TTerm& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return r + l;
}

template <class TValueF, class TValueTerm, class TTerm>
constexpr Function<TValueF, TValueTerm, TTerm> operator-(
    const TValueF& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return Function<TValueF, TValueTerm, TTerm>(l) - r;
}

template <class TValueF, class TValueTerm, class TTerm>
constexpr Function<TValueF, TValueTerm, TTerm> operator-(
    const TTerm& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return Function<TValueF, TValueTerm, TTerm>(l) - r;
}

template <class TValueF, class TValueTerm, class TTerm>
constexpr Function<TValueF, TValueTerm, TTerm> operator*(
    const TValueF& l, const Function<TValueF, TValueTerm, TTerm>& r) {
  return r * l;
}

using DFunction = Function<double, double, DTerm>;
}  // namespace ext_polynomial
}  // namespace calculus
