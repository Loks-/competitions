#pragma once

#include "common/base.h"
#include "common/data_structures/cover/dlmatrix.h"
#include "common/data_structures/unsigned_set.h"
#include "common/vector/shuffle.h"

#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

namespace ds {
namespace cover {
// Extended DLX with support integer cover (min and max) for each column.
class DLXEMinMax : public DLMatrix {
 public:
  using TBase = DLMatrix;
  using Node = TBase::Node;

  static constexpr size_t missed = size_t(-1ll);
  static constexpr size_t stop = missed - 1;

 protected:
  std::vector<size_t> selected_rows;
  std::stack<Node*> disabled_nodes;
  std::stack<size_t> stack_sizes;
  std::vector<unsigned> count_min, count_max, count_current;
  ds::UnsignedSet uncovered;

 public:
  void Clear() {
    TBase::Clear();
    selected_rows.clear();
    std::stack<Node*>().swap(disabled_nodes);
    std::stack<size_t>().swap(stack_sizes);
    count_min.clear();
    count_max.clear();
    count_current.clear();
    uncovered.Clear();
  }

  void Init(size_t rows, size_t columns) {
    Clear();
    TBase::Init(rows, columns);
    count_min = count_max = std::vector<unsigned>(columns, 1);
    count_current = std::vector<unsigned>(columns, 0);
    uncovered.Resize(columns);
    uncovered.InsertAll();
  }

  void ApplyMinMax() {
    for (unsigned i = 0; i < count_max.size(); ++i) {
      if (count_min[i] == 0) uncovered.Remove(i);
      if (count_max[i] == 0) DisableColumn(i);
    }
  }

  // Should be called after matrix is initialized
  void SetColumnsMinMax(const std::vector<unsigned>& cmin,
                        const std::vector<unsigned>& cmax) {
    assert((cmin.size() == TBase::ncolumns) &&
           (cmax.size() == TBase::ncolumns));
    count_min = cmin;
    count_max = cmax;
    ApplyMinMax();
  }

  void ResetSearch() {
    selected_rows.clear();
    while (!disabled_nodes.empty()) EnableLastNode();
    std::stack<size_t>().swap(stack_sizes);
    std::fill(count_current.begin(), count_current.end(), 0);
    uncovered.InsertAll();
    ApplyMinMax();
  }

  void ShuffleOrder() {
    auto v = uncovered.List();
    nvector::Shuffle(v);
    uncovered.Clear();
    for (unsigned c : v) uncovered.Insert(c);
  }

  constexpr std::vector<size_t> SelectedRows() const { return selected_rows; }

 protected:
  void DisableNode(Node* node) {
    TBase::DisableNode(node);
    disabled_nodes.push(node);
  }

  void EnableLastNode() {
    assert(!disabled_nodes.empty());
    TBase::EnableNode(disabled_nodes.top());
    disabled_nodes.pop();
  }

  void DisableRow(size_t row) {
    auto node = headers_rows[row];
    while (node->r != node) DisableNode(node->r);
    DisableNode(node);
  }

  void DisableColumn(size_t column) {
    auto node = headers_columns[column];
    while (node->d != node) {
      DisableRow(node->d->row);
    }
    DisableNode(node);
  }

  void IncreaseCoveredColumn(size_t column) {
    auto cnt = ++count_current[column];
    if (cnt == count_min[column]) uncovered.Remove(column);
    if (cnt == count_max[column]) DisableColumn(column);
  }

  void DecreaseCoveredColumn(size_t column) {
    if (count_current[column]-- == count_min[column]) uncovered.Insert(column);
  }

 public:
  // TODO:
  // Better logic for column selection.
  size_t GetBestColumn() const {
    size_t best_count = TBase::nrows + 2;
    size_t best_column = missed;
    for (unsigned c : uncovered.List()) {
      assert(count_current[c] < count_min[c]);
      if (count_columns[c] + count_current[c] <= count_min[c]) return stop;
      unsigned d = count_columns[c] + count_current[c] - count_min[c];
      if (d < best_count) {
        best_count = d;
        best_column = c;
      }
    }
    return best_column;
  }

  void SelectRow(size_t row) {
    selected_rows.push_back(row);
    stack_sizes.push(disabled_nodes.size());
    auto h = headers_rows[row];
    for (auto node = h->r; node != h; node = h->r) {
      DisableNode(node);
      IncreaseCoveredColumn(node->column);
    }
    DisableNode(h);
  }

  void ReleaseLastRow() {
    unsigned row = selected_rows.back();
    for (; disabled_nodes.size() > stack_sizes.top();) EnableLastNode();
    stack_sizes.pop();
    selected_rows.pop_back();
    auto h = headers_rows[row];
    for (auto node = h->r; node != h; node = node->r) {
      DecreaseCoveredColumn(node->column);
    }
  }

  template <class TAlgorthimXCallBack>
  bool Search(TAlgorthimXCallBack& callback) {
    const size_t c = GetBestColumn();
    if (c == stop) return false;
    if (c == missed) return callback(selected_rows);
    auto h = headers_columns[c];
    for (auto n = h->d; n != h; n = n->d) {
      SelectRow(n->row);
      if (Search(callback)) return true;
      ReleaseLastRow();
      DisableRow(n->row);
    }
    return false;
  }

  bool SearchAny() {
    auto stop = [](const std::vector<size_t>&) { return true; };
    return Search(stop);
  }
};
}  // namespace cover
}  // namespace ds
