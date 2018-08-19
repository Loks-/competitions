// https://www.hackerrank.com/challenges/swap-nodes-algo

#include "common/stl_base.h"

namespace {
	static vector<int> vl, vr, vd;

	static void InitD(int c, int cd)
	{
		if (c != -1)
		{
			vd[c] = cd;
			InitD(vl[c], cd + 1);
			InitD(vr[c], cd + 1);
		}
	}

	static void Print(int c)
	{
		if (c != -1)
		{
			Print(vl[c]);
			cout << c << " ";
			Print(vr[c]);
		}
	}
}

int main_swap_nodes_algo() {
	int N;
	cin >> N;
	vl.resize(N + 1, -1);
	vr.resize(N + 1, -1);
	vd.resize(N + 1, -1);
	vector<bool> vh(N + 1, true);
	for (int i = 1; i <= N; ++i)
	{
		int l, r;
		cin >> l >> r;
		vl[i] = l;
		vr[i] = r;
		if (l != -1) vh[l] = false;
		if (r != -1) vh[r] = false;

	}
	int h = 0;
	for (int i = 1; i < N; ++i)
	{
		if (vh[i])
		{
			h = i;
			break;
		}
	}
	InitD(h, 1);
	int T;
	cin >> T;
	for (; T; --T)
	{
		int d;
		cin >> d;
		for (int i = 1; i <= N; ++i)
		{
			if ((vd[i] % d) == 0)
			{
				swap(vl[i], vr[i]);
			}
		}
		Print(h);
		cout << endl;
	}
	return 0;
}
