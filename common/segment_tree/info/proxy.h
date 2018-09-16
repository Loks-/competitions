#pragma once

#include "../../base.h"

template<class TTBase>
class STInfoProxy : public TTBase
{
public:
    using TBase = TTBase;
	using TSelf = STInfoProxy<TBase>;

    template<class TNode>
	void Update(TNode* node)
    {
        assert(node);
        node->ApplyAction();
        if (node->IsLeaf())
            TBase::UpdateData(node->data);
        else
            TBase::UpdateLR(node->l->info, node->r->info);
    }

    static TSelf MergeLR(const TSelf& l, const TSelf& r)
    {
        TSelf t;
        t.UpdateLR(l, r);
        return t;
    }
};
