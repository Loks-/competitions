#pragma once

#include "common/base.h"
#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include <algorithm>
#include <stack>
#include <vector>

namespace graph {
class CentroidDecomposition {
 protected:
  // Edges from smaller groups to parent are removed.
  TreeGraph tree;
  TreeNodesInfo info;
  std::vector<unsigned> group, group_root;
  std::stack<unsigned> vertices_to_check;

 public:
  const TreeGraph& GetCurrentTree() const { return tree; }
  const std::vector<unsigned>& GetGroupMap() const { return group; }

 protected:
  CentroidDecomposition(const TreeGraph& _tree) : tree(_tree) {
    unsigned n = tree.Size();
    group.resize(n, 0);
    group_root.reserve(n);
  }

  void InitParentAndSubtreeSizes() {
    info.Init(tree);
    group_root.clear();
    group_root.push_back(info.root);
  }

  void FindCentroid(unsigned current_group) {
    unsigned root = group_root[current_group];
    unsigned group_size = info.subtree_size[root];
    if (group_size <= 2) return;
    unsigned best_size = group_size, best_node = root;
    unsigned v = root;
    for (bool next = true; next;) {
      next = false;
      unsigned p = info.parent[v];
      for (unsigned c : tree.Edges(v)) {
        if (c == p) continue;
        if (info.subtree_size[c] > group_size / 2) {
          v = c;
          next = true;
          break;
        }
      }
    }
    if (v != root) {
      // Update parent and subtree_size with new root in centroid
      unsigned vparent = info.parent[v];
      unsigned vsize = info.subtree_size[v];
      group_root[current_group] = v;
      info.parent[v] = info.parent[root];
      info.subtree_size[v] = group_size;
      for (; v != root;) {
        unsigned vnextparent = info.parent[vparent];
        info.parent[vparent] = v;
        v = vparent;
        vparent = vnextparent;
        unsigned vnextsize = info.subtree_size[v];
        info.subtree_size[v] = group_size - vsize;
        vsize = vnextsize;
      }
    }
  }

  void Decompose(unsigned current_group) {
    unsigned root = group_root[current_group];
    if (info.subtree_size[root] == 1) return;
    for (unsigned v : tree.Edges(root)) {
      unsigned new_group = unsigned(group_root.size());
      group_root.push_back(v);
      group[v] = new_group;
      std::vector<unsigned>& vedges = tree.Edges(v);
      for (unsigned i = 0; i < vedges.size();) {
        if (vedges[i] == root) {
          std::swap(vedges[i], vedges.back());
          vedges.pop_back();
        } else {
          vertices_to_check.push(vedges[i++]);
        }
      }
      for (; !vertices_to_check.empty();) {
        unsigned u = vertices_to_check.top();
        vertices_to_check.pop();
        unsigned p = info.parent[u];
        group[u] = new_group;
        for (unsigned c : tree.Edges(u)) {
          if (c == p) continue;
          vertices_to_check.push(c);
        }
      }
    }
  }

 public:
  template <class TCentroidDecompositionCallBack>
  static void Run(const TreeGraph& tree,
                  TCentroidDecompositionCallBack& callback) {
    CentroidDecomposition cd(tree);
    cd.InitParentAndSubtreeSizes();
    for (unsigned i = 0; i < cd.group_root.size(); ++i) {
      cd.FindCentroid(i);
      callback.PreDecompose(cd.group_root[i], cd);
      cd.Decompose(i);
      callback.PostDecompose(cd.group_root[i], cd);
    }
    if (callback.IsComposeRequired()) {
      // ...
      assert(false);
    }
  }
};

class ICentroidDecompositionCallBack {
 public:
  void PreDecompose(unsigned vertex, const CentroidDecomposition& cd) {}
  void PostDecompose(unsigned vertex, const CentroidDecomposition& cd) {}
  bool IsComposeRequired() const { return false; }
  void Compose(unsigned vertex, const CentroidDecomposition& cd) {}
};
}  // namespace graph
