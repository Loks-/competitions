// !!! UNTESTED !!!

#include "message.h"
#include "kenneth.h"

#include "common/base.h"
#include "common/numeric/modular.h"
#include "common/numeric/primes_list.h"

using TModular = Modular<>;

int main_kenneth()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();

	vector<int64_t> vs(nodes + 1);
	vs[0] = 0;
	for (int i = 0; i < nodes; ++i)
	{
		vs[i + 1] = vs[i] + GetPieceLength(i);
	}
	int64_t first = vs[node_id], last = vs[node_id + 1], ln = last - first;

	vector<char> vstr(ln);
	for (int64_t i = first; i < last; ++i)
	{
		vstr[i - first] = GetSignalCharacter(i);
	}
	vector<TModular> vh(ln + 1), vmi(ln);
	TModular mm = TModular(0x9e3779b9), mmi = mm.Inverse();
	TModular mb = mm.PowU(vs[node_id]), mc = mb, mci = mc.Inverse();
	vh[0] = 0;
	for (int64_t i = 0; i < ln; ++i)
	{
		vmi[i] = mci;
		vh[i + 1] = vh[i] + mc * TModular(vstr[i]);
		mc *= mm;
		mci *= mmi;
	}

	if (node_id == 0)
	{
		PrimesList pl(1000000);
		auto vp = pl.GetDivisors(vs.back());
		for (int64_t d : vp)
		{
			for (int in = 1; in < nodes; ++in)
			{
				PutLL(in, d);
				Send(in);
			}
			int64_t status = -1, i = 0;
			for (; i + d <= last; i += d)
			{
				int64_t t = ((vh[i + d] - vh[i]) * vmi[i]).Get();
				if (status == -1)
					status = t;
				else if (t != status)
				{
					status = -2;
					break;
				}
			}
			PutLL(1, status);
			PutLL(1, (vh.back() - vh[i]).Get());
			Send(1);
			// ...
			Receive(nodes - 1);
			status = GetLL(nodes - 1);
			i = GetLL(nodes - 1);
			if (status != -1)
			{
				cout << d << endl;
				for (int in = 1; in < nodes; ++in)
				{
					PutLL(in, -1);
					Send(in);
				}
				return 0;
			}
		}
	}
	else
	{
		for (;;)
		{
			Receive(0);
			int64_t d = GetLL(0);
			if (d < 0)
				return 0;
			int64_t status = -1;
			int64_t ifirst = (first / d) * d;
			if (ifirst < first)
				ifirst += d;
			int64_t current = ifirst;
			for (; current + d <= last; current += d)
			{
				int64_t t = ((vh[current + d - first] - vh[current - first]) * vmi[current - first]).Get();
				if (status == -1)
					status = t;
				else if (t != status)
				{
					status = -2;
					break;
				}
			}
			Receive(node_id - 1);
			int64_t sl = GetLL(node_id - 1);
			int64_t cl = GetLL(node_id - 1);
			if (status == -1)
				status = sl;
			if ((sl != -1) && (sl != status))
			{
				status = -2;
			}
			if (status != -2)
			{
				if (ifirst < last)
				{
					if (ifirst != first)
					{
						int64_t t = ((TModular(cl) + vh[ifirst - first]) * mmi.PowU(ifirst - d)).Get();
						if (status == -1)
							status = t;
						else if (t != status)
							status = -2;
					}
					cl = (vh.back() - vh[current]).Get();
				}
				else
				{
					cl = (TModular(cl) + vh.back()).Get();
				}
			}
			PutLL((node_id + 1) % nodes, status);
			PutLL((node_id + 1) % nodes, cl);
			Send((node_id + 1) % nodes);
		}
	}
	return 0;
}
