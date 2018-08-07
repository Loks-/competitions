#include "message.h"
#include "query_of_death.h"

#include "common/base.h"

int main_query_of_death()
{
	int nodes = NumberOfNodes();
	int node_id = MyNodeId();
    int64_t length = GetLength();
    
    assert((nodes % 3) == 1);
    int blocks = nodes / 3;
    int block_id = node_id / 3;
    int64_t first = block_id * length / blocks, last = (block_id + 1) * length / blocks, ln = last - first;

    if (node_id != nodes - 1)
    {
        int master = 3 * block_id;
        if (ln > 1)
        {
            if ((node_id % 3) == 1)
            {
                for (int64_t i = first; i < last; ++i)
                    PutChar(master, GetValue(i));
                Send(master);
            }
            else if ((node_id % 3) == 2)
            {
                vector<char> v(ln);
                for (int64_t i = last - 1; i >= first; --i)
                    v[i - first] = GetValue(i);
                for (char c : v)
                    PutChar(master, c);
                Send(master);
            }
            else // master
            {
                assert(node_id == master);
                vector<char> v(ln);
                Receive(master + 1);
                for (int64_t i = 0; i < ln; ++i)
                {
                    v[i] = GetChar(master + 1);
                }
                Receive(master + 2);
                int sum = 0;
                for (int64_t i = 0; i < ln; ++i)
                {
                    char c1 = v[i];
                    char c2 = GetChar(master + 2);
                    if (c1 == c2)
                        sum += c1;
                    else
                        sum += GetValue(i + first);
                }
                PutInt(nodes - 1, sum);
                Send(nodes - 1);
            }
        }
        else if (node_id == master)
        {
            int sum = 0;
            for (int64_t i = first; i < last; ++i)
            {
                sum += GetChar(i);
            }
            PutInt(nodes - 1, sum);
            Send(nodes - 1);
        }
    }
    else // (node_id == node_id - 1)
    {
        int sum = 0;
        for (int inode = 0; inode < node_id; inode += 3)
        {
            Receive(inode);
            sum += GetInt(inode);
        }
        cout << sum << endl;
    }
    return 0;
}
