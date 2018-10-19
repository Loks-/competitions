// https://www.hackerrank.com/challenges/matrix-tree

#include "common/graph/tree/base_tree.h"
#include "common/modular/modular.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

#include <functional>

using TModular = Modular<>;

int main_matrix_tree()
{
	unsigned N;
    cin >> N;
    vector<unsigned> vw = ReadVector<unsigned>(N);
    BaseTree tree(N);
    tree.ReadEdges();

    std::function<TModular(unsigned, unsigned, unsigned)> SolveR = [&](unsigned node, unsigned p, unsigned pw) -> TModular
    {
        TModular r = TModular(vw[node]) - TModular(pw);
        for (unsigned c : tree.edges[node])
        {
            if (c == p) continue;
            r *= SolveR(c, node, vw[node]);
        }
        return r;
    };

    cout << SolveR(0, 0, 0).Get() << endl;
	return 0;
}
