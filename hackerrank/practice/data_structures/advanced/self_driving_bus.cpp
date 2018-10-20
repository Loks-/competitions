// https://www.hackerrank.com/challenges/self-driving-bus

#include "common/graph/tree/base_tree.h"
#include "common/graph/tree/centroid_decomposition.h"
#include "common/stl/base.h"

#include <stack>

namespace
{
	class SolutionSelfDrivingBus : public ICentroidDecompositionCallBack
	{
	protected:
		unsigned n;
		BaseTree tree;
		uint64_t total_segments;
		vector<unsigned> path_min, path_max;

	public:
		void Solve()
		{
			cin >> n;
			tree.Resize(n);
			tree.ReadEdges();
			total_segments = 0;
			path_min.resize(n);
			path_max.resize(n);
			CentroidDecomposition::Run(tree, *this);
			cout << total_segments << endl;
		}

		void UpdatePaths(unsigned vertex, const BaseTree& current_tree)
		{
			path_min[vertex] = path_max[vertex] = vertex;
			static stack<pair<unsigned, unsigned>> s;
			for (s.push(make_pair(vertex, vertex)); !s.empty(); )
			{
				unsigned current = s.top().first, from = s.top().second;
				s.pop();
				for (unsigned u : current_tree.edges[current])
				{
					if (u == from)
						continue;
					path_min[u] = min(u, path_min[current]);
					path_max[u] = max(u, path_max[current]);
					s.push(make_pair(u, current));
				}
			}
		}

		void PreDecompose(unsigned vertex, const CentroidDecomposition& cd)
		{
			UpdatePaths(vertex, cd.GetCurrentTree());

			const vector<unsigned>& group_map = cd.GetGroupMap();
			unsigned current_group = group_map[vertex];
			static vector<pair<unsigned, unsigned>> candidates_l, candidates_r;
			candidates_l.push_back(make_pair(vertex, vertex));
			candidates_r.push_back(make_pair(vertex, vertex));
			{
				// Go left
				unsigned req_min = vertex;
				unsigned req_max = vertex;
				for (unsigned j = vertex; j--; )
				{
					if (group_map[j] != current_group)
						break;
					req_min = min(req_min, path_min[j]);
					req_max = max(req_max, path_max[j]);
					if (req_min == j)
						candidates_l.push_back(make_pair(req_min, req_max));
				}
			}
			{
				// Go Right
				unsigned req_min = vertex;
				unsigned req_max = vertex;
				for (unsigned j = vertex + 1; j < n; ++j)
				{
					if (group_map[j] != current_group)
						break;
					req_min = min(req_min, path_min[j]);
					req_max = max(req_max, path_max[j]);
					if (req_max == j)
						candidates_r.push_back(make_pair(req_min, req_max));
				}
			}

			static vector<unsigned> cr_min, cr_max;
			cr_min.resize(candidates_r.size());
			cr_max.resize(candidates_r.size());
			for (size_t i = 0; i < candidates_r.size(); ++i)
			{
				cr_min[i] = n-candidates_r[i].first;
				cr_max[i] = candidates_r[i].second;
			}
			for (size_t i = 0; i < candidates_l.size(); ++i)
			{
				size_t mink = lower_bound(cr_max.begin(), cr_max.end(), candidates_l[i].second) - cr_max.begin();
				size_t maxk = upper_bound(cr_min.begin(), cr_min.end(), n-candidates_l[i].first) - cr_min.begin();
				if (maxk > mink)
					total_segments += maxk - mink;
			}
			candidates_l.clear(); candidates_r.clear();
		}
	};
}

int main_self_driving_bus()
{
	SolutionSelfDrivingBus s;
	s.Solve();
	return 0;
}
