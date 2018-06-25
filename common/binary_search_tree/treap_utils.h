#pragma once

namespace {

// Swap intervals [first, middle) and [middle, last)
template<class TTreap>
void Rotate(typename TTreap::TNode*& root, unsigned first, unsigned middle, unsigned last)
{
	static_assert(!TTreap::use_key, "use_key should be false");
	if ((middle <= first) || (middle >= last)) return;
	typename TTreap::TNode *p1, *p2, *p3, *p4;
	TTreap::SplitBySize(root, middle, p2, p3);
	TTreap::SplitBySize(p2, first, p1, p2);
	TTreap::SplitBySize(p3, last - middle, p3, p4);
	root = TTreap::Merge(TTreap::Merge(p1, p3), TTreap::Merge(p2, p4));
}

// Rotate elements in interval [first, last) to right by shift
template<class TTreap>
void RotateRight(typename TTreap::TNode*& root, unsigned first, unsigned last, unsigned shift)
{
	assert(last > first);
	shift %= (last - first);
	if (shift == 0) return;
	return Rotate(root, first, last - shift, last);
}

// Rotate elements in interval [first, last) to right by shift
template<class TTreap>
void RotateLeft(typename TTreap::TNode*& root, unsigned first, unsigned last, unsigned shift)
{
	assert(last > first);
	shift %= (last - first);
	if (shift == 0) return root;
	return Rotate(root, first, first + shift, last);
}

} // namespace
