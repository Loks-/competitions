#pragma once

#include "common/base.h"
#include "common/data_structures/cover/dlmatrix.h"

#include <stack>
#include <vector>

namespace ds {
namespace cover {
class DLX : public DLMatrix {
 public:
  using TBase = DLMatrix;
  using Node = TBase::Node;

 protected:
  std::vector<size_t> selected_rows;
  std::stack<Node*> disabled_nodes;
  std::stack<size_t> stack_sizes;

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

  void DisableCoveredColumn(size_t column) {
    auto node = headers_columns[column];
    while (node->d != node) {
      DisableRow(node->d->row);
    }
    DisableNode(node);
  }

 public:
  Node* GetBestColumn() const {
    size_t best_count = TBase::nrows + 2;
    Node* best_node = nullptr;
    for (Node* n = header->r; n != header; n = n->r) {
      if (count_columns[n->column] < best_count) {
        best_count = count_columns[n->column];
        best_node = n;
      }
    }
    return best_node;
  }

  void SelectRow(size_t row) {
    selected_rows.push_back(row);
    stack_sizes.push(disabled_nodes.size());
    auto h = headers_rows[row];
    for (auto node = h->r; node != h; node = h->r) {
      DisableNode(node);
      DisableCoveredColumn(node->column);
    }
    DisableNode(h);
  }

  void ReleaseLastRow() {
    for (; disabled_nodes.size() > stack_sizes.top();) EnableLastNode();
    stack_sizes.pop();
    selected_rows.pop_back();
  }

  template <class TAlgorthimXCallBack>
  bool Search(TAlgorthimXCallBack& callback) {
    auto h = GetBestColumn();
    if (h == nullptr) return callback(selected_rows);
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
