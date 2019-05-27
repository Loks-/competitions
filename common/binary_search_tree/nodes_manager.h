// #pragma once

// #include "common/base.h"
// #include "common/template.h"
// #include <random>

// namespace bst {
// template <class TTNode, template <class> class TNodesManager>
// class NodesManager : public TNodesManager<TTNode> {
//  public:
//   using TNode = TTNode;
//   using TBase = TNodesManager<TNode>;
//   using TData = typename TNode::TData;
//   using TKey = typename TNode::TKey;

//   static const bool use_key = TNode::use_key;
//   static const bool use_height = TNode::use_height;

//  protected:
//   std::minstd_rand random_engine;

//  public:
//   NodesManager(unsigned reserve_nodes) : TBase(max_nodes) {}

//  protected:
//   void InitRandomHeightI(TNode* p, TFakeFalse) {}
//   void InitRandomHeightI(TNode* p, TFakeTrue) { p->height = random_engine();
//   }

//  public:
//   void InitRandomHeight(TNode* p) {
//     InitRandomHeightI(p, TFakeBool<use_height>());
//   }

//   TNode* New() { return TBase::New(); }

//   TNode* New(const TData& data) {
//     auto p = New();
//     p->data = data;
//     p->UpdateInfo();
//     return p;
//   }

//   TNode* New(const TData& data, const TKey& key) {
//     static_assert(use_key, "use_key should be true");
//     auto p = New();
//     p->data = data;
//     p->key = key;
//     p->UpdateInfo();
//     return p;
//   }

//   void ReleaseNode(TNode* p) { released_nodes.push(p); }

//   unsigned UsedNodes() const {
//     return used_nodes - unsigned(released_nodes.size());
//   }

//   unsigned AvailableNodes() const {
//     return unsigned(nodes.size()) - UsedNodes();
//   }

//   void ReserveAvailableNodes(unsigned new_nodes) {
//     if (AvailableNodes() < new_nodes) {
//       Reserve(UsedNodes() + new_nodes);
//     }
//   }

//   TNode* GetNodeByRawIndex(unsigned index) { return &(nodes[index]); }

//   void ResetNodes() {
//     std::stack<TNode*>().swap(released_nodes);
//     used_nodes = 0;
//     for (TNode& node : nodes) {
//       node.ResetLinks();
//       node.ClearAction();
//     }
//   }
// };
// }  // namespace bst
