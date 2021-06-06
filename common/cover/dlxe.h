#pragma once

#include "common/base.h"
#include "common/cover/dlmatrix.h"

#include <stack>
#include <utility>
#include <vector>

namespace cover {
// Extended DLX with support integer cover (min and max) for each column.
class DLXE : public DLMatrix {
 public:
  using TBase = DLMatrix;
  using Node = TBase::Node;

 protected:
  std::vector<size_t> selected_rows;
  std::stack<Node*> disabled_nodes;
  std::stack<size_t> stack_sizes;
  std::vector<unsigned> count_min, count_max, count_current;

 public:
  void Clear() {
    selected_rows.clear();
    std::stack<Node*>().swap(disabled_nodes);
    std::stack<size_t>().swap(stack_sizes);
    TBase::Clear();
  }

  void Init(size_t rows, size_t columns) {
    Clear();
    TBase::Init(rows, columns);
    count_min = count_max = std::vector<unsigned>(columns, 1);
    count_current = std::vector<unsigned>(columns, 0);
  }

  // Should be called after matrix is initialized
  void SetColumnsMinMax(const std::vector<unsigned>& cmin,
                        const std::vector<unsigned>& cmax) {
    assert((cmin.size() == TBase::ncolumns) &&
           (cmax.size() == TBase::ncolumns));
    count_min = cmin;
    count_max = cmax;
    for (unsigned i = 0; i < count_max.size(); ++i) {
      if (count_max[i] == count_current[i]) DisableColumn(i);
    }
  }

  void ResetSearch() {
    selected_rows.clear();
    while (!disabled_nodes.empty()) EnableLastNode();
    std::stack<size_t>().swap(stack_sizes);
  }

  const std::vector<size_t> SelectedRows() const { return selected_rows; }

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
    if (++count_current[column] == count_max[column]) {
      DisableColumn(column);
    }
  }

  void DecreaseCoveredColumn(size_t column) { --count_current[column]; }

 public:
  // TODO:
  // Better logic for column selection.
  std::pair<bool, Node*> GetBestColumn() const {
    size_t best_count = TBase::nrows + 2;
    Node* best_node = nullptr;
    for (Node* n = header->r; n != header; n = n->r) {
      if ((count_columns[n->column] + count_current[n->column] <=
           count_min[n->column]) ||
          (count_current[n->column] > count_max[n->column]))
        return {false, nullptr};
      if (count_current[n->column] >= count_min[n->column]) continue;
      unsigned d = count_columns[n->column] + count_current[n->column] -
                   count_min[n->column];
      if (d < best_count) {
        best_count = d;
        best_node = n;
      }
    }
    return {true, best_node};
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
    auto p = GetBestColumn();
    if (!p.first) return false;
    auto h = p.second;
    if (h == nullptr) return callback(selected_rows);
    if (count_columns[h->column] + count_current[h->column] <=
        count_min[h->column])
      return false;
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
