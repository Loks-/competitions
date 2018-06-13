#include "common/base.h"
#include "common/hash.h"

// O(S^2) solution for Mysterious Road Signs.
// It passes all tests.

namespace {
	struct BestIntervals
	{
		unsigned max_length;
		unordered_set<pair<unsigned, unsigned>> intervals;

		BestIntervals() : max_length(0) {}

		void AddInterval(unsigned first, unsigned last)
		{
			unsigned length = last - first;
			if (max_length < length)
			{
				max_length = length;
				intervals.clear();
			}
			if (length == max_length)
				intervals.insert(make_pair(first, last));
		}
	};
}

int main_mysterious_road_signs_slow()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned S;
		cin >> S;
		array<vector<int64_t>, 2> targets;
		for (unsigned i = 0; i < S; ++i)
		{
			int64_t d, a, b;
			cin >> d >> a >> b;
			targets[0].push_back(d + a);
			targets[1].push_back(d - b);
		}
		array<map<int64_t, vector<pair<unsigned, unsigned>>>, 2> point2intervals;
		for (unsigned j = 0; j < 2; ++j)
		{
			unsigned start_index = 0;
			int64_t last_value = targets[j][0];
			for (unsigned i = 1; i < S; ++i)
			{
				if (targets[j][i] != last_value)
				{
					point2intervals[j][last_value].push_back(make_pair(start_index, i));
					start_index = i;
					last_value = targets[j][i];
				}
			}
			point2intervals[j][last_value].push_back(make_pair(start_index, S));
			for (auto& it : point2intervals[j])
				it.second.push_back(make_pair(S, S));
		}
		BestIntervals best_intervals;
		for (const auto it0 : point2intervals[0])
		{
			const auto& interval_list0 = it0.second;
			unordered_set<int64_t> candidates;
			for (const auto& interval : interval_list0)
			{
				if ((interval.first > 0) && (interval.first < S))
					candidates.insert(targets[1][interval.first - 1]);
				if (interval.second < S)
					candidates.insert(targets[1][interval.second]);
			}
			if (candidates.size() == 0)
			{
				best_intervals.AddInterval(0, S);
				break;
			}
			for (int64_t candidate : candidates)
			{
				auto it1 = point2intervals[1].find(candidate);
				assert(it1 != point2intervals[1].end());
				const auto& interval_list1 = it1->second;
				unsigned first_index = 0, last_index = 0, i0 = 0, i1 = 0;
				for (; (i0 < interval_list0.size()) && (i1 < interval_list1.size());)
				{
					auto current = (interval_list0[i0].first <= interval_list1[i1].first) ? interval_list0[i0++] : interval_list1[i1++];
					if (last_index < current.first)
					{
						best_intervals.AddInterval(first_index, last_index);
						first_index = current.first;
					}
					last_index = max(last_index, current.second);
				}
				best_intervals.AddInterval(first_index, last_index);
			}
		}
		cout << "Case #" << it << ": " << best_intervals.max_length << " " << best_intervals.intervals.size() << endl;
	}
	return 0;
}
