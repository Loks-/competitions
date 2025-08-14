#pragma once

#include <string_view>

namespace tester {
namespace bst {

enum class TestType { kSmall, kBase, kExpensiveData, kSplitJoin };

bool test(TestType test_type, std::string_view implementation_filter);

}  // namespace bst
}  // namespace tester
