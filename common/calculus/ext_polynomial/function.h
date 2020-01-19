#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValue, class TTerm = Term<TValue>>
class Function {
 public:
  using TSelf = Function<TValue, TTerm>;

  std::vector<TTerm> terms;

  Function() {}
  Function(const TValue& x) { terms.emplace_back(TTerm(x)); }
  Function(const TTerm& t) { terms.emplace_back(t); }

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

  // After this function terms became unsorted
  void AddTermUnsafe(const TTerm& t) { terms.emplace_back(t); }

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
        if (terms[i].a != TValue(0)) f.AddTermUnsafe(terms[i]);
      }
      swap(f);
    }
  }

  TValue Get(const TValue& x) const {
    TValue s(0);
    for (const auto& t : terms) s += t.Get(x);
    return s;
  }
};
}  // namespace ext_polynomial
}  // namespace calculus
