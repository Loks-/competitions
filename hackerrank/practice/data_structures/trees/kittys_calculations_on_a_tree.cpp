// https://www.hackerrank.com/challenges/kittys-calculations-on-a-tree

#include "common/graph/centroid_decomposition.h"
#include "common/graph/lca.h"
#include "common/numeric/modular.h"
#include "common/stl_base.h"

#include <unordered_map>

using TModular = Modular<>;

namespace
{
	class SolutionKittysCalculationOnATree : public ICentroidDecompositionCallBack
	{
	protected:
		BaseTree tree;
		LCA lca;
		vector<TModular> results;
		vector<unordered_map<unsigned, vector<unsigned>>> sets;

	public:
		void Solve()
		{
			unsigned n, q, k, v;
			cin >> n >> q;
			tree.Resize(n);
			tree.ReadEdges();
			lca.Build(tree);
			sets.resize(n);
			for (unsigned iq = 0; iq < q; ++iq)
			{
				cin >> k;
				for (unsigned ik = 0; ik < k; ++ik)
				{
					cin >> v;
					sets[0][iq].push_back(v - 1);
				}
			}
			results.clear();
			results.resize(q);
			CentroidDecomposition::Run(tree, *this);
			for (TModular r : results)
				cout << r.Get() << endl;
		}

		void Decompose(unsigned vertex, const vector<unsigned>& group)
		{
			unsigned current_group = group[vertex];
			for (const auto& it : sets[current_group])
			{
				const auto& v = it.second;
				if (v.size() <= 1) continue;
				unsigned q = it.first;
				TModular s, sd;
				unordered_map<unsigned, pair<TModular, TModular>> m;
				for (unsigned u : v)
				{
					TModular d(lca.GetDistance(u, vertex));
					TModular w(u + 1), wd = w * d;
					s += w; sd += wd;
					unsigned g = group[u];
					m[g].first += w;
					m[g].second += wd;
					if (u != vertex)
						sets[g][q].push_back(u);
				}
				TModular t = s * sd;
				for (auto& p : m)
					t -= p.second.first * p.second.second;
				results[q] += t;
			}
		}
	};
}

int main_kittys_calculations_on_a_tree()
{
	SolutionKittysCalculationOnATree s;
	s.Solve();
	return 0;
}
