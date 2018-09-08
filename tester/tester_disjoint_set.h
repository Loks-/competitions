#pragma once

#include "common/disjoint_set_extended.h"
#include "common/hash.h"
#include "common/timer.h"

#include "disjoint_set_proxy.h"

#include <iostream>
#include <vector>
#include <utility>

class TesterDisjointSet
{
protected:
	unsigned size, unions, unions_per_block;
	std::vector<std::pair<unsigned, unsigned>> vunions;

public:
	TesterDisjointSet(unsigned _size, unsigned _unions, unsigned _unions_per_block);
	TesterDisjointSet(unsigned _size, unsigned _unions) : TesterDisjointSet(_size, _unions, _size) {}

	template<EPathCompression pc>
	size_t Test() const
	{
		Timer t;
		size_t h = 0;
		DisjointSetProxy<pc> dsp(size);
		for (unsigned i = 0; i < unions; ++i)
		{
			if ((i > 0) && ((i % unions_per_block) == 0))
				dsp.Init(size);
			dsp.Union(vunions[i].first, vunions[i].second);
			h = std::hash_combine(h, dsp.GetUnions());
		}
		std::cout << "Test results: " << int(pc) << "\t" << h << "\t" << t.GetMilliseconds() << std::endl;
		return h;
	}

	size_t TestExtended() const
	{
		Timer t;
		size_t h = 0;
		DisjointSetExtended<unsigned> dsp;
		dsp.Reserve(size);                                      
		for (unsigned i = 0; i < unions; ++i)
		{
			if ((i > 0) && ((i % unions_per_block) == 0))
				dsp.Reset();
			dsp.Union(vunions[i].first, vunions[i].second);
			h = std::hash_combine(h, dsp.GetUnions());
		}
		std::cout << "Test results: " << "E" << "\t" << h << "\t" << t.GetMilliseconds() << std::endl;
		return h;
	}

	void TestAll() const;
};
