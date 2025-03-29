#pragma once

#include "common/base.h"

#include <algorithm>
#include <stack>
#include <type_traits>
#include <utility>
#include <vector>

namespace memory {

/**
 * @brief Manages a pool of nodes allocated in fixed-size blocks.
 *
 * This class manages a pool of nodes that are allocated in fixed-size blocks
 * of contiguous memory. Unlike NodesManager, this class allocates and releases
 * nodes in blocks rather than individually. This can be more efficient when
 * multiple nodes are needed at once, as it ensures they are allocated in
 * contiguous memory.
 *
 * The block-based allocation strategy provides better memory locality
 * for nodes that are used together, but comes at the cost of potentially
 * wasting memory when not all nodes in a block are used.
 *
 * @tparam TNode The type of node to manage.
 * @tparam manage_nodes_lifecycle If true, the manager will call initialize(),
 *                               release(), and reuse() on nodes. If false,
 *                               these methods will not be called.
 */
template <typename TNode, bool manage_nodes_lifecycle = true>
class BlocksManager {
 public:
  using NodeType = TNode;

  /**
   * @brief Constructs a blocks manager with specified block size.
   *
   * @param block_size The number of nodes in each block.
   *                  Must be greater than 0.
   */
  [[nodiscard]] constexpr explicit BlocksManager(size_t block_size) {
    assert(block_size > 0);
    clear();
    init_block_size(block_size);
  }

  /**
   * @brief Default constructor with block size of 1.
   */
  [[nodiscard]] constexpr BlocksManager() : BlocksManager(1) {}

  /**
   * @brief Copy constructor is deleted.
   *
   * BlocksManager cannot be copied because it manages raw pointers and
   * maintains internal state about block allocation.
   */
  BlocksManager(const BlocksManager&) = delete;

  /**
   * @brief Copy assignment operator is deleted.
   *
   * BlocksManager cannot be copied because it manages raw pointers and
   * maintains internal state about block allocation.
   */
  BlocksManager& operator=(const BlocksManager&) = delete;

  /**
   * @brief Move constructor.
   *
   * @param other The blocks manager to move from. After the move,
   *              other will be in a valid but unspecified state.
   */
  constexpr BlocksManager(BlocksManager&& other) noexcept
      : block_size_(std::exchange(other.block_size_, 0)),
        nodes_(std::move(other.nodes_)),
        total_blocks_(std::exchange(other.total_blocks_, 0)),
        used_blocks_(std::exchange(other.used_blocks_, 0)),
        current_index1_(std::exchange(other.current_index1_, 0)),
        current_index2_(std::exchange(other.current_index2_, 0)),
        released_blocks_(std::move(other.released_blocks_)) {}

  /**
   * @brief Move assignment operator.
   *
   * @param other The blocks manager to move from. After the move,
   *              other will be in a valid but unspecified state.
   * @return Reference to this blocks manager.
   */
  constexpr BlocksManager& operator=(BlocksManager&& other) noexcept {
    if (this != &other) {
      block_size_ = std::exchange(other.block_size_, 0);
      nodes_ = std::move(other.nodes_);
      total_blocks_ = std::exchange(other.total_blocks_, 0);
      used_blocks_ = std::exchange(other.used_blocks_, 0);
      current_index1_ = std::exchange(other.current_index1_, 0);
      current_index2_ = std::exchange(other.current_index2_, 0);
      released_blocks_ = std::move(other.released_blocks_);
    }
    return *this;
  }

  /**
   * @brief Reserves space for at least the specified number of blocks.
   *
   * If the requested capacity is greater than the current capacity,
   * the capacity is increased to at least the requested size.
   *
   * @param new_max_blocks The minimum number of blocks to reserve.
   */
  constexpr void reserve_blocks(size_t new_max_blocks) {
    if (new_max_blocks > total_blocks_) {
      nodes_.resize(nodes_.size() + 1);
      const auto new_blocks =
          std::max(new_max_blocks - total_blocks_, total_blocks_);
      nodes_.back().resize(block_size_ * new_blocks);
      total_blocks_ += new_blocks;
    }
  }

  /**
   * @brief Reserves space for at least the specified number of nodes.
   *
   * This function converts the requested number of nodes into blocks
   * and reserves the appropriate number of blocks.
   *
   * @param new_max_nodes The minimum number of nodes to reserve.
   */
  constexpr void reserve_nodes(size_t new_max_nodes) {
    reserve_blocks(new_max_nodes / block_size_ + 1);
  }

  /**
   * @brief Reserves additional space for the specified number of blocks.
   *
   * If the current available capacity is less than the requested number
   * of blocks, the capacity is increased to accommodate the additional blocks.
   *
   * @param new_blocks The number of additional blocks to reserve.
   */
  constexpr void reserve_additional_blocks(size_t new_blocks) {
    if (available_blocks() < new_blocks) {
      reserve_blocks(used_blocks_ + new_blocks);
    }
  }

  /**
   * @brief Reserves additional space for the specified number of nodes.
   *
   * This function converts the requested number of nodes into blocks
   * and reserves the appropriate number of additional blocks.
   *
   * @param new_nodes The number of additional nodes to reserve.
   */
  constexpr void reserve_additional_nodes(size_t new_nodes) {
    if (available_nodes() < new_nodes) {
      reserve_nodes(used_nodes() + new_nodes);
    }
  }

  /**
   * @brief Allocates a new block of nodes.
   *
   * If there are any released blocks available, one is reused.
   * Otherwise, a new block is allocated from the pool.
   *
   * @return Pointer to the first node in the new block.
   */
  [[nodiscard]] constexpr NodeType* new_block() {
    if (!released_blocks_.empty()) {
      NodeType* p = released_blocks_.top();
      released_blocks_.pop();
      ++used_blocks_;
      if constexpr (manage_nodes_lifecycle) {
        for (size_t i = 0; i < block_size_; ++i) {
          p[i].reuse();
        }
      }
      return p;
    }

    reserve_additional_blocks(1);
    if (current_index2_ >= nodes_[current_index1_].size()) {
      ++current_index1_;
      current_index2_ = 0;
    }
    NodeType* p = &(nodes_[current_index1_][current_index2_]);
    current_index2_ += block_size_;
    ++used_blocks_;
    if constexpr (manage_nodes_lifecycle) {
      for (size_t i = 0; i < block_size_; ++i) {
        p[i].initialize((used_blocks_ - 1) * block_size_ + i);
      }
    }
    return p;
  }

  /**
   * @brief Releases a block of nodes for future reuse.
   *
   * The block is added to the stack of available blocks.
   * The block can be reused later by calling new_block().
   *
   * @param p Pointer to the first node in the block to release.
   */
  constexpr void release_block(NodeType* p) {
    if constexpr (manage_nodes_lifecycle) {
      for (size_t i = 0; i < block_size_; ++i) {
        p[i].release();
      }
    }
    released_blocks_.push(p);
    --used_blocks_;
  }

  /**
   * @brief Returns the total capacity in blocks.
   *
   * @return The total number of blocks that can be stored in the pool.
   */
  [[nodiscard]] constexpr size_t capacity_blocks() const noexcept {
    return total_blocks_;
  }

  /**
   * @brief Returns the number of currently used blocks.
   *
   * @return The number of blocks that are currently in use, not including
   *         blocks that have been released and are available for reuse.
   */
  [[nodiscard]] constexpr size_t used_blocks() const noexcept {
    return used_blocks_;
  }

  /**
   * @brief Returns the number of available blocks.
   *
   * @return The number of blocks that can be allocated without requiring
   *         additional memory allocation.
   */
  [[nodiscard]] constexpr size_t available_blocks() const noexcept {
    return capacity_blocks() - used_blocks();
  }

  /**
   * @brief Returns the total capacity in nodes.
   *
   * @return The total number of nodes that can be stored in the pool.
   */
  [[nodiscard]] constexpr size_t capacity_nodes() const noexcept {
    return block_size_ * capacity_blocks();
  }

  /**
   * @brief Returns the number of currently used nodes.
   *
   * @return The number of nodes that are currently in use, not including
   *         nodes in blocks that have been released and are available for
   * reuse.
   */
  [[nodiscard]] constexpr size_t used_nodes() const noexcept {
    return block_size_ * used_blocks();
  }

  /**
   * @brief Returns the number of available nodes.
   *
   * @return The number of nodes that can be allocated without requiring
   *         additional memory allocation.
   */
  [[nodiscard]] constexpr size_t available_nodes() const noexcept {
    return block_size_ * available_blocks();
  }

  /**
   * @brief Removes all blocks and frees allocated memory.
   *
   * This function releases all blocks and frees the memory allocated for them.
   * After this call, the manager will be empty and ready for new allocations.
   */
  constexpr void clear_memory() {
    if constexpr (manage_nodes_lifecycle) {
      // Handle all complete vectors
      for (size_t i = 0; i < current_index1_; ++i) {
        for (size_t j = 0; j < nodes_[i].size(); ++j) {
          nodes_[i][j].release();
        }
      }
      // Handle the current vector up to current_index2_
      if (current_index1_ < nodes_.size()) {
        for (size_t j = 0; j < current_index2_; ++j) {
          nodes_[current_index1_][j].release();
        }
      }
    }
    used_blocks_ = 0;
    current_index1_ = 0;
    current_index2_ = 0;
    std::stack<NodeType*>().swap(released_blocks_);
    std::vector<std::vector<NodeType>>().swap(nodes_);
  }

  /**
   * @brief Removes all blocks but keeps the memory allocated.
   *
   * This function releases all blocks and resets them to their initial state.
   * Unlike clear_memory(), this function does not free the memory, allowing for
   * immediate reuse of the blocks.
   */
  constexpr void clear() {
    if constexpr (manage_nodes_lifecycle) {
      // Handle all complete vectors
      for (size_t i = 0; i < current_index1_; ++i) {
        for (size_t j = 0; j < nodes_[i].size(); ++j) {
          nodes_[i][j].release();
          nodes_[i][j].reuse();
        }
      }
      // Handle the current vector up to current_index2_
      if (current_index1_ < nodes_.size()) {
        for (size_t j = 0; j < current_index2_; ++j) {
          nodes_[current_index1_][j].release();
          nodes_[current_index1_][j].reuse();
        }
      }
    }
    used_blocks_ = 0;
    current_index1_ = 0;
    current_index2_ = 0;
    std::stack<NodeType*>().swap(released_blocks_);
  }

  /**
   * @brief Initializes or reinitializes the block size.
   *
   * This function sets the size of each block and clears all existing blocks.
   * Must be called before allocating any blocks.
   *
   * @param new_block_size The new size for each block. Must be greater than 0.
   */
  constexpr void init_block_size(size_t new_block_size) {
    assert(new_block_size > 0);
    clear_memory();
    block_size_ = new_block_size;
  }

 protected:
  size_t block_size_ = 0;
  std::vector<std::vector<NodeType>> nodes_;
  size_t total_blocks_ = 0;
  size_t used_blocks_ = 0;
  size_t current_index1_ = 0;
  size_t current_index2_ = 0;
  std::stack<NodeType*> released_blocks_;
};

}  // namespace memory
