// https://www.hackerrank.com/challenges/matrix-tree

#include "common/graph/tree/base_tree.h"
#include "common/modular/modular_io.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

#include <functional>

using TModular = Modular<>;

int main_matrix_tree()
{
	unsigned N;
    cin >> N;
    vector<TModular> vw = ReadVector<TModular>(N);
    BaseTree tree(N);
    tree.ReadEdges();

    std::function<TModular(unsigned, unsigned, TModular)> SolveR = [&](unsigned node, unsigned p, TModular pw) -> TModular
    {
        TModular r = vw[node] - pw;
        for (unsigned c : tree.edges[node])
        {
            if (c == p) continue;
            r *= SolveR(c, node, vw[node]);
        }
        return r;
    };

    cout << SolveR(0, 0, 0) << endl;
	return 0;
}
