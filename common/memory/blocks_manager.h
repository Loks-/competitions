#pragma once

#include "common/base.h"

#include <algorithm>
#include <stack>
#include <vector>

namespace memory {
template <class TTNode>
// For each request 'block_size' nodes will be allocated in straight memory.
// All allocated memory will be uninitialized.
class BlocksManager {
 public:
  using TNode = TTNode;

 protected:
  size_t block_size = 0;
  std::vector<std::vector<TNode>> nodes;
  size_t total_blocks, used_blocks;
  size_t current_index1, current_index2;
  std::stack<TNode*> released_blocks;

 public:
  constexpr explicit BlocksManager(size_t block_size_) {
    Clear();
    InitBlockSize(block_size_);
  }

  constexpr BlocksManager() : BlocksManager(1) {}

  constexpr void Clear() {
    nodes.clear();
    total_blocks = used_blocks = 0;
    current_index1 = current_index2 = 0;
    std::stack<TNode*>().swap(released_blocks);
  }

  constexpr void InitBlockSize(size_t new_block_size) {
    if (new_block_size == block_size) return;
    Clear();
    block_size = new_block_size;
  }

  constexpr void ReserveBlocks(size_t new_max_blocks) {
    if (new_max_blocks > total_blocks) {
      nodes.resize(nodes.size() + 1);
      const auto new_blocks =
          std::max(new_max_blocks - total_blocks, total_blocks);
      nodes.back().resize(block_size * new_blocks);
      total_blocks += new_blocks;
    }
  }

  constexpr void ReserveNodes(size_t new_max_nodes) {
    ReserveBlocks(new_max_nodes / block_size + 1);
  }

  constexpr void ReserveAdditionalBlocks(size_t new_blocks) {
    if (ReservedBlocks() < new_blocks) {
      ReserveBlocks(UsedBlocks() + new_blocks);
    }
  }

  constexpr void ReserveAdditionalNodes(size_t new_nodes) {
    if (ReservedNodes() < new_nodes) {
      ReserveNodes(UsedNodes() + new_nodes);
    }
  }

  constexpr TNode* NewBlock() {
    if (!released_blocks.empty()) {
      TNode* p = released_blocks.top();
      released_blocks.pop();
      ++used_blocks;
      return p;
    } else {
      ReserveAdditionalBlocks(1);
      if (current_index2 >= nodes[current_index1].size()) {
        ++current_index1;
        current_index2 = 0;
      }
      TNode* p = &(nodes[current_index1][current_index2]);
      current_index2 += block_size;
      ++used_blocks;
      return p;
    }
  }

  constexpr void ReleaseBlock(TNode* p) {
    released_blocks.push(p);
    --used_blocks;
  }

  constexpr size_t SizeBlocks() const { return total_blocks; }

  constexpr size_t UsedBlocks() const { return used_blocks; }

  constexpr size_t ReservedBlocks() const {
    return SizeBlocks() - UsedBlocks();
  }

  constexpr size_t SizeNodes() const { return block_size * SizeBlocks(); }

  constexpr size_t UsedNodes() const { return block_size * UsedBlocks(); }

  constexpr size_t ReservedNodes() const {
    return block_size * ReservedBlocks();
  }
};
}  // namespace memory
