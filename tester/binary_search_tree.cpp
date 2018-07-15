#pragma once

#include "common/binary_search_tree/treap.h"

#include "binary_search_tree.h"

TesterBinarySearchTree::TesterBinarySearchTree(unsigned size)
{
	keys.resize(order_end);
	keys_sorted.resize(size);
	for (unsigned i = 0; i < size; ++i)
		keys_sorted[i] = 2 * i + 1;
	keys[sorted] = &keys_sorted;
	keys_reversed = keys_sorted;
	reverse(keys_reversed.begin(), keys_reversed.end());
	keys[reversed] = &keys_reversed;
	keys_shuffled = keys_sorted;
	random_shuffle(keys_shuffled.begin(), keys_shuffled.end());
	keys[shuffled] = &keys_shuffled;
}

void TesterBinarySearchTree::TestAllTrees()
{
	cout << "Testing base trees:" << endl;
	TestAll<Treap<true, false, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upf");
	TestAll<Treap<true, true, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upt");
}

void TesterBinarySearchTree::Test()
{
	TesterBinarySearchTree tbst(100000);
	tbst.TestAllTrees();
}
