#include "message.h"

#include "common/base.h"

namespace
{
#include "towels.h"
}

int main_towels()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
    int64_t stacks = GetNumberOfStacks();
    int64_t towels_per_stack = GetNumberOfTowelsInStack();
    int64_t towels = stacks * towels_per_stack;
    int64_t index = GetNumberOfGymUsersBeforeYou();

    int64_t ifirst = node_id * stacks / nodes;
	int64_t ilast = (node_id + 1) * stacks / nodes;

    // Read data
    vector<vector<int64_t>> vtowels(ilast - ifirst, vector<int64_t>(towels_per_stack));
    vector<int64_t> vtowels_merged;
    vtowels_merged.reserve((ilast - ifirst) * towels_per_stack);
    for (int64_t i = 0; i < ilast - ifirst; ++i)
    {
        int64_t current_max = 0;
        for (int64_t j = 0; j < towels_per_stack; ++j)
        {
            int64_t value = GetTowelCleanlinessRank(i + ifirst, towels_per_stack - j - 1);
            current_max = max(current_max, value);
            vtowels[i][j] = current_max;
            vtowels_merged.push_back(current_max);
        }
    }
    sort(vtowels_merged.begin(), vtowels_merged.end());

    // Binary search
    int64_t a = 1, at = 0, b = towels + 1;
    for (int current_master = 0; b - a > 1; current_master = (current_master + 1) % nodes)
    {
        int64_t c = (a + b) / 2;
        int64_t local = lower_bound(vtowels_merged.begin(), vtowels_merged.end(), c) - vtowels_merged.begin(), global;
        if (current_master == node_id)
        {
            global = local;
            for (int inode = 0; inode < nodes; ++inode)
            {
                if (inode == node_id) continue;
                Receive(inode);
                global += GetLL(inode);
            }
            for (int inode = 0; inode < nodes; ++inode)
            {
                if (inode == node_id) continue;
                PutLL(inode, global);
                Send(inode);
            }
        }
        else
        {
            PutLL(current_master, local);
            Send(current_master);
            Receive(current_master);
            global = GetLL(current_master);
        }
        if (global <= index)
        {
            a = c; at = global;
        }
        else
        {
            b = c;
        }
    }

    // Print output
    for (int64_t i = 0; i < ilast - ifirst; ++i)
    {
        for (int64_t j = 0; j < towels_per_stack; ++j)
        {
            if (vtowels[i][j] == a)
            {
                cout << GetTowelCleanlinessRank(i + ifirst, towels_per_stack - (j + index - at) - 1) << endl;
                return 0;
            }
        }
    }
    return 0;
}
