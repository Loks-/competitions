#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class Function {
 public:
  using TTerm = Term<TValue>;

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

  bool SortedTerms() const {
    for (size_t i = 1; i < terms.size(); ++i) {
      if (terms[i].tp < terms[i - 1].tp) return false;
    }
    return true;
  }

  void SortTerms() {
    if (!SortedTerms()) {
      std::sort(terms.begin(), terms.end(),
                [](auto& x, auto& y) { return x.tp < y.tp; });
    }
  }

  void CompressSorted() {
    // ...
  }

  void Compress() {
    SortTerms();
    CompressSorted();
  }
};
}  // namespace ext_polynomial
}  // namespace calculus
