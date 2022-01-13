#include "common/binary_search_tree/base/deep.h"
#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/next_leaf.h"
#include "common/binary_search_tree/base/prev_leaf.h"
#include "common/binary_search_tree/base_tree.h"
#include "common/binary_search_tree/info/none.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <functional>

using TTree = bst::BaseTree<true, unsigned, bst::info::None>;
using TNode = TTree::TNode;

int main_2118() {
  TTree tree(128);
  TNode* root = nullptr;

  std::function<TNode*(string)> Parse = [&](string s) -> TNode* {
    if (s[0] == '[') {
      s = s.substr(1, s.size() - 2);
      unsigned d = 0;
      for (unsigned i = 0; i < s.size(); ++i) {
        if (s[i] == '[') ++d;
        if (s[i] == ']') --d;
        if ((s[i] == ',') && (d == 0)) {
          auto n = tree.New(0);
          n->SetL(Parse(s.substr(0, i)));
          n->SetR(Parse(s.substr(i + 1)));
          return n;
        }
      }
      return nullptr;
    } else {
      return tree.New(stoi(s));
    }
  };

  auto Explode = [&](TNode* l) {
    auto p = l->p;
    auto l0 = bst::base::PrevLeaf(p->l), r0 = bst::base::NextLeaf(p->r);
    if (l0) l0->data += p->l->data;
    if (r0) r0->data += p->r->data;
    tree.Release(p->l);
    tree.Release(p->r);
    p->SetL(nullptr);
    p->SetR(nullptr);
  };

  auto Split = [&](TNode* n) {
    n->SetL(tree.New(n->data / 2));
    n->SetR(tree.New((n->data + 1) / 2));
    n->data = 0;
  };

  auto Compress = [&]() {
    for (auto n = bst::base::Left(root); n; n = bst::base::NextLeaf(n)) {
      if (bst::base::Deep(n) > 5) {
        Explode(n);
        return true;
      }
    }
    for (auto n = bst::base::Left(root); n; n = bst::base::NextLeaf(n)) {
      if (n->data > 9) {
        Split(n);
        return true;
      }
    }
    return false;
  };

  auto CompressAll = [&]() {
    while (Compress()) {
    }
  };

  std::function<int64_t(TNode*)> Mag = [&](TNode* node) -> int64_t {
    return (node->l) ? 3 * Mag(node->l) + 2 * Mag(node->r) : node->data;
  };

  auto vs = nvector::ReadLines();
  for (auto s : vs) {
    auto n = Parse(s);
    if (root) {
      auto new_root = tree.New(0);
      new_root->SetL(root);
      new_root->SetR(n);
      root = new_root;
      CompressAll();
    } else {
      root = n;
    }
  }
  cout << Mag(root) << endl;

  int64_t r = 0;
  for (auto s1 : vs) {
    for (auto s2 : vs) {
      if (s1 == s2) continue;
      tree.ReleaseTree(root->l);
      tree.ReleaseTree(root->r);
      root->SetL(Parse(s1));
      root->SetR(Parse(s2));
      CompressAll();
      auto m = Mag(root);
      r = max(r, m);
    }
  }
  cout << r << endl;
  return 0;
}
