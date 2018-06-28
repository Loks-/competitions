#pragma once

namespace {

// Swap intervals [first, middle) and [middle, last)
template<class TBST>
void Rotate(typename TBST::TNode*& root, unsigned first, unsigned middle, unsigned last)
{
	static_assert(!TBST::use_key, "use_key should be false");
	if ((middle <= first) || (middle >= last)) return;
	typename TBST::TNode *p1, *p2, *p3, *p4;
	TBST::SplitBySize(root, middle, p2, p3);
	TBST::SplitBySize(p2, first, p1, p2);
	TBST::SplitBySize(p3, last - middle, p3, p4);
	root = TBST::Merge(TBST::Merge(p1, p3), TBST::Merge(p2, p4));
}

// Rotate elements in interval [first, last) to right by shift
template<class TBST>
void RotateRight(typename TBST::TNode*& root, unsigned first, unsigned last, unsigned shift)
{
	assert(last > first);
	shift %= (last - first);
	if (shift == 0) return;
	return Rotate<TBST>(root, first, last - shift, last);
}

// Rotate elements in interval [first, last) to right by shift
template<class TBST>
void RotateLeft(typename TBST::TNode*& root, unsigned first, unsigned last, unsigned shift)
{
	assert(last > first);
	shift %= (last - first);
	if (shift == 0) return root;
	return Rotate<TBST>(root, first, first + shift, last);
}

} // namespace
