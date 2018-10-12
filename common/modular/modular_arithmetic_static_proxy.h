#pragma once

#include "modular_arithmetic.h"

template <uint64_t mod = 1000000007, bool is_prime = true, bool is_32bit = true>
class ModularArithmeticStaticProxy
{
public:
	using TValue = uint64_t;
	using TBase = ModularArithmetic<is_prime, is_32bit, TValue>;
	using TSelf = ModularArithmeticStaticProxy<mod, is_prime, is_32bit>;

public:
	static bool IsModPrime() { return is_prime; }
	static bool IsMod32Bits() { return is_32bit; }
	static TValue GetMod() { return mod; }

	static TValue ApplyU(uint64_t value) { return TBase::ApplyU(value, mod); }
	static TValue ApplyS(int64_t value) { return TBase::ApplyS(value, mod); }
	static TValue ApplyT(TValue value) { return TBase::ApplyT(value, mod); }

	static TValue Add(TValue lvalue, TValue rvalue) { return TBase::Add(lvalue, rvalue, mod); }
	static TValue AddSafe(TValue lvalue, TValue rvalue) { return TBase::AddSafe(lvalue, rvalue, mod); }
	static TValue Sub(TValue lvalue, TValue rvalue) { return TBase::Sub(lvalue, rvalue, mod); }
	static TValue SubSafe(TValue lvalue, TValue rvalue) { return TBase::SubSafe(lvalue, rvalue, mod); }
	static TValue Minus(TValue lvalue) { return TBase::Minus(lvalue, mod); }
	static TValue MinusSafe(TValue lvalue) { return TBase::MinusSafe(lvalue, mod); }
	static TValue Mult(TValue lvalue, TValue rvalue) { return TBase::Mult(lvalue, rvalue, mod); }
	static TValue MultSafe(TValue lvalue, TValue rvalue) { return TBase::MultSafe(lvalue, rvalue, mod); }
	static TValue Inverse(TValue lvalue) { return TBase::Inverse(lvalue, mod); }
	static TValue InverseSafe(TValue lvalue) { return TBase::InverseSafe(lvalue, mod); }
	static TValue Div(TValue numerator, TValue denominator) { return TBase::Div(numerator, denominator, mod); }
	static TValue DivSafe(TValue numerator, TValue denominator) { return TBase::DivSafe(numerator, denominator, mod); }

	static TValue PowU(TValue x, uint64_t pow) { return TBase::PowU(x, pow, mod); }
	static TValue PowUSafe(TValue x, uint64_t pow) { return TBase::PowUSafe(x, pow, mod); }
	static TValue PowS(TValue x, int64_t pow) { return TBase::PowS(x, pow, mod); }
	static TValue PowSSafe(TValue x, int64_t pow) { return TBase::PowSSafe(x, pow, mod); }
};

template <uint64_t mod> using TModularArithmeticStaticProxy_P32 = ModularArithmeticStaticProxy<mod, true, true>;
template <uint64_t mod> using TModularArithmeticStaticProxy_C32 = ModularArithmeticStaticProxy<mod, false, true>;
