#include "common/vector/utils.h"
#include "common/stl_base.h"

#include <algorithm>

int main_ant_stack()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned N;
		cin >> N;
        vector<uint64_t> weights = ReadVector<uint64_t>(N);
        uint64_t max_weight = 10 * uint64_t(1000000000);
        vector<uint64_t> best_weights(N+1, max_weight);
        best_weights[0] = 0;
        for (uint64_t w : weights)
        {
            unsigned j0 = upper_bound(best_weights.begin(), best_weights.end(), w) - best_weights.begin();
            unsigned j1 = upper_bound(best_weights.begin(), best_weights.end(), 6 * w) - best_weights.begin();
            for (unsigned j = j1; j >= j0; --j)
            {
                best_weights[j] = min(best_weights[j], best_weights[j-1] + w);
            }
        }

		cout << "Case #" << it << ": " << (lower_bound(best_weights.begin(), best_weights.end(), max_weight) - best_weights.begin()) - 1 << endl;
	}
	return 0;
}
