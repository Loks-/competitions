#pragma once

#include "../../base.h"

class ModularBool
{
public:
	using TValue = bool;
    using TSelf = ModularBool;

public:
	static bool IsModPrime() { return true; }
	static bool IsMod32Bits() { return true; }
	static unsigned GetMod() { return 2; }

protected:
	TValue value;

public:
	TValue Get() const { return value; }
	void SetU(uint64_t _value) { value = (_value & 1u); }
	void SetS(int64_t _value) { value = (_value & 1); }
	void SetT(TValue _value) { value = _value; }

	ModularBool() : value(false) {}
	ModularBool(const TSelf& s) : value(s.value) {}
    ModularBool(uint64_t _value) { SetU(_value); }

	TSelf operator+(TSelf rvalue) const { return TSelf(value ^ rvalue.value); }
	TSelf& operator+=(TSelf rvalue) { value ^= rvalue.value; return *this; }
	TSelf operator-() const { return *this; }
	TSelf operator-(TSelf rvalue) const { return TSelf(value ^ rvalue.value); }
	TSelf& operator-=(TSelf rvalue) { value ^= rvalue.value; return *this; }
	TSelf operator*(TSelf rvalue) const { return TSelf(value & rvalue.value); }
	TSelf& operator*=(TSelf rvalue) { value &= rvalue.value; return *this; }
	TSelf operator/(TSelf rvalue) const { assert(rvalue.value); return *this; }
	TSelf& operator/=(TSelf rvalue) { assert(rvalue.value); return *this; }
	TSelf Inverse() const { assert(value); return *this; }
	TSelf PowU(uint64_t pow) const { return (pow == 0) ? TSelf(1) : *this; }
	TSelf PowS(int64_t pow) const { assert(value || (pow >= 0)); return (pow == 0) ? TSelf(1) : *this; }

	bool operator<(const TSelf& r) const { return !value && r.value; }
	bool operator==(const TSelf& r) const { return value == r.value; }
	bool operator!=(const TSelf& r) const { return value != r.value; }
};
