#include "disjoint_set_proxy.h"
#include "tester_disjoint_set.h"

TesterDisjointSet::TesterDisjointSet(unsigned _size, unsigned _unions)
{
	size = _size;
	unions = _unions;
	vunions.reserve(unions);
	minstd_rand random_engine;
	std::uniform_int_distribution<unsigned> distribution(0, size - 1);
	for (unsigned i = 0; i < unions; ++i)
	{
		vunions.push_back({ distribution(random_engine), distribution(random_engine) });
	}
}

void TesterDisjointSet::TestAll() const
{
	// Test<pc_none>();
	Test<pc_default>();
	Test<pc_compression_recursive>();
	Test<pc_compression_two_runs>();
	Test<pc_compression_stack>();
	Test<pc_halving>();
	Test<pc_splitting>();
	TextExtended();
}

bool TestDisjointSet()
{
	TesterDisjointSet tds1(100000, 100000000);
	TesterDisjointSet tds2(1000000, 10000000);
	tds1.TestAll();
	tds2.TestAll();
	return true;
}
