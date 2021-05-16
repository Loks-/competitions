#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/enumerate.h"
#include "common/vector/read.h"
#include "common/vector/rmq.h"

#include <stack>

int main_hidden_pancakes__rmq() {
  class Task {
   public:
    size_t b, e, m;
  };

  unsigned T, N;
  modular::mstatic::Factorial<TModularD> f;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    std::vector<uint64_t> v = nvector::Read<uint64_t>(N), va(N);
    for (unsigned i = 0; i < N; ++i) va[i] = (v[i] + 1) * N - (i + 1);
    nvector::RMQ<uint64_t> rmq(va);
    TModularD r = 1;
    stack<Task> tasks;
    for (tasks.push({0, N, 1}); !tasks.empty();) {
      auto t = tasks.top();
      tasks.pop();
      auto x = rmq.Minimum(t.b, t.e);
      if (x.value / N != t.m) {
        r = 0;
        break;
      }
      r *= f.BinomialCoefficient(t.e - t.b - 1, x.pos - t.b);
      if (x.pos > t.b) tasks.push({t.b, x.pos, t.m});
      if (x.pos + 1 < t.e) tasks.push({x.pos + 1, t.e, t.m + 1});
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
