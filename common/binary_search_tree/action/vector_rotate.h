#pragma once

#include "none.h"

// Support only VectorStaticSize from LinearAlgebra.
class BSTActionVectorRotate : public BSTActionNone
{
protected:
	template<class TVector>
	static void Rotate(TVector& v, unsigned r)
	{
		if (r == 0) return;
		static thread_local TVector vt;
		unsigned size = v.Size();
		for (unsigned i = 0; i < size; ++i)
			vt((i + r) % size) = v(i);
		v.swap(vt);
	}

public:
	using TBase = BSTActionNone;
	using TSelf = BSTActionVectorRotate;
	static const bool is_none = false;

	unsigned r;

	BSTActionVectorRotate() : r(0) {}
	bool IsEmpty() const { return r == 0; }
	void Clear() { r = 0; };

	template<class TNode>
	void Add(TNode* node, unsigned _r)
	{
		Rotate(node->info.sum, _r);
		r = (r + _r) % node->info.sum.Size();
	}

	template<class TNode>
	void Apply(TNode* node)
	{
		if (IsEmpty()) return;
		Rotate(node->data, r);
		if (node->l) node->l->AddAction(r);
		if (node->r) node->r->AddAction(r);
		r = 0;
	}
};
