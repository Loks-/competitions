#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValue, class TTerm = Term<TValue>>
class Function {
 public:
  std::vector<TTerm> terms;

  Function() {}
  Function(const TValue& x) { terms.emplace_back(TTerm(x)); }
  Function(const TTerm& t) { terms.emplace_back(t); }

  void AddTerm(const TTerm& t) { terms.emplace_back(t); }

  TValue Get(const TValue& x) const {
    TValue s(0);
    for (const auto& t : terms) s += t.Get(x);
    return s;
  }

  size_t Size() const { return terms.size(); }
  TTerm& operator()(size_t index) { return terms[index]; }
  const TTerm& operator()(size_t index) const { return terms[index]; }

  bool Sorted() const {
    for (size_t i = 1; i < terms.size(); ++i) {
      if (terms[i].tp < terms[i - 1].tp) return false;
    }
    return true;
  }

  bool StrictlySorted() const {
    for (size_t i = 1; i < terms.size(); ++i) {
      if (!(terms[i - 1].tp < terms[i].tp)) return false;
    }
    return true;
  }

  void SortTerms() {
    if (!Sorted()) {
      std::sort(terms.begin(), terms.end(),
                [](auto& x, auto& y) { return x.tp < y.tp; });
    }
  }

  void CompressSorted() {
    if (StrictlySorted()) return;
    std::vector<TTerm> vt;
    TValue zero = TValue(0);
    for (unsigned i = 0; i < terms.size(); ++i) {
      if (terms[i].a == zero) continue;
      for (unsigned j = i + 1; j < terms.size(); ++j) {
        if (terms[i].tp < terms[j].tp) break;
        if (terms[i].tp == terms[j].tp) {
          terms[i].a += terms[j].a;
          terms[j].a = zero;
        }
      }
      if (terms[i].a != zero) vt.emplace_back(terms[i]);
    }
    terms.swap(vt);
  }

  void Compress() {
    SortTerms();
    CompressSorted();
  }
};
}  // namespace ext_polynomial
}  // namespace calculus
