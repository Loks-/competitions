#pragma once

template<class TNode, class TCoordinate>
inline TNode* CSTFind(TNode* root, const TCoordinate& key)
{
    using TInfo = typename TNode::TInfo;
	static_assert(TInfo::has_segment, "has_segment should be true");
	for (TNode * node = root; node != 0; )
	{
		node->ApplyAction();
        if (node->IsLeaf())
        {
            if ((node->info.xleft <= key) && (key <= node->info.xright))
                return node;
            else
                return 0;
        }
		else if (node->r->info.xleft <= key)
			node = node->r;
		else if (key <= node->l->info.xright)
			node = node->l;
		else
			return node;
	}
	return 0;
}
