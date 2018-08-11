#pragma once

#include "modular_arithmetic.h"
#include "primes_list.h"

class MillerRabinPrimalityTest
{
public:
    using TModular = ModularArithmetic<false, false, uint64_t>;
	
    enum Primality {
		COMPOSITE,
		PROBABLY_PRIME,
	};

	// n - 1 == d * 2^s
	struct Factorization {
		uint64_t n;
		uint64_t s;
		uint64_t d;
	};

	static Factorization Factor(uint64_t n) {
		Factorization f;
		f.n = n;
		f.d = n - 1;
		f.s = 0;
		while ((~f.d) & 1) {
			f.d >>= 1;
			++f.s;
		}
		return f;
	}

	static const vector<uint64_t>& SelectWitnesses(uint64_t n) {
		// Note: http://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
		// lists minimal sets of witnesses that are sufficient for deterministic
		// testing of primes for certain ranges of inputs. We've selected only
		// a few of those ranges; it may be worthwhile to add more ranges here.
		static const vector<uint64_t> w2k = { 2 };
		static const vector<uint64_t> w25M = { 2, 3, 5 };
		static const vector<uint64_t> w3T = { 2, 3, 5, 7, 11, 13 };
		static const vector<uint64_t> w341T = { 2, 3, 5, 7, 11, 13, 17 };
		static const vector<uint64_t> wmax = { 2, 3, 5, 7, 11, 13, 17, 19, 23 };

		if (n < 2047ull) { return w2k; }
		else if (n < 25326001ull) { return w25M; }
		else if (n < 3474749660383ull) { return w3T; }
		else if (n < 341550071728321ull) { return w341T; }
		else if (n < 3825123056546413051ull) { return wmax; }
		else { throw std::out_of_range("Input is too large."); }
	}

	static Primality CheckMillerRabinWitness(const Factorization& f, uint64_t witness) {
		uint64_t x = TModular::PowU(witness, f.d, f.n);
		if ((x == 1) || (x == (f.n - 1))) return PROBABLY_PRIME;

		for (uint64_t i = 0; i < f.s - 1; ++i) {
			x = TModular::Mult(x, x, f.n);
			if (x == 1) return COMPOSITE;
			if (x == f.n - 1) return PROBABLY_PRIME;
		}
		return COMPOSITE;
	}

	static Primality RunMillerRabinTest(uint64_t n) {
		if ((~n) & 1) return COMPOSITE;
		Factorization f = Factor(n);
		const std::vector<uint64_t>& witnesses = SelectWitnesses(n);

		for (size_t i = 0; i < witnesses.size(); ++i) {
			uint64_t witness = witnesses[i];
			if (CheckMillerRabinWitness(f, witness) == COMPOSITE) return COMPOSITE;
		}
		return PROBABLY_PRIME;
	}

private:
	uint64_t N, M;
	vector<uint8_t> primes;
	vector<uint8_t> table;

public:
	MillerRabinPrimalityTest(unsigned maxprime = 13)
	{
		N = maxprime, M = 1;
		PrimesList p(maxprime);
		primes.resize(maxprime + 1, 0);
		for (size_t i = 0; i < p.vprimes.size(); ++i)
		{
			M *= p.vprimes[i];
			primes[p.vprimes[i]] = 1;
		}
		table.resize(M, 1);
		for (size_t i = 0; i < p.vprimes.size(); ++i)
		{
			uint64_t ip = p.vprimes[i];
			for (uint64_t j = 0; j < M; j += ip)
			{
				table[j] = 0;
			}
		}
	}

	bool IsPrime(uint64_t n) const
	{
		if (n <= N)
			return (primes[n] != 0);
		if (!table[n % M])
			return false;
		return (RunMillerRabinTest(n) == PROBABLY_PRIME);
	}
};
