#pragma once

#include "common/calculus/ext_polynomial/term.h"
#include "common/calculus/ext_polynomial/term_bases/one.h"
#include <algorithm>
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class Function {
 public:
  using TTerm = Term<TValue>;

  std::vector<TTerm> v;

  Function() {}
  Function(const TValue& x) {
    v.emplace_back(TTerm(v, term_bases::CreateOne<TValue>(0)));
  }
  Function(const TTerm& t) { v.emplace_back(t); }

  void AddTerm(const TTerm& t) { v.emplace_back(t); }

  TValue Get(const TValue& x) const {
    TValue s(0);
    for (const auto& t : v) s += t.Get(x);
    return s;
  }

  bool SortedTerms() const {
    for (size_t i = 1; i < v.size(); ++i) {
      if (v[i] < v[i - 1]) return false;
    }
    return true;
  }

  void SortTerms() {
    if (!SortedTerms()) {
      std::sort(v.begin(), v.end(),
                [](auto& x, auto& y) { return x->b < y->b; });
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
