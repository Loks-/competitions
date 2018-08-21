// https://www.hackerrank.com/challenges/minimum-average-waiting-time

#include "common/stl_base.h"

#include <functional>
#include <queue>

int main_minimum_average_waiting_time()
{
    uint64_t N, a, l, sum_a = 0, sum_f = 0, time = 0;
    priority_queue<uint64_t, vector<uint64_t>, greater<uint64_t>> q_current_orders;
    priority_queue<pair<uint64_t, uint64_t>, vector<pair<uint64_t, uint64_t>>, greater<pair<uint64_t, uint64_t>>> q_all_orders;
    cin >> N;
    for (unsigned i = 0; i < N; ++i)
    {
        cin >> a >> l;
        sum_a += a;
        q_all_orders.push({a, l});
	}
    for (; !q_all_orders.empty() || !q_current_orders.empty(); q_current_orders.pop())
    {
        if (q_current_orders.empty())
            time = max(time, q_all_orders.top().first);
        for (; !q_all_orders.empty() && (q_all_orders.top().first <= time); q_all_orders.pop())
            q_current_orders.push(q_all_orders.top().second);
        time += q_current_orders.top();
        sum_f += time;
    }
    cout << (sum_f - sum_a) / N << endl;
	return 0;
}
