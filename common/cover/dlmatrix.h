#pragma once

#include "common/base.h"
#include <deque>
#include <vector>

namespace cover {
class DLMatrix {
 public:
  class Node {
   public:
    Node *l, *r, *u, *d;
    size_t row;
    size_t column;

    Node(size_t row_, size_t column_) : row(row_), column(column_) {}

    void ResetLinks() { l = r = u = d = this; }
  };

 protected:
  size_t nrows;
  size_t ncolumns;
  std::deque<Node> nodes;
  std::vector<Node*> headers_rows;
  std::vector<Node*> headers_columns;
  std::vector<size_t> count_rows;
  std::vector<size_t> count_columns;
  Node* header;

 protected:
  Node* NewNode(size_t row, size_t column) {
    nodes.push_back(Node(row, column));
    nodes.back().ResetLinks();
    return &(nodes.back());
  }

 public:
  void Clear() {
    nrows = 0;
    ncolumns = 0;
    nodes.clear();
    headers_rows.clear();
    headers_columns.clear();
    count_rows.clear();
    count_columns.clear();
    header = nullptr;
  }

  void Init(size_t rows, size_t columns) {
    Clear();
    nrows = rows;
    ncolumns = columns;
    header = NewNode(nrows, ncolumns);
    for (size_t i = 0; i < nrows; ++i)
      headers_rows.push_back(NewNode(i, ncolumns));
    headers_rows.push_back(header);
    for (size_t i = 0; i <= nrows; ++i) {
      size_t j = (i + 1) % (nrows + 1);
      headers_rows[i]->d = headers_rows[j];
      headers_rows[j]->u = headers_rows[i];
    }
    for (size_t i = 0; i < ncolumns; ++i)
      headers_columns.push_back(NewNode(nrows, i));
    headers_columns.push_back(header);
    for (size_t i = 0; i <= ncolumns; ++i) {
      size_t j = (i + 1) % (ncolumns + 1);
      headers_columns[i]->r = headers_columns[j];
      headers_columns[j]->l = headers_columns[i];
    }
    count_rows.resize(nrows + 1, 1);
    count_rows.back() = ncolumns + 1;
    count_columns.resize(ncolumns + 1, 1);
    count_columns.back() = nrows + 1;
  }

  void DisableNode(Node* node) {
    node->l->r = node->r;
    node->r->l = node->l;
    node->u->d = node->d;
    node->d->u = node->u;
    --count_rows[node->row];
    --count_columns[node->column];
  }

  void EnableNode(Node* node) {
    node->l->r = node;
    node->r->l = node;
    node->u->d = node;
    node->d->u = node;
    ++count_rows[node->row];
    ++count_columns[node->column];
  }

  void Add(size_t row, size_t column) {
    assert((row < nrows) && (column < ncolumns));
    auto node = NewNode(row, column);
    auto hrow = headers_rows[row], hcolumn = headers_columns[column];
    node->r = hrow;
    node->l = hrow->l;
    node->d = hcolumn;
    node->u = hcolumn->u;
    EnableNode(node);
  }

  Node* Find(size_t row, size_t column) const {
    if (row > nrows) return nullptr;
    auto h = headers_rows[row];
    if (h->column == column) return h;
    for (auto node = h->r; node != h; node = node->r) {
      if (node->column == column) return node;
    }
    return nullptr;
  }

  void Remove(size_t row, size_t column) {
    auto node = Find(row, column);
    if (node) DisableNode(node);
  }
};
}  // namespace cover
