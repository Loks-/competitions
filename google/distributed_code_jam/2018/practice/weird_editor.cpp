#include "message.h"

#include "common/base.h"
#include "common/numeric/modular.h"

namespace
{
#include "weird_editor.h"
}

using TModular = Modular<>;

int main_weird_editor()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
    int length = int(GetNumberLength());

    vector<vector<int>> v(10);
    for (int i = node_id; i < length; i += nodes)
    {
        int digit = int(GetDigit(i));
        v[digit].push_back(i);
    }
    vector<int> vc(10);

    int start_pos = -1;
    for (int digit = 9; digit > 0; --digit)
    {
        int l = int(v[digit].end() - lower_bound(v[digit].begin(), v[digit].end(), start_pos));
        int final_pos = ((l > 0) ? v[digit].back() : start_pos);
        if (node_id == 0)
        {
            for (int inode = 1; inode < nodes; ++inode)
            {
                Receive(inode);
                l += GetInt(inode);
                final_pos = max(final_pos, GetInt(inode));                
            }
            vc[digit] = l;
            start_pos = final_pos;
            for (int inode = 1; inode < nodes; ++inode)
            {
                PutInt(inode, start_pos);
                Send(inode);
            }
        }
        else
        {
            PutInt(0, l);
            PutInt(0, final_pos);
            Send(0);
            Receive(0);
            start_pos = GetInt(0);
        }
    }

    if (node_id == 0)
    {
        TModular f, one(1), nine(9), ten(10);
        for (int digit = 9; digit > 0; --digit)
        {
            length -= vc[digit];
            f += ten.PowU(length) * (ten.PowU(vc[digit]) - one) / nine * TModular(digit);
        }
        cout << f.Get() << endl;
    }

    return 0;
}
