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
		BaseTree tree;
		uint64_t total_segments;
		vector<unsigned> path_min, path_max;

	public:
		void Solve()
		{
			unsigned n;
			cin >> n;
			tree.Resize(n);
			tree.ReadEdges();
			total_segments = 0;
			path_min.resize(n);
			path_max.resize(n);
			CentroidDecomposition::Run(tree, *this);
			cout << total_segments << endl;
		}

		void UpdatePaths(unsigned vertex, const vector<unsigned>& group)
		{
			path_min[vertex] = path_max[vertex] = vertex;
			unsigned min_group = group[vertex];
			stack<pair<unsigned, unsigned>> s;
			for (s.push(make_pair(vertex, vertex)); !s.empty(); )
			{
				unsigned current = s.top().first, from = s.top().second;
				s.pop();
				for (unsigned u : tree.edges[current])
				{
					if ((group[u] < min_group) || (u == from))
						continue;
					path_min[u] = min(u, path_min[current]);
					path_max[u] = max(u, path_max[current]);
					s.push(make_pair(u, current));
				}
			}
		}

		void Decompose(unsigned vertex, const vector<unsigned>& group)
		{
			UpdatePaths(vertex, group);

			unsigned min_group = group[vertex];
			vector<pair<unsigned, unsigned>> candidates_l, candidates_r;
			candidates_l.push_back(make_pair(vertex, vertex));
			candidates_r.push_back(make_pair(vertex, vertex));
			{
				// Go left
				unsigned req_min = vertex;
				unsigned req_max = vertex;
				for (unsigned j = vertex; j--; )
				{
					if (group[j] < min_group)
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
				for (int j = vertex + 1; j < group.size(); ++j)
				{
					if (group[j] < min_group)
						break;
					req_min = min(req_min, path_min[j]);
					req_max = max(req_max, path_max[j]);
					if (req_max == j)
						candidates_r.push_back(make_pair(req_min, req_max));
				}
			}


			for (size_t i = 0; i < candidates_l.size(); ++i)
			{
				for (size_t j = 0; j < candidates_r.size(); ++j)
				{
					if ((candidates_l[i].first <= candidates_r[j].first) && (candidates_l[i].second <= candidates_r[j].second))
						++total_segments;
				}
			}
		}
	};
}

int main_self_driving_bus()
{
	SolutionSelfDrivingBus s;
	s.Solve();
	return 0;
}
