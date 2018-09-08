#pragma once

#include "../base.h"
#include <unordered_map>

template<class TValue = int64_t>
class BITSparse
{
protected:
	uint64_t size;
	std::unordered_map<uint64_t, TValue> values;

public:
	BITSparse(uint64_t _size) : size(_size) {}

	void Reset(unsigned new_size)
	{
		size = new_size;
		values.clear();
	}

	void Add(unsigned index, TValue value = TValue(1))
	{
		for (; index < size; index |= (index + 1))
			values[index] += value;
	}

	// Get sum of all elements before end.
	TValue Sum(unsigned end) const
	{
		TValue sum = 0;
		for (unsigned index, index1 = end; index1; index1 &= index)
		{
			index = index1 - 1;
			sum += values[index];
		}
		return sum;
	}
	TValue Sum(unsigned begin, unsigned end) const { return Sum(end) - Sum(begin); }

	TValue Get(unsigned index) const { return Sum(index, index + 1); }
};
