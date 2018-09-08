#pragma once

#include "../base.h"
#include <algorithm>
#include <vector>

template<class TValue = int64_t>
class BIT_3D
{
protected:
	unsigned size1, size2, size3;
	std::vector<TValue> values;

public:
	BIT_3D(unsigned _size1, unsigned _size2, unsigned _size3) : size1(_size1), size2(_size2), size3(_size3)
	{
		values.resize(size1 * size2 * size3);
	}

	void Reset(unsigned new_size1, unsigned new_size2, unsigned new_size3)
	{
		size1 = new_size1;
		size2 = new_size2;
		size3 = new_size3;
		values.resize(size1 * size2 * size3);
		std::fill(values.begin(), values.end(), TValue(0));
	}

	void Add(unsigned index1, unsigned index2, unsigned index3, TValue value = TValue(1))
	{
		for (unsigned i1 = index1; i1 < size1; i1 |= (i1 + 1))
		{
			for (unsigned i2 = index2; i2 < size2; i2 |= (i2 + 1))
			{
				for (unsigned i3 = index3; i3 < size3; i3 |= (i3 + 1))
				{
					values[i3 + size3 * (i2 + size2 * i1)] += value;
				}
			}
		}
	}

	// Get sum of all elements before end.
	TValue Sum(unsigned end1, unsigned end2, unsigned end3) const
	{
		TValue sum = TValue();
		for (unsigned j1, i1 = end1; i1; i1 &= j1)
		{
			j1 = i1 - 1;
			for (unsigned j2, i2 = end2; i2; i2 &= j2)
			{
				j2 = i2 - 1;
				for (unsigned j3, i3 = end3; i3; i3 &= j3)
				{
					j3 = i3 - 1;
					sum += values[j3 + size3 * (j2 + size2 * j1)];
				}
			}
		}
		return sum;
	}

	TValue Sum(unsigned b1, unsigned b2, unsigned b3, unsigned e1, unsigned e2, unsigned e3) const
	{
		return Sum(e1, e2, e3) - Sum(e1, e2, b3) - Sum(e1, b2, e3) + Sum(e1, b2, b3) - 
			Sum(b1, e2, e3) + Sum(b1, e2, b3) + Sum(b1, b2, e3) - Sum(b1, b2, b3);
	}

	TValue Get(unsigned index1, unsigned index2, unsigned index3) const
	{
		return Sum(index1, index2, index3, index1 + 1, index2 + 1, index3 + 1);
	}
};
