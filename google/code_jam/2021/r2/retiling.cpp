#include "common/geometry/d2/distance/distance_l1.h"
#include "common/geometry/d2/point.h"
#include "common/stl/full.h"
#include "common/vector/read.h"

#include <string>

class CMinCostFlow {
 protected:
  struct Edge {
    unsigned to;
    int flow;
    int max_flow;
    int cost;
    unsigned reverse_edge;
    size_t external_id;  // for return edges in flow

    Edge(unsigned _to, int _max_flow, int _cost, unsigned _reverse_edge,
         size_t _id = 0)
        : to(_to),
          flow(0),
          max_flow(_max_flow),
          cost(_cost),
          reverse_edge(_reverse_edge),
          external_id(_id) {}
  };

 protected:
  vector<vector<Edge>> edges;
  bool edges_cost_non_negative;

 public:
  CMinCostFlow(unsigned nvertices = 0) { Init(nvertices); }

  void Init(unsigned vertices) {
    edges.resize(0);
    edges.resize(vertices);
    edges_cost_non_negative = true;
  }

  void AddEdge(unsigned from, unsigned to, int max_flow, int cost,
               size_t id = 0) {
    edges[from].push_back(
        Edge(to, max_flow, cost, unsigned(edges[to].size()), id));
    edges[to].push_back(
        Edge(from, 0, -cost, unsigned(edges[from].size() - 1), id));
    edges_cost_non_negative = edges_cost_non_negative && (cost >= 0);
  }

  static void BellmanFord(vector<vector<Edge>> edges, unsigned from,
                          vector<int>& output_dist) {
    unsigned n = unsigned(edges.size());
    output_dist.resize(n);
    fill(output_dist.begin(), output_dist.end(), INT_MAX);
    output_dist[from] = 0;
    vector<bool> inqueue(n, false);
    queue<unsigned> q;
    for (q.push(from); !q.empty();) {
      unsigned u = q.front();
      q.pop();
      inqueue[u] = false;
      for (unsigned i = 0; i < edges[u].size(); ++i) {
        Edge& e = edges[u][i];
        if (e.max_flow <= e.flow) continue;
        unsigned v = e.to;
        int new_dist = output_dist[u] + e.cost;
        if (output_dist[v] > new_dist) {
          output_dist[v] = new_dist;
          if (!inqueue[v]) {
            inqueue[v] = true;
            q.push(v);
          }
        }
      }
    }
  }

  pair<unsigned, int64_t> MinCostFlow(unsigned from, unsigned to,
                                      unsigned max_flow = UINT_MAX) {
    unsigned n = unsigned(edges.size());

    vector<int> prio(n);
    vector<int> pot(n, 0);
    vector<int> curflow(n);
    vector<bool> finished(n);
    vector<unsigned> prevnode(n);
    vector<unsigned> prevedge(n);
    priority_queue<pair<int, unsigned>, vector<pair<int, unsigned>>,
                   greater<pair<int, unsigned>>>
        q;

    if (!edges_cost_non_negative) {
      BellmanFord(edges, from, pot);
    }
    unsigned current_flow = 0;
    int64_t current_flow_cost = 0;
    while (current_flow < max_flow) {
      q.push(make_pair(0, from));
      fill(prio.begin(), prio.end(), INT_MAX);
      fill(finished.begin(), finished.end(), false);
      prio[from] = 0;
      curflow[from] = INT_MAX;
      while (!finished[to] && !q.empty()) {
        pair<int, unsigned> cur = q.top();
        q.pop();
        unsigned u = cur.second;
        int priou = cur.first;
        if (priou != prio[u]) continue;
        for (unsigned i = 0; i < edges[u].size(); ++i) {
          Edge& e = edges[u][i];
          if (e.flow >= e.max_flow) continue;
          unsigned v = e.to;
          int nprio = prio[u] + e.cost + pot[u] - pot[v];
          if (prio[v] > nprio) {
            prio[v] = nprio;
            q.push(make_pair(nprio, v));
            prevnode[v] = u;
            prevedge[v] = i;
            curflow[v] = min(curflow[u], e.max_flow - e.flow);
          }
        }
        finished[u] = true;
      }
      if (prio[to] == INT_MAX) break;  // Max flow already
      for (unsigned i = 0; i < n; ++i) {
        if (finished[i]) {
          pot[i] += prio[i] - prio[to];
        }
      }
      unsigned dflow = min(unsigned(curflow[to]), max_flow - current_flow);
      current_flow += dflow;
      for (unsigned v = to; v != from; v = prevnode[v]) {
        Edge& e = edges[prevnode[v]][prevedge[v]];
        e.flow += dflow;
        edges[v][e.reverse_edge].flow -= dflow;
        current_flow_cost += dflow * e.cost;
      }
    }
    return make_pair(current_flow, current_flow_cost);
  }

  vector<pair<size_t, int>> GetEdgesWithFlow() const {
    vector<pair<size_t, int>> v;
    for (unsigned i = 0; i < edges.size(); ++i) {
      for (unsigned j = 0; j < edges[i].size(); ++j) {
        const Edge& e = edges[i][j];
        if (e.flow <= 0) continue;
        v.push_back(make_pair(e.external_id, e.flow));
      }
    }
    return v;
  }
};

int main_retiling() {
  unsigned T, R, C, F, S;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C >> F >> S;
    auto vs = nvector::Read<string>(R);
    auto ve = nvector::Read<string>(R);
    std::vector<int> vr;
    for (unsigned ic = 0; ic < 2; ++ic) {
      char c = (ic == 0) ? 'M' : 'G';
      vector<I2Point> vps, vpe;
      for (unsigned i = 0; i < R; ++i) {
        for (unsigned j = 0; j < C; ++j) {
          if (vs[i][j] == c) vps.push_back({i, j});
          if (ve[i][j] == c) vpe.push_back({i, j});
        }
      }
      if (vps.size() > vpe.size()) vps.swap(vpe);
      unsigned h = vpe.size(), nf = 2 * h, nt = nf + 1;
      CMinCostFlow mcf(2 * h + 2);
      for (unsigned i = 0; i < h; ++i) {
        mcf.AddEdge(nf, i, 1, 0);
        mcf.AddEdge(i + h, nt, 1, 0);
        for (unsigned j = 0; j < h; ++j) {
          int d;
          if (j < vps.size())
            d = std::min<int>(2 * F, S * DistanceL1(vps[j], vpe[i]));
          else
            d = F;
          mcf.AddEdge(j, i + h, 1, d);
        }
      }
      auto r = mcf.MinCostFlow(nf, nt, h);
      // cerr << vps.size() << " " << vpe.size() << " " << h << " " << r.first
      //      << " " << r.second << endl;
      vr.push_back(r.second);
    }
    cout << "Case #" << it << ": " << min(vr[0], vr[1]) << endl;
  }
  return 0;
}
