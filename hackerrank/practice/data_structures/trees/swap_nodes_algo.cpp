// https://www.hackerrank.com/challenges/swap-nodes-algo

#include "common/stl/base.h"

#include <functional>

int main_swap_nodes_algo()
{
	unsigned N;
	cin >> N;
	vector<int> vl(N + 1, -1), vr(N + 1, -1), vd(N + 1, -1);
	vector<bool> vh(N + 1, true);
	for (unsigned i = 1; i <= N; ++i)
	{
		int l, r;
		cin >> l >> r;
		vl[i] = l;
		vr[i] = r;
		if (l != -1) vh[l] = false;
		if (r != -1) vh[r] = false;
	}
	unsigned h = 0;
	for (unsigned i = 1; i < N; ++i)
	{
		if (vh[i])
		{
			h = i;
			break;
		}
	}

    std::function<void(int, int, vector<int>&)> InitD = [vl, vr, &InitD](int c, int cd, vector<int>& vd) -> void
	{
		if (c != -1)
		{
			vd[c] = cd;
			InitD(vl[c], cd + 1, vd);
			InitD(vr[c], cd + 1, vd);
		}
	};

    std::function<void(int, const vector<int>&, const vector<int>&)> Print = [&Print](int c, const vector<int>& vl, const vector<int>& vr) -> void
	{
		if (c != -1)
		{
			Print(vl[c], vl, vr);
			cout << c << " ";
			Print(vr[c], vl, vr);
		}
	};

	InitD(h, 1, vd);
	unsigned T;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
		int d;
		cin >> d;
		for (unsigned i = 1; i <= N; ++i)
		{
			if ((vd[i] % d) == 0)
				swap(vl[i], vr[i]);
		}
		Print(h, vl, vr);
		cout << endl;
	}
	return 0;
}
