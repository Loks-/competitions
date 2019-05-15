// https://www.hackerrank.com/challenges/minimum-average-waiting-time

#include "common/heap.h"
#include "common/stl/base.h"

int main_minimum_average_waiting_time() {
  uint64_t N, a, l, sum_a = 0, sum_f = 0, time = 0;
  HeapMinOnTop<uint64_t> q_current_orders;
  HeapMinOnTop<pair<uint64_t, uint64_t>> q_all_orders;
  cin >> N;
  for (unsigned i = 0; i < N; ++i) {
    cin >> a >> l;
    sum_a += a;
    q_all_orders.Add({a, l});
  }
  for (; !q_all_orders.Empty() || !q_current_orders.Empty();) {
    if (q_current_orders.Empty()) time = max(time, q_all_orders.Top().first);
    for (; !q_all_orders.Empty() && (q_all_orders.Top().first <= time);
         q_all_orders.Pop())
      q_current_orders.Add(q_all_orders.Top().second);
    time += q_current_orders.Extract();
    sum_f += time;
  }
  cout << (sum_f - sum_a) / N << endl;
  return 0;
}
