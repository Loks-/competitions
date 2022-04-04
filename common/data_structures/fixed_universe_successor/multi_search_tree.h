#pragma once

#include "common/data_structures/fixed_universe_successor/empty.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// Based on A.G. code.
// Memory      -- O(S)
// Init        -- O(1)
// Insert      -- O(log U)
// HasKey      -- O(log U)
// Delete      -- O(log U)
// Size        -- O(1)
// Min         -- O(log U)
// Max         -- O(log U)
// Successor   -- O(log U)
// Predecessor -- O(log U)
template <class TMask>
class MultiSearchTree {
  struct Node {
    size_t key = 0;
    size_t count = 0;
    size_t min_value = 0;
    size_t max_value = 0;
    TMask mask;

    bool IsEmpty() const { return count == 0; }
    bool IsSplit() const { return count > 1; }
  };
  enum { LEVEL_BITS = TMask::nbits, LEVEL_MASK = (1ull << LEVEL_BITS) - 1 };
  std::vector<Node> Nodes;
  Node root;
  size_t hash_shift = 0;
  size_t node_ptr_mask = 0;
  size_t nodes_used = 0;
  size_t max_depth = 0;
  std::vector<Node *> path;

  // depth - level of current split
  size_t CalcHighBits(size_t x, size_t depth) const {
    if (depth >= max_depth) {
      return 0;
    }
    size_t maskLowBits = (1ull << ((depth + 1) * LEVEL_BITS)) - 1;
    return (x & ~maskLowBits);
  }

  size_t CalcKey(size_t x, size_t depth) const {
    return CalcHighBits(x, depth) + depth;
  }

  size_t CalcHash(size_t x) const {
    const size_t MAGIC_MULT = 0x4906ba494954cb65ull;
    return (x * MAGIC_MULT) >> hash_shift;
  }

  size_t CalcLevelIndex(size_t x, size_t depth) const {
    size_t idx = (x >> (depth * LEVEL_BITS)) & LEVEL_MASK;
    return idx;
  }

  void ResizeHash(size_t count) {
    // invalidates all pointers
    size_t nodeCountLn = 2;
    while ((1ull << nodeCountLn) < count * 2) {
      ++nodeCountLn;
    }
    hash_shift = 64 - nodeCountLn;
    size_t nodeCount = 1ull << nodeCountLn;
    std::vector<Node> newNodes;
    // ClearPodArray(&newNodes, nodeCount);
    newNodes.resize(nodeCount);
    node_ptr_mask = nodeCount - 1;
    if (nodes_used > 0) {
      for (const Node &x : Nodes) {
        if (x.count > 0) {
          for (size_t idx = CalcHash(x.key);; ++idx) {
            size_t ptr = idx & node_ptr_mask;
            if (newNodes[ptr].IsEmpty()) {
              newNodes[ptr] = x;
              break;
            }
          }
        }
      }
    }
    Nodes.swap(newNodes);
  }

  size_t FindNodeIndex(size_t key) const {
    for (size_t idx = CalcHash(key);; ++idx) {
      size_t ptr = idx & node_ptr_mask;
      if (Nodes[ptr].key == key) {
        return ptr;
      }
    }
  }

  Node *FindNode(size_t x, size_t depth) {
    size_t key = CalcKey(x, depth);
    return &Nodes[FindNodeIndex(key)];
  }

  const Node *FindNode(size_t x, size_t depth) const {
    size_t key = CalcKey(x, depth);
    return &Nodes[FindNodeIndex(key)];
  }

  Node *AddNode(size_t x, size_t depth) {
    ++nodes_used;
    size_t key = CalcKey(x, depth);
    for (size_t idx = CalcHash(key);; ++idx) {
      size_t ptr = idx & node_ptr_mask;
      if (Nodes[ptr].IsEmpty()) {
        Nodes[ptr].key = key;
        return &Nodes[ptr];
      }
      assert(Nodes[ptr].key != key);  // inserting same key twice
    }
  }

  void DeleteSubtree(Node *node, size_t x, size_t depth) {
    bool needRecursion = node->IsSplit();
    node->count = 0;
    --nodes_used;
    if (depth == 0 || !needRecursion) {
      return;
    }
    size_t highX = CalcHighBits(x, depth);
    size_t idx;
    while (!node->mask.IsEmpty()) {
      idx = node->mask.MinI();
      node->mask.Delete(idx);
      size_t bitX = highX + (idx << (depth * LEVEL_BITS));
      Node *subNode = FindNode(bitX, depth - 1);
      DeleteSubtree(subNode, bitX, depth - 1);
    }
  }

  void MakeSingleValueNode(Node *node, size_t x, size_t depth) {
    node->count = 1;
    node->min_value = x;
    node->max_value = x;
    node->mask.Set1(CalcLevelIndex(x, depth));
  }

 public:
  MultiSearchTree() { Init(64); }
  void Init(size_t maxValue) {
    root.count = 0;
    root.min_value = 0;
    root.max_value = 0;
    root.mask.Clear();
    max_depth = 0;
    for (;; ++max_depth) {
      size_t bits = (max_depth + 1) * LEVEL_BITS;
      if (bits >= 64) {
        break;
      }
      if ((1ull << bits) >= maxValue) {
        break;
      }
    }
    path.resize(max_depth + 1);
    ResizeHash(1000);
  }

  size_t Min() const {
    if (root.IsEmpty()) {
      return ds::fus::Empty;
    }
    return root.min_value;
  }

  size_t Max() const {
    if (root.IsEmpty()) {
      return ds::fus::Empty;
    }
    return root.max_value;
  }

  size_t Successor(size_t x) {
    if (root.IsEmpty()) {
      return ds::fus::Empty;
    }
    Node *node = &root;
    size_t depth = max_depth;
    for (;;) {
      if (!node->IsSplit()) {
        assert(node->count == 1);
        if (node->min_value > x) {
          return node->min_value;
        } else {
          ++depth;
          break;
        }
      }
      path[depth] = node;
      size_t idx = CalcLevelIndex(x, depth);
      if (!node->mask.HasKey(idx)) {
        break;
      }
      if (depth == 0) {
        break;
      }
      --depth;
      node = FindNode(x, depth);
    }
    for (; depth <= max_depth; ++depth) {
      node = path[depth];
      size_t idx = CalcLevelIndex(x, depth);
      //   size_t minIdx;
      //   if (node->mask.GetLowestBitAfter(idx, &minIdx)) {
      size_t minIdx = node->mask.Successor(idx);
      if (minIdx != Empty) {
        size_t keyX = CalcHighBits(x, depth) + (minIdx << (depth * LEVEL_BITS));
        if (depth > 0) {
          const Node *nextNode = FindNode(keyX, depth - 1);
          return nextNode->min_value;
        } else {
          return keyX;
        }
      }
    }
    return ds::fus::Empty;
  }

  size_t Predecessor(size_t x) {
    if (root.IsEmpty()) {
      return ds::fus::Empty;
    }
    Node *node = &root;
    size_t depth = max_depth;
    for (;;) {
      if (!node->IsSplit()) {
        assert(node->count == 1);
        if (node->min_value < x) {
          return node->min_value;
        } else {
          ++depth;
          break;
        }
      }
      size_t idx = CalcLevelIndex(x, depth);
      if (!node->mask.HasKey(idx)) {
        break;
      }
      if (depth == 0) {
        break;
      }
      --depth;
      node = FindNode(x, depth);
    }
    for (; depth <= max_depth; ++depth) {
      node = path[depth];
      size_t idx = CalcLevelIndex(x, depth);
      //   size_t maxIdx;
      //   if (node->mask.GetHighestBitBefore(idx, &maxIdx)) {
      size_t maxIdx = node->mask.Predecessor(idx);
      if (maxIdx != Empty) {
        size_t keyX = CalcHighBits(x, depth) + (maxIdx << (depth * LEVEL_BITS));
        if (depth > 0) {
          const Node *prevNode = FindNode(keyX, depth - 1);
          return prevNode->max_value;
        } else {
          return keyX;
        }
      }
    }
    return ds::fus::Empty;
  }

  bool HasKey(size_t x) const {
    if (root.IsEmpty()) {
      return false;
    }
    const Node *node = &root;
    for (size_t depth = max_depth;;) {
      if (!node->IsSplit()) {
        assert(node->count == 1);
        if (node->min_value == x) {
          return true;
        } else {
          return false;
        }
      }
      size_t idx = CalcLevelIndex(x, depth);
      if (!node->mask.HasKey(idx)) {
        return false;
      }
      if (depth == 0) {
        return true;
      }
      --depth;
      node = FindNode(x, depth);
    }
  }

  void Delete(size_t x) {
    // assert(HasKey(x));
    Node *prevNode = nullptr;
    Node *node = &root;
    size_t prevIdx = 0;
    size_t depth = max_depth;
    for (;;) {
      path[depth] = node;
      if (node->count == 1) {
        assert(!node->IsSplit());
        node->count = 0;
        assert(node->min_value == x);
        if (prevNode) {
          --nodes_used;
          assert(prevNode->mask.HasKey(prevIdx));
          prevNode->mask.Delete(prevIdx);
        }
        break;
      } else if (node->count == 2) {
        // unsplit node
        size_t otherX =
            (x == node->min_value) ? node->max_value : node->min_value;
        DeleteSubtree(node, x, depth);
        ++nodes_used;
        MakeSingleValueNode(node, otherX, depth);
        break;
      } else {
        --node->count;
        prevIdx = CalcLevelIndex(x, depth);
        prevNode = node;
        assert(prevNode->mask.HasKey(prevIdx));
        if (depth == 0) {
          prevNode->mask.Delete(prevIdx);
          if (prevNode->min_value == x) {
            size_t minIdx = prevNode->mask.MinI();
            prevNode->min_value = CalcHighBits(x, depth) + minIdx;
          } else if (prevNode->max_value == x) {
            size_t maxIdx = prevNode->mask.MaxI();
            prevNode->max_value = CalcHighBits(x, depth) + maxIdx;
          }
          break;
        } else {
          --depth;
          node = FindNode(x, depth);
        }
      }
    }
    ++depth;
    for (; depth <= max_depth; ++depth) {
      node = path[depth];
      if (node->min_value == x) {
        assert(node->max_value !=
               x);  // possible if multiple copies of same value allowed
        size_t minIdx = node->mask.MinI();
        size_t keyX = CalcHighBits(x, depth) + (minIdx << (depth * LEVEL_BITS));
        Node *minNode = FindNode(keyX, depth - 1);
        node->min_value = minNode->min_value;
      } else if (node->max_value == x) {
        size_t maxIdx = node->mask.MaxI();
        size_t keyX = CalcHighBits(x, depth) + (maxIdx << (depth * LEVEL_BITS));
        Node *maxNode = FindNode(keyX, depth - 1);
        node->max_value = maxNode->max_value;
      } else {
        break;
      }
    }
    // assert(!HasKey(x));
  }

  void Insert(size_t x) {
    // assert(!HasKey(x));
    const size_t SPARSITY = 3;
    const size_t GROWTH = 4;
    if (nodes_used * SPARSITY > node_ptr_mask) {
      ResizeHash(GROWTH * (node_ptr_mask + 1));
    }
    // printf("insert %d\n", (int)x);
    Node *node = &root;
    for (size_t depth = max_depth;;) {
      if (node->count == 0) {
        MakeSingleValueNode(node, x, depth);
        return;
      } else {
        if (node->count == 1) {
          // split the node
          if (depth > 0) {
            size_t sx = node->min_value;
            Node *sn = AddNode(sx, depth - 1);
            MakeSingleValueNode(sn, sx, depth - 1);
          }
        }
        // add to the splitted node
        ++node->count;  // could try to avoid these writes
        node->min_value = std::min(node->min_value, x);
        node->max_value = std::max(node->max_value, x);
        size_t idx = CalcLevelIndex(x, depth);
        if (node->mask.HasKey(idx)) {
          if (depth == 0) {
            assert(false);  // duplicate values not supported
            return;
          }
          --depth;
          node = FindNode(x, depth);
        } else {
          node->mask.Insert(idx);
          if (depth == 0) {
            return;
          }
          --depth;
          node = AddNode(x, depth);
        }
      }
    }
  }
};

// struct TReferenceST
// {
//     yhash_map<size_t, bool, I64Hash> Vals;

//     void Init(yint) {}
//     bool HasKey(size_t x)
//     {
//         return Vals.find(x) != Vals.end();
//     }
//     void Delete(size_t x)
//     {
//         auto it = Vals.find(x);
//         Vals.erase(it);
//     }
//     void Insert(size_t x)
//     {
//         Vals[x];
//     }
// };
}  // namespace fus
}  // namespace ds
