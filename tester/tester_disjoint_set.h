#pragma once

#include "common/disjoint_set_extended.h"
#include "common/hash.h"
#include "common/timer.h"
#include "disjoint_set_proxy.h"

class TesterDisjointSet
{
protected:
	unsigned size, unions;
	vector<pair<unsigned, unsigned>> vunions;

public:
	TesterDisjointSet(unsigned _size, unsigned _unions);

	template<EPathCompression pc>
	size_t Test() const
	{
		Timer t;
		size_t h = 0;
		DisjointSetProxy<pc> dsp(size);
		for (auto& p : vunions)
		{
			dsp.Union(p.first, p.second);
			h = hash_combine(h, dsp.GetUnions());
		}
		cout << "Test results: " << int(pc) << "\t" << h << "\t" << t.GetMilliseconds() << endl;
		return h;
	}

	size_t TextExtended() const
	{
		Timer t;
		size_t h = 0;
		DisjointSetExtended<unsigned> dsp;
		dsp.Reserve(size);                                      
		for (auto& p : vunions)
		{
			dsp.Union(p.first, p.second);
			h = hash_combine(h, dsp.GetUnions());
		}
		cout << "Test results: " << "E" << "\t" << h << "\t" << t.GetMilliseconds() << endl;
		return h;
	}

	void TestAll() const;
};
