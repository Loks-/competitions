#pragma once

#include "common/base.h"
#include "common/hash.h"
#include "common/timer.h"

#include "tester/assert.h"
#include "tester/binary_search_tree_action.h"
#include "tester/binary_search_tree_info.h"

#include <iostream>
#include <stack>
#include <string>
#include <vector>

class TesterBinarySearchTree {
 public:
  using TKey = uint64_t;

  enum TBSTKeysType {
    sorted = 0,
    reversed = 1,
    shuffled = 2,
    shuffled_dups = 3,
    one_value = 4,
    sixteen_values = 5,
    type_end = 6
  };

  enum TBSTMode { hash_test, time_test };

 protected:
  TBSTMode mode;
  std::vector<TKey> keys_sorted, keys_reversed, keys_shuffled,
      keys_shuffled_dups, keys_one_value, keys_sixteen_values;
  std::vector<std::vector<TKey>*> keys;

  const std::vector<TKey>& GetKeys(TBSTKeysType type) const {
    Assert(type < type_end);
    return *keys[type];
  }

  unsigned Size() const { return unsigned(keys_sorted.size()); }

  class Result {
   public:
    std::string job;
    std::string tree;
    std::string task;
    unsigned keys_type;
    size_t h;
    size_t time;
  };

  std::string current_job, current_tree;
  std::vector<Result> results;

  void AddResult(const std::string& task, unsigned keys_type, size_t h,
                 size_t time);
  void AddMax();
  bool TestHash() const;
  void PrintTime() const;

 public:
  TesterBinarySearchTree(unsigned size, TBSTMode _mode);

 protected:
  template <class TNode>
  size_t TreeHash(TNode* root, size_t h) {
    if (!root) return h;
    h = TreeHash(root->l, h);
    h = hash_combine(h, root->key);
    return TreeHash(root->r, h);
  }

  template <class TNode>
  void VerifyParentLinksI(TNode* root, TFakeFalse) {}

  template <class TNode>
  void VerifyParentLinksI(TNode* root, TFakeTrue f) {
    std::stack<TNode*> s;
    for (s.push(root); !s.empty();) {
      root = s.top();
      s.pop();
      if (root->l) {
        s.push(root->l);
        Assert(root->l->p == root, "Parent link is incorrect!");
      }
      if (root->r) {
        s.push(root->r);
        Assert(root->r->p == root, "Parent link is incorrect!");
      }
    }
  }

  template <class TNode>
  void VerifyParentLinks(TNode* root) {
    if (mode == hash_test)
      VerifyParentLinksI(root, TFakeBool<TNode::use_parent>());
  }

  template <class TNode>
  void VerifyParentLinksLazy(TNode* root) {
    if (root && !(root->info.size & (root->info.size - 1)))
      VerifyParentLinks(root);
  }

  template <class TTree>
  typename TTree::TNode* TestBuild(TTree& tree, TBSTKeysType type) {
    Timer t;
    typename TTree::TNode* root = tree.Build(GetKeys(type), GetKeys(type));
    t.Stop();
    AddResult("Build", type, TreeHash(root, 0), t.GetMilliseconds());
    VerifyParentLinks(root);
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestInsert(TTree& tree, TBSTKeysType type) {
    const std::vector<TKey>& vkeys = GetKeys(type);
    Timer t;
    typename TTree::TNode* root = 0;
    size_t h = 0;
    for (TKey key : vkeys) {
      AddAction(root);
      root = tree.InsertNewNode(root, key, key);
      VerifyParentLinksLazy(root);
      h = hash_combine(h, GetInfoValue(root));
    }
    AddResult("Insert", type, h, t.GetMilliseconds());
    VerifyParentLinks(root);
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestFindByOrder(TTree& tree,
                                         typename TTree::TNode* root,
                                         TBSTKeysType type) {
    Timer t;
    size_t h = 0;
    for (unsigned i = 0; i < Size(); ++i) {
      typename TTree::TNode* node = tree.FindByOrder(root, i);
      Assert(node);
      h = hash_combine(h, node->key);
    }
    AddResult("FindO", type, h, t.GetMilliseconds());
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestFindByKey0(TTree& tree,
                                        typename TTree::TNode* root,
                                        TBSTKeysType type) {
    Timer t;
    size_t h = 0;
    for (unsigned i = 0; i <= Size(); ++i) {
      typename TTree::TNode* node = tree.FindByKey(root, 2 * i);
      Assert(!node);
      h = hash_combine(h, reinterpret_cast<size_t>(node));
    }
    AddResult("FindK0", type, h, t.GetMilliseconds());
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestFindByKey1(TTree& tree,
                                        typename TTree::TNode* root,
                                        TBSTKeysType type) {
    const std::vector<TKey>& vkeys = GetKeys(type);
    Timer t;
    size_t h = 0;
    for (const TKey& key : vkeys) {
      typename TTree::TNode* node = tree.FindByKey(root, key);
      Assert(node);
      h = hash_combine(h, (type <= shuffled) ? node->data : node->key);
    }
    AddResult("FindK1", type, h, t.GetMilliseconds());
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestDeleteByKey(TTree& tree,
                                         typename TTree::TNode* root,
                                         TBSTKeysType type) {
    const std::vector<TKey>& vkeys = GetKeys(type);
    Timer t;
    size_t h = 0;
    for (const TKey& key : vkeys) {
      AddAction(root);
      root = tree.RemoveAndReleaseByKey(root, key);
      VerifyParentLinksLazy(root);
      h = hash_combine(h, (type <= shuffled) ? GetInfoValue(root)
                                             : root ? root->info.size : 0);
    }
    AddResult("DelKey", type, h, t.GetMilliseconds());
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestDeleteByNodeI(TTree& tree,
                                           typename TTree::TNode* root,
                                           TBSTKeysType type, TFakeFalse) {
    tree.ReleaseTree(root);
    return 0;
  }

  template <class TTree>
  typename TTree::TNode* TestDeleteByNodeI(TTree& tree,
                                           typename TTree::TNode* root,
                                           TBSTKeysType type, TFakeTrue) {
    Timer t;
    size_t h = 0;
    for (unsigned i = 0; i < Size(); ++i) {
      AddAction(root);
      root = tree.RemoveAndReleaseByNode(tree.NodeByRawIndex(i));
      VerifyParentLinksLazy(root);
      h = hash_combine(h, GetInfoValue(root));
    }
    AddResult("DelNode", type, h, t.GetMilliseconds());
    return root;
  }

  template <class TTree>
  typename TTree::TNode* TestDeleteByNode(TTree& tree,
                                          typename TTree::TNode* root,
                                          TBSTKeysType type) {
    return TestDeleteByNodeI(tree, root, type, TFakeBool<TTree::use_parent>());
  }

  template <class TTree>
  void TestAll(const std::string& tree_name) {
    std::cout << "\tTesting " << tree_name << ":" << std::endl;
    current_tree = tree_name;
    typename TTree::TNode* root = 0;
    TTree tree(Size());
    for (unsigned type = 0; type < type_end; ++type) {
      TBSTKeysType ktype = TBSTKeysType(type);
      root = TestBuild<TTree>(tree, ktype);
      root = TestDeleteByNode(tree, root, ktype);
      // tree.ReleaseTree(root); root = 0;
      Assert(!root);
      root = TestInsert<TTree>(tree, ktype);
      root = TestFindByOrder<TTree>(tree, root, ktype);
      root = TestFindByKey0<TTree>(tree, root, ktype);
      root = TestFindByKey1<TTree>(tree, root, ktype);
      root = TestDeleteByKey<TTree>(tree, root, ktype);
      // tree.ReleaseTree(root); root = 0;
      Assert(!root);
      tree.ResetNodes();
    }
  }

 public:
  bool TestAllTrees();
};
