#include "message.h"
#include "flagpoles.h"

#include "common/base.h"

namespace
{
	void solve_small_flagpoles()
	{
		int l = int(GetNumFlagpoles());
		int best = 0, current_line = 1;
		int64_t last = GetHeight(0), lastd = 0;
		for (int i = 1; i < l; ++i)
		{
			int64_t current = GetHeight(i);
			if (current != last + lastd)
			{
				best = max(best, current_line);
				current_line = 1;
				lastd = current - last;
			}
			last = current;
			++current_line;
		}
		best = max(best, current_line);
		cout << best << endl;
	}
}

int main_flagpoles()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
	int64_t length = GetNumFlagpoles();
	if ((nodes == 1) || (length <= 2 * nodes))
	{
		if (node_id == 0)
			solve_small_flagpoles();
		return 0;
	}

	int64_t ifirst = node_id * length / nodes;
	int64_t ilast = (node_id + 1) * length / nodes;
	assert(ilast - ifirst >= 2);
	int64_t h0 = GetHeight(ifirst);
	int64_t h1 = GetHeight(ifirst + 1);
	int64_t dh0 = h1 - h0;
	bool first = true;
	int64_t best = 2, current_line = 2, last = h1, dh = dh0, current_line_left = 2;
	for (int64_t i = ifirst + 2; i < ilast; ++i)
	{
		int64_t current = GetHeight(i);
		if (current != last + dh)
		{
			if (first)
			{
				current_line_left = current_line;
				first = false;
			}
			best = max(best, current_line);
			current_line = 1;
			dh = current - last;
		}
		last = current;
		++current_line;
	}
	best = max(best, current_line);

	if (node_id != 0)
	{
		Receive(node_id - 1);
		int64_t lbest = GetLL(node_id - 1);
		int64_t llast = GetLL(node_id - 1);
		int64_t ldh = GetLL(node_id - 1);
		int64_t lcl = GetLL(node_id - 1);
		if (first && (llast + ldh == h0) && (ldh == dh0))
		{
			// Same line
			current_line += lcl;
		}
		else
		{
			if (first && (llast + dh0 == h0))
			{
				current_line += 1;
			}
			if ((llast + ldh == h0) && (ldh == dh0))
			{
				lcl += current_line_left;
				best = max(best, lcl);
			}
			else if (llast + ldh == h0)
			{
				lcl += 1;
				best = max(best, lcl);
			}
		}
		best = max(best, current_line);
		best = max(best, lbest);
	}
	if (node_id != nodes - 1)
	{
		PutLL(node_id + 1, best);
		PutLL(node_id + 1, last);
		PutLL(node_id + 1, dh);
		PutLL(node_id + 1, current_line);
		Send(node_id + 1);
	}
	else
	{
		cout << best << endl;
	}
	return 0;
}
