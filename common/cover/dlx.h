#pragma once

#include "common/base.h"
#include "common/cover/dlmatrix.h"
#include <stack>
#include <vector>

#include <iostream>

namespace cover {
class IDLXCallBack {
  // If output true, the main algorithm stops.
  // If output false, the main algirthm will search next solution.
  bool operator()(const std::vector<size_t>& rows);
};

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
    // std::cout << "Disabling node " << node << " " << node->row << " "
    //           << node->column << std::endl;
    TBase::DisableNode(node);
    disabled_nodes.push(node);
  }

  void EnableLastNode() {
    assert(!disabled_nodes.empty());
    auto node = disabled_nodes.top();
    // std::cout << "Enabling node " << node << " " << node->row << " "
    //           << node->column << std::endl;
    TBase::EnableNode(disabled_nodes.top());
    disabled_nodes.pop();
  }

  void DisableRow(size_t row) {
    // std::cout << "DisableRow(" << row << ")" << std::endl;
    auto node = headers_rows[row];
    while (node->r != node) DisableNode(node->r);
    DisableNode(node);
    // std::cout << "DisableRow finished" << std::endl;
  }

  void DisableCoveredColumn(size_t column) {
    // std::cout << "DisableCoveredColumn(" << column << ")" << std::endl;
    auto node = headers_columns[column];
    while (node->d != node) {
      DisableRow(node->d->row);
    }
    DisableNode(node);
    // std::cout << "DisableCoveredColumn finished" << std::endl;
  }

 public:
  Node* GetBestColumn() const {
    size_t best_count = TBase::nrows + 2;
    Node* best_node = nullptr;
    // std::cout << header << "\t" << header->r << std::endl;
    for (Node* n = header->r; n != header; n = n->r) {
      // std::cout << n << "\t" << n->r << std::endl;
      if (count_columns[n->column] < best_count) {
        best_count = count_columns[n->column];
        best_node = n;
      }
    }
    return best_node;
  }

  void SelectRow(size_t row) {
    // std::cout << "SelectRow(" << row << ")" << std::endl;
    selected_rows.push_back(row);
    stack_sizes.push(disabled_nodes.size());
    auto h = headers_rows[row];
    for (auto node = h->r; node != h; node = h->r) {
      DisableNode(node);
      DisableCoveredColumn(node->column);
    }
    DisableNode(h);
    // std::cout << "SelectRow finished" << std::endl;
  }

  void ReleaseLastRow() {
    // std::cout << "ReleaseLastRow()" << std::endl;
    for (; disabled_nodes.size() > stack_sizes.top();) EnableLastNode();
    stack_sizes.pop();
    selected_rows.pop_back();
    // std::cout << "ReleaseLastRow finished" << std::endl;
  }

  template <class TAlgorthimXCallBack>
  bool Search(TAlgorthimXCallBack& callback) {
    // std::cout << "Search called" << std::endl;
    auto h = GetBestColumn();
    if (h == nullptr) {
      // std::cout << "Search finished" << std::endl;
      return callback(selected_rows);
    }
    // std::cout << "Suggested column = " << h << " " << h->column << std::endl;
    for (auto n = h->d; n != h; n = n->d) {
      // std::cout << "Current node = " << n << " " << n->row << " " <<
      // n->column
      //           << std::endl;
      SelectRow(n->row);
      if (Search(callback)) return true;
      ReleaseLastRow();
    }
    // std::cout << "Search finished" << std::endl;
    return false;
  }

  bool SearchAny() {
    auto stop = [](const std::vector<size_t>& rows) { return true; };
    return Search(stop);
  }
};
}  // namespace cover
