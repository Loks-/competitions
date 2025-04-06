#include "tester/binary_search_tree/data.h"

#include "common/base.h"
#include "common/vector/shuffle.h"

#include <algorithm>
#include <unordered_map>

namespace tester {
namespace bst {

namespace {
std::vector<int64_t> empty_data_int64;

// Anonymous namespace for static instances per size
std::unordered_map<unsigned, std::vector<int64_t>> increasing_data;
std::unordered_map<unsigned, std::vector<int64_t>> reverse_data;
std::unordered_map<unsigned, std::vector<int64_t>> shuffled_data;
std::unordered_map<unsigned, std::vector<int64_t>> shuffled_dups_data;
}  // namespace

[[nodiscard]] const std::vector<int64_t>& get_data_int64(DataType type,
                                                         size_t size) {
  switch (type) {
    case DataType::kIncreasing: {
      auto& data = increasing_data[size];
      if (data.empty()) {
        data.resize(size);
        for (size_t i = 0; i < size; ++i) {
          data[i] = 2 * i;
        }
      }
      assert(data.size() == size);
      return data;
    }
    case DataType::kReverse: {
      auto& data = reverse_data[size];
      if (data.empty()) {
        data.resize(size);
        for (size_t i = 0; i < size; ++i) {
          data[i] = 2 * (size - 1 - i);
        }
      }
      assert(data.size() == size);
      return data;
    }
    case DataType::kShuffled: {
      auto& data = shuffled_data[size];
      if (data.empty()) {
        data.resize(size);
        for (size_t i = 0; i < size; ++i) {
          data[i] = 2 * i;
        }
        nvector::Shuffle(data);
      }
      assert(data.size() == size);
      return data;
    }
    case DataType::kShuffledDuplicates: {
      auto& data = shuffled_dups_data[size];
      if (data.empty()) {
        data.resize(size);
        for (size_t i = 0; i < size; ++i) {
          data[i] = 2 * (i / 4);
        }
        nvector::Shuffle(data);
      }
      assert(data.size() == size);
      return data;
    }
  }
  assert(false);  // Unreachable
  return empty_data_int64;
}

[[nodiscard]] int64_t get_sorted_value_int64(DataType type, size_t index) {
  switch (type) {
    case DataType::kIncreasing:
    case DataType::kReverse:
    case DataType::kShuffled:
      return 2 * index;
    case DataType::kShuffledDuplicates:
      return 2 * (index / 4);
  }
  assert(false);  // Unreachable
  return 0;
}

[[nodiscard]] std::string get_name(DataType type) {
  switch (type) {
    case DataType::kIncreasing:
      return "increasing";
    case DataType::kReverse:
      return "reverse";
    case DataType::kShuffled:
      return "shuffled";
    case DataType::kShuffledDuplicates:
      return "shuffled_dups";
  }
  assert(false);  // Unreachable
  return "unknown";
}

}  // namespace bst
}  // namespace tester
