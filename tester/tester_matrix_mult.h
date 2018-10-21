#pragma once

#include "common/hash.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular/modular.h"
#include "common/modular/modular_arithmetic_static_proxy.h"
#include "common/timer.h"

#include "matrix_mult.h"

#include <iostream>
#include <string>
#include <unordered_set>

template<unsigned large_matrix_size, unsigned small_matrix_size, unsigned small_matrix_runs>
class TesterMatrixMult
{
public:
	using TModular = Modular<>;
	using TModularProxy = ModularArithmeticStaticProxy<>;

public:
	static size_t MatrixHash(const Matrix<uint64_t>& m)
	{
		size_t h = 0;
		for (uint64_t t : m.GetData())
			h = hash_combine(h, TModularProxy::ApplyU(t));
		return h;
	}

	static size_t MatrixHash(const Matrix<TModular>& m)
	{
		size_t h = 0;
		for (TModular t : m.GetData())
			h = hash_combine(h, t.Get());
		return h;
	}

	template<unsigned matrix_size>
	static size_t MatrixHash(const MatrixStaticSize<uint64_t, matrix_size, matrix_size>& m)
	{
		size_t h = 0;
		for (uint64_t t : m.GetData())
			h = hash_combine(h, TModularProxy::ApplyU(t));
		return h;
	}

	template<unsigned matrix_size>
	static size_t MatrixHash(const MatrixStaticSize<TModular, matrix_size, matrix_size>& m)
	{
		size_t h = 0;
		for (TModular t : m.GetData())
			h = hash_combine(h, t.Get());
		return h;
	}

protected:
    std::unordered_set<size_t> results;
	Matrix<uint64_t> mluA, mluB, mluC, msuA, msuB, msuC;
	Matrix<TModular> mlmA, mlmB, mlmC, msmA, msmB, msmC;
    MatrixStaticSize<uint64_t, small_matrix_size, small_matrix_size> mssuA, mssuB, mssuC;
    MatrixStaticSize<TModular, small_matrix_size, small_matrix_size> mssmA, mssmB, mssmC;

public:
    TesterMatrixMult() :
        mluA(large_matrix_size), mluB(large_matrix_size), mluC(large_matrix_size),
		msuA(small_matrix_size), msuB(small_matrix_size), msuC(small_matrix_size),
		mlmA(large_matrix_size), mlmB(large_matrix_size), mlmC(large_matrix_size),
		msmA(small_matrix_size), msmB(small_matrix_size), msmC(small_matrix_size)
	{}

    void Init()
    {
        results.clear();
        for (unsigned i = 0; i < large_matrix_size; ++i)
        {
            for (unsigned j = 0; j < large_matrix_size; ++j)
            {
                mluA(i, j) = i * large_matrix_size + j;
                mlmA(i, j) = TModular(mluA(i, j));
                mluB(i, j) = (i + 2) * large_matrix_size + j;
                mlmB(i, j) = TModular(mluB(i, j));
            }
        }
		for (unsigned i = 0; i < small_matrix_size; ++i)
		{
			for (unsigned j = 0; j < small_matrix_size; ++j)
			{
				mssuA(i, j) = msuA(i, j) = i * small_matrix_size + j;
				mssmA(i, j) = msmA(i, j) = TModular(msuA(i, j));
			}
		}
	}

    template<class TMatrix>
    void TestLargeMultBase(const std::string& text, const TMatrix& A, const TMatrix& B, TMatrix& C)
    {
        Timer t;
        A.Mult(B, C);
        t.Stop();
        size_t h = MatrixHash(C);
        std::cout << text << " base\t" << h << "\t" << t.GetMilliseconds() << std::endl;
        results.insert(h);
    }

    template<class TMatrix>
    void TestLargeMultPointers(const std::string& text, const TMatrix& A, const TMatrix& B, TMatrix& C)
    {
        Timer t;
        MatrixMultPointers(A, B, C);
        t.Stop();
        size_t h = MatrixHash(C);
        std::cout << text << " pntr\t" << h << "\t" << t.GetMilliseconds() << std::endl;
        results.insert(h);
    }

    template<class TMatrix>
    void TestLargeMultLoops(const std::string& text, const TMatrix& A, const TMatrix& B, TMatrix& C)
    {
        Timer t;
        MatrixMultLoops(A, B, C);
        t.Stop();
        size_t h = MatrixHash(C);
        std::cout << text << " loop\t" << h << "\t" << t.GetMilliseconds() << std::endl;
        results.insert(h);
    }

	template<class TMatrix>
	void TestSmallMultBase(const std::string& text, const TMatrix& A, TMatrix& B, TMatrix& C)
	{
		Timer t;
		B = A;
		for (unsigned i = 0; i < small_matrix_runs; ++i)
		{
			A.Mult(B, C);
			B.swap(C);
		}
		t.Stop();
		size_t h = MatrixHash(B);
		std::cout << text << " base\t" << h << "\t" << t.GetMilliseconds() << std::endl;
		results.insert(h);
	}

	template<class TMatrix>
	void TestSmallMultPointers(const std::string& text, const TMatrix& A, TMatrix& B, TMatrix& C)
	{
		Timer t;
		B = A;
		for (unsigned i = 0; i < small_matrix_runs; ++i)
		{
			MatrixMultPointers(A, B, C);
			B.swap(C);
		}
		t.Stop();
		size_t h = MatrixHash(B);
		std::cout << text << " pntr\t" << h << "\t" << t.GetMilliseconds() << std::endl;
		results.insert(h);
	}

	template<class TMatrix>
	void TestSmallMultLoops(const std::string& text, const TMatrix& A, TMatrix& B, TMatrix& C)
	{
		Timer t;
		B = A;
		for (unsigned i = 0; i < small_matrix_runs; ++i)
		{
			MatrixMultLoops(A, B, C);
			B.swap(C);
		}
		t.Stop();
		size_t h = MatrixHash(B);
		std::cout << text << " loop\t" << h << "\t" << t.GetMilliseconds() << std::endl;
		results.insert(h);
	}

	void TestLargeMultAll()
    {
		TestLargeMultBase("uint64t l ", mluA, mluB, mluC);
		TestLargeMultPointers("uint64t l ", mluA, mluB, mluC);
		TestLargeMultLoops("uint64t l ", mluA, mluB, mluC);
		TestLargeMultBase("modular l ", mlmA, mlmB, mlmC);
		TestLargeMultPointers("modular l ", mlmA, mlmB, mlmC);
		TestLargeMultLoops("modular l ", mlmA, mlmB, mlmC);
    }

	void TestSmallMultAll()
	{
		TestSmallMultBase("uint64t s ", msuA, msuB, msuC);
		TestSmallMultPointers("uint64t s ", msuA, msuB, msuC);
		TestSmallMultLoops("uint64t s ", msuA, msuB, msuC);
		TestSmallMultBase("modular s ", msmA, msmB, msmC);
		TestSmallMultPointers("modular s ", msmA, msmB, msmC);
		TestSmallMultLoops("modular s ", msmA, msmB, msmC);

		TestSmallMultBase("uint64t ss", mssuA, mssuB, mssuC);
		TestSmallMultPointers("uint64t ss", mssuA, mssuB, mssuC);
		TestSmallMultLoops("uint64t ss", mssuA, mssuB, mssuC);
		TestSmallMultBase("modular ss", mssmA, mssmB, mssmC);
		TestSmallMultPointers("modular ss", mssmA, mssmB, mssmC);
		TestSmallMultLoops("modular ss", mssmA, mssmB, mssmC);
	}

	bool TestMultAll()
	{
		Init();
		TestLargeMultAll();
		TestSmallMultAll();
		return (results.size() == 2);
	}
};
