#pragma once

#include <string>
#include <vector>

namespace tester {
namespace bst {

enum class DataType { kIncreasing, kReverse, kShuffled, kShuffledDuplicates };

// Data access functions
[[nodiscard]] const std::vector<int64_t>& get_data_int64(DataType type,
                                                         size_t size);
[[nodiscard]] int64_t get_sorted_value_int64(DataType type, size_t index);
[[nodiscard]] std::string get_name(DataType type);

}  // namespace bst
}  // namespace tester
