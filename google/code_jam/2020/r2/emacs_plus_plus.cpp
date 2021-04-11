#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/graph/graph_ei.h"

#include <string>
#include <stack>

int main_emacs_plus_plus() {
  unsigned T, K, Q;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> K >> Q;
    string s;
    cin >> s;
    auto vl = nvector::Read<uint64_t>(K), vr = nvector::Read<uint64_t>(K), vp = nvector::Read<uint64_t>(K);
    vector<unsigned> vj(K), vs = nvector::Read<unsigned>(Q), ve = nvector::Read<unsigned>(Q);
    for (auto& v : vs) v -= 1;
    for (auto& v : ve) v -= 1;
    stack<unsigned> st;
    for (unsigned i = 0; i < K; ++i) {
        if (s[i] == '(') {st.push(i);}else {assert(!st.empty()); vj[i] = s.top(); vj[s.top()] = i; s.pop();}
    }
    assert(s.empty());
    DirectedGraphEI<uint64_t> g(K);
    for (unsigned i =0 ; i < K; ++i) {
        if (i > 0) g.AddEdge(i, i-1, vl[i]);
        if (i < (K-1)) g.AddEdge(i, i+1, vr[i]);
        g.AddEdge(i, vj[i], vp[i]);
    }
    // ...
    cout << "Case #" << it << ": " << endl;
  }
  return 0;
}
