#include "message.h"

#include "common/base.h"

namespace
{
#include "number_bases.h"
}

int main_number_bases()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
    int64_t length = GetLength();

    int64_t ifirst = node_id * length / nodes;
	int64_t ilast = (node_id + 1) * length / nodes;

    int64_t b0 = 2, b1 = 1000000000, x, y, z, s = 0;
    if (ifirst > 0)
    {
        x = GetDigitX(ifirst - 1);
        y = GetDigitY(ifirst - 1);
        z = GetDigitZ(ifirst - 1);
        if (z < x + y)
            s = 1;
    }
    for (int64_t i = ifirst; i < ilast; ++i)
    {
        x = GetDigitX(i);
        y = GetDigitY(i);
        z = GetDigitZ(i);
        b0 = max(b0, x + 1);
        b0 = max(b0, y + 1);
        b0 = max(b0, z + 1);
        if (z > x + y + s)
        {
            b1 = 1; break;
        }
        else if (z == x + y + s)
        {
            s = 0;
        }
        else
        {
            b0 = max(b0, x + y + s - z);
            b1 = min(b1, x + y + s - z);
            s = 1;
        }
    }
    if ((ilast == length) && (s == 1))
        b1 = 1;

    if (node_id == 0)
    {
        for (int i = 1; i < nodes; ++i)
        {
            Receive(i);
            b0 = max(b0, int64_t(GetLL(i)));
            b1 = min(b1, int64_t(GetLL(i)));
        }
        cout << ((b1 < b0) ? "IMPOSSIBLE" : (b1 > b0) ? "NON-UNIQUE" : to_string(b0)) << endl;
    }
    else
    {
        PutLL(0, b0);
        PutLL(0, b1);
        Send(0);
    }

    return 0;
}
