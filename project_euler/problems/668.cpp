#include "common/factorization/primality_test.h"
#include "common/stl/base.h"
#include "common/thread_pool.h"

#include <numeric>

int main_668() {
  uint64_t n = 10000000000;
  const unsigned threads = 8;
  vector<uint64_t> vs(threads);
  {
    ThreadPool tp(threads);
    for (unsigned it = 0; it < threads; ++it) {
      auto task = std::make_shared<std::packaged_task<void()>>([&, it]() {
        uint64_t begin = ((it * n) / threads) | 1;
        uint64_t end = ((it + 1) * n) / threads;
        for (uint64_t i = begin; i < end; i += 2) {
          if (IsPrime(i)) {
            uint64_t ndi = n / i;
            vs[it] += std::min(ndi, i);
          }
        }
      });
      tp.enqueueTask(std::move(task));
    }
  }
  uint64_t s = 2;
  s = accumulate(vs.begin(), vs.end(), s);
  std::cout << n - s << std::endl;
  return 0;
}
