#pragma once

#include <iostream>

namespace geometry {
namespace kdtree {
template <class TNode>
inline void Write(const TNode* p, unsigned deep) {
  for (unsigned i = 0; i < deep; ++i) std::cout << "  ";
  if (p->IsLeaf()) {
    std::cout << "[" << p->ldata << "]" << std::endl;
  } else {
    std::cout << "d = " << p->split_dim << " v = " << p->split_value
              << std::endl;
    PrintI(p->l, d + 1);
    PrintI(p->r, d + 1);
  }
}

template <class TISPTree>
inline void WriteISPTree(const TISPTree& tree) {
  std::cout << "[" << tree.sb << "] [" << tree.se << "]" << std::endl;
  Write(tree.root, 0);
}
}  // namespace kdtree
}  // namespace geometry
