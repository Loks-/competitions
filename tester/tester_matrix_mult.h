#pragma once

#include "common/hash.h"
#include "common/linear_algebra/matrix.h"
#include "common/modular/modular.h"
#include "common/modular/modular_arithmetic_static_proxy.h"
#include "common/timer.h"

#include "matrix_mult.h"

#include <iostream>
#include <string>
#include <unordered_set>

template<unsigned matrix_size>
class TesterMatrixMult
{
public:
	using TModular = Modular<>;
	using TModularProxy = ModularArithmeticStaticProxy<>;

public:
	static size_t MatrixHash(const Matrix<TModular>& m)
	{
		size_t h = 0;
		for (TModular t : m.GetData())
			h = hash_combine(h, t.Get());
		return h;
	}

	static size_t MatrixHash(const Matrix<uint64_t>& m)
	{
		size_t h = 0;
		for (uint64_t t : m.GetData())
			h = hash_combine(h, TModularProxy::ApplyU(t));
		return h;
	}

protected:
    std::unordered_set<size_t> results;
	Matrix<uint64_t> muA, muB, muC;
	Matrix<TModular> mmA, mmB, mmC;

public:
    TesterMatrixMult() :
        muA(matrix_size), muB(matrix_size), muC(matrix_size),
        mmA(matrix_size), mmB(matrix_size), mmC(matrix_size) {}

    void Init()
    {
        results.clear();
        for (unsigned i = 0; i < matrix_size; ++i)
        {
            for (unsigned j = 0; j < matrix_size; ++j)
            {
                muA(i, j) = i * matrix_size + j;
                mmA(i, j) = TModular(muA(i, j));
                muB(i, j) = (i + 2) * matrix_size + j;
                mmB(i, j) = TModular(muB(i, j));
            }
        }
    }

    template<class TMatrix>
    void TestMultBase(const std::string& text, const TMatrix& A, const TMatrix& B, TMatrix& C)
    {
        Timer t;
        A.Mult(B, C);
        t.Stop();
        size_t h = MatrixHash(C);
        std::cout << text << " base\t" << h << "\t" << t.GetMilliseconds() << std::endl;
        results.insert(h);
    }

    template<class TMatrix>
    void TestMultPointers(const std::string& text, const TMatrix& A, const TMatrix& B, TMatrix& C)
    {
        Timer t;
        MatrixMultPointers(A, B, C);
        t.Stop();
        size_t h = MatrixHash(C);
        std::cout << text << " pntr\t" << h << "\t" << t.GetMilliseconds() << std::endl;
        results.insert(h);
    }

    template<class TMatrix>
    void TestMultLoops(const std::string& text, const TMatrix& A, const TMatrix& B, TMatrix& C)
    {
        Timer t;
        MatrixMultLoops(A, B, C);
        t.Stop();
        size_t h = MatrixHash(C);
        std::cout << text << " loop\t" << h << "\t" << t.GetMilliseconds() << std::endl;
        results.insert(h);
    }

    void TestMultBaseAll()
    {
        TestMultBase("uint64t   ", muA, muB, muC);
        TestMultBase("modular   ", mmA, mmB, mmC);
    }

    void TestMultPointersAll()
    {
        TestMultPointers("uint64t   ", muA, muB, muC);
        TestMultPointers("modular   ", mmA, mmB, mmC);
    }

    void TestMultLoopsAll()
    {
        TestMultLoops("uint64t   ", muA, muB, muC);
        TestMultLoops("modular   ", mmA, mmB, mmC);
    }

    bool TestMultAll()
    {
        Init();
        TestMultBaseAll();
        TestMultPointersAll();
        TestMultLoopsAll();
        return (results.size() == 1);
    }
};
