#include "message.h"
#include "stones.h"

#include "common/stl_base.h"

namespace {
	void SolveSmall_Stones() {
		int64_t l = GetNumberOfStones() + 1;
		int64_t jumps = 0, cur_best = 0, next_best = 0;
		for (int64_t i = 0; i < l; ++i)
		{
			if (i > cur_best)
			{
				++jumps;
				cur_best = next_best;
			}
			int64_t d = GetJumpLength(i);
			next_best = max(next_best, i + d);
		}
		if (cur_best < l)
			++jumps;
		cout << jumps << endl;
	}
}

int main_stones()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
	int64_t l = GetNumberOfStones() + 1;
	if (l <= 10 * nodes)
	{
		if (node_id == 0)
			SolveSmall_Stones();
		return 0;
	}
	int64_t first = (node_id * l) / nodes;
	int64_t last = ((node_id + 1) * l) / nodes;
	int64_t ln = last - first;

	int64_t next_best = first;
	vector<int> vj(ln, 0), vl(ln, 0), ve(ln, 0);
	for (int64_t i = first; i < last; ++i)
	{
		int64_t d = GetJumpLength(i);
		next_best = max(next_best, i + d);
		vl[i - first] = int(next_best);
	}
	for (int64_t i = last - 1; i >= first; --i)
	{
		int64_t j = vl[i - first];
		if (j >= last)
		{
			ve[i - first] = int(j);
			vj[i - first] = 1;
		}
		else
		{
			ve[i - first] = ve[j - first];
			vj[i - first] = vj[j - first] + 1;
		}
	}
	int64_t last_jumps = 0, last_pos0 = 0, last_pos1 = 0;
	if (node_id != 0)
	{
		Receive(node_id - 1);
		last_jumps = GetLL(node_id - 1);
		last_pos0 = GetLL(node_id - 1);
		last_pos1 = GetLL(node_id - 1);
	}
	if (last_pos0 < last)
	{
		if (last_pos1 > vl[last_pos0 - first])
		{
			++last_jumps;
			last_pos0 = last_pos1;
		}
		if (last_pos0 < last)
		{
			last_jumps += vj[last_pos0 - first];
			last_pos0 = ve[last_pos0 - first];
		}
		last_pos1 = max(last_pos0, int64_t(vl[last - 1 - first]));
	}
	else
	{
		if (last_pos1 < vl[last - 1 - first])
		{
			last_pos1 = vl[last - 1 - first];
		}
	}
	if (node_id != nodes - 1)
	{
		PutLL(node_id + 1, last_jumps);
		PutLL(node_id + 1, last_pos0);
		PutLL(node_id + 1, last_pos1);
		Send(node_id + 1);
	}
	else
	{
		cout << last_jumps << endl;
	}
	return 0;
}
