#pragma once

namespace tester {
namespace bst {

enum class TestType { kSmall, kBase, kExpensiveData, kSegment, kJoinSplit };

bool test(TestType test_type);

}  // namespace bst
}  // namespace tester
