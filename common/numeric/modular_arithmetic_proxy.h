#pragma once

#include "modular_arithmetic.h"

template <bool is_prime = true, bool is_32bit = true, typename _TValue = uint64_t>
class ModularArithmeticProxy
{
public:
	using TValue = _TValue;
	using TBase = ModularArithmetic<is_prime, is_32bit, TValue>;
	using TSelf = ModularArithmeticProxy<is_prime, is_32bit, TValue>;

protected:
	TValue mod;

public:
	static bool IsModPrime() { return is_prime; }
	static bool IsMod32Bits() { return is_32bit; }
	TValue GetMod() const { return mod; }
	void SetMod(TValue _mod) { mod = _mod; }

	ModularArithmeticProxy(TValue _mod) : mod(_mod) {}

	TValue ApplyU(uint64_t value) const { return TBase::ApplyU(value, mod); }
	TValue ApplyS(int64_t value) const { return TBase::ApplyS(value, mod); }
	TValue ApplyT(TValue value) const { return TBase::ApplyT(value, mod); }

	TValue Add(TValue lvalue, TValue rvalue) const { return TBase::Add(lvalue, rvalue, mod); }
	TValue AddSafe(TValue lvalue, TValue rvalue) const { return TBase::AddSafe(lvalue, rvalue, mod); }
	TValue Sub(TValue lvalue, TValue rvalue) const { return TBase::Sub(lvalue, rvalue, mod); }
	TValue SubSafe(TValue lvalue, TValue rvalue) const { return TBase::SubSafe(lvalue, rvalue, mod); }
	TValue Minus(TValue lvalue) const { return TBase::Minus(lvalue, mod); }
	TValue MinusSafe(TValue lvalue) const { return TBase::MinusSafe(lvalue, mod); }
	TValue Mult(TValue lvalue, TValue rvalue) const { return TBase::Mult(lvalue, rvalue, mod); }
	TValue MultSafe(TValue lvalue, TValue rvalue) const { return TBase::MultSafe(lvalue, rvalue, mod); }
	TValue Inverse(TValue lvalue) const { return TBase::Inverse(lvalue, mod); }
	TValue InverseSafe(TValue lvalue) const { return TBase::InverseSafe(lvalue, mod); }
	TValue Div(TValue numerator, TValue denominator) const { return TBase::Div(numerator, denominator, mod); }
	TValue DivSafe(TValue numerator, TValue denominator) const { return TBase::DivSafe(numerator, denominator, mod); }

	TValue PowU(TValue x, uint64_t pow) const { return TBase::PowU(x, pow, mod); }
	TValue PowUSafe(TValue x, uint64_t pow) const { return TBase::PowUSafe(x, pow, mod); }
	TValue PowS(TValue x, int64_t pow) const { return TBase::PowS(x, pow, mod); }
	TValue PowSSafe(TValue x, int64_t pow) const { return TBase::PowSSafe(x, pow, mod); }
};

using TModularArithmeticProxy_P32S = ModularArithmeticProxy<true, true, int64_t>;
using TModularArithmeticProxy_P32U = ModularArithmeticProxy<true, true, uint64_t>;
using TModularArithmeticProxy_P64S = ModularArithmeticProxy<true, false, int64_t>;
using TModularArithmeticProxy_P64U = ModularArithmeticProxy<true, false, uint64_t>;
using TModularArithmeticProxy_C32S = ModularArithmeticProxy<false, true, int64_t>;
using TModularArithmeticProxy_C32U = ModularArithmeticProxy<false, true, uint64_t>;
using TModularArithmeticProxy_C64S = ModularArithmeticProxy<false, false, int64_t>;
using TModularArithmeticProxy_C64U = ModularArithmeticProxy<false, false, uint64_t>;
