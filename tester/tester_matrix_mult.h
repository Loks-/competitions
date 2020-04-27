#pragma once

#include "tester/matrix_mult.h"

#include "common/hash.h"
#include "common/linear_algebra/bool/matrix.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular.h"
#include "common/modular/static/bool.h"
#include "common/modular/static/proxy.h"
#include "common/timer.h"

#include <iostream>
#include <string>
#include <unordered_set>

template <unsigned large_matrix_size, unsigned small_matrix_size,
          unsigned small_matrix_runs>
class TesterMatrixMult {
 public:
  using TModular2 = TModular_P32<2>;
  using TModularProxy = modular::mstatic::TProxy_P32<1000000007>;

 public:
  static size_t MatrixHash(const la::Matrix<uint64_t>& m,
                           bool use_bool_hash = false) {
    size_t h = 0;
    for (uint64_t t : m.GetData())
      h = HashCombine(h, (use_bool_hash ? t & 1 : TModularProxy::ApplyU(t)));
    return h;
  }

  static size_t MatrixHash(const la::Matrix<TModularD>& m) {
    size_t h = 0;
    for (TModularD t : m.GetData()) h = HashCombine(h, t.Get());
    return h;
  }

  static size_t MatrixHash(const la::Matrix<TModular2>& m) {
    size_t h = 0;
    for (TModular2 t : m.GetData()) h = HashCombine(h, t.Get());
    return h;
  }

  static size_t MatrixHash(const la::Matrix<ModularBool>& m) {
    size_t h = 0;
    for (ModularBool t : m.GetData()) h = HashCombine(h, t.Get() ? 1 : 0);
    return h;
  }

  template <unsigned matrix_size>
  static size_t MatrixHash(
      const la::MatrixStaticSize<uint64_t, matrix_size, matrix_size>& m) {
    size_t h = 0;
    for (uint64_t t : m.GetData()) h = HashCombine(h, TModularProxy::ApplyU(t));
    return h;
  }

  template <unsigned matrix_size>
  static size_t MatrixHash(
      const la::MatrixStaticSize<TModularD, matrix_size, matrix_size>& m) {
    size_t h = 0;
    for (TModularD t : m.GetData()) h = HashCombine(h, t.Get());
    return h;
  }

  static size_t MatrixHash(const la::MatrixBool& m) {
    size_t h = 0;
    for (unsigned i = 0; i < m.Rows(); ++i) {
      for (unsigned j = 0; j < m.Columns(); ++j)
        h = HashCombine(h, m.Get(i, j).Get());
    }
    return h;
  }

  static void ApplyMod(la::Matrix<uint64_t>& m) {
    for (uint64_t& t : m) t = TModularProxy::ApplyU(t);
  }

  static void ApplyMod(la::Matrix<TModularD>&) {}

  template <unsigned matrix_size>
  static void ApplyMod(
      la::MatrixStaticSize<uint64_t, matrix_size, matrix_size>& m) {
    for (uint64_t& t : m) t = TModularProxy::ApplyU(t);
  }

  template <unsigned matrix_size>
  static void ApplyMod(
      la::MatrixStaticSize<TModularD, matrix_size, matrix_size>&) {}

 protected:
  std::unordered_set<size_t> results;
  la::Matrix<uint64_t> mluA, mluB, mluC, msuA, msuB, msuC;
  la::Matrix<TModularD> mlmA, mlmB, mlmC, msmA, msmB, msmC;
  la::Matrix<TModular2> mlm2A, mlm2B, mlm2C;
  la::Matrix<ModularBool> mlmbA, mlmbB, mlmbC;
  la::MatrixBool mlbA, mlbB, mlbC;
  la::MatrixStaticSize<uint64_t, small_matrix_size, small_matrix_size> mssuA,
      mssuB, mssuC;
  la::MatrixStaticSize<TModularD, small_matrix_size, small_matrix_size> mssmA,
      mssmB, mssmC;

 public:
  TesterMatrixMult()
      : mluA(large_matrix_size),
        mluB(large_matrix_size),
        mluC(large_matrix_size),
        msuA(small_matrix_size),
        msuB(small_matrix_size),
        msuC(small_matrix_size),
        mlmA(large_matrix_size),
        mlmB(large_matrix_size),
        mlmC(large_matrix_size),
        msmA(small_matrix_size),
        msmB(small_matrix_size),
        msmC(small_matrix_size),
        mlm2A(large_matrix_size),
        mlm2B(large_matrix_size),
        mlm2C(large_matrix_size),
        mlmbA(large_matrix_size),
        mlmbB(large_matrix_size),
        mlmbC(large_matrix_size),
        mlbA(large_matrix_size),
        mlbB(large_matrix_size),
        mlbC(large_matrix_size) {}

  void Init() {
    results.clear();
    for (unsigned i = 0; i < large_matrix_size; ++i) {
      for (unsigned j = 0; j < large_matrix_size; ++j) {
        mluA(i, j) = i * large_matrix_size + j;
        mlmA(i, j) = TModularD(mluA(i, j));
        mlm2A(i, j) = TModular2(mluA(i, j));
        mlmbA(i, j) = ModularBool(mluA(i, j));
        mlbA.Set(i, j, mlmbA(i, j));
        mluB(i, j) = (i + 2) * large_matrix_size + j;
        mlmB(i, j) = TModularD(mluB(i, j));
        mlm2B(i, j) = TModular2(mluB(i, j));
        mlmbB(i, j) = ModularBool(mluB(i, j));
        mlbB.Set(i, j, mlmbB(i, j));
      }
    }
    for (unsigned i = 0; i < small_matrix_size; ++i) {
      for (unsigned j = 0; j < small_matrix_size; ++j) {
        mssuA(i, j) = msuA(i, j) = i * small_matrix_size + j;
        mssmA(i, j) = msmA(i, j) = TModularD(msuA(i, j));
      }
    }
  }

  template <class TMatrix>
  void TestLargeMultBase(const std::string& text, const TMatrix& A,
                         const TMatrix& B, TMatrix& C) {
    Timer t;
    A.Mult(B, C);
    t.Stop();
    size_t h = MatrixHash(C);
    std::cout << text << " base\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  template <class TMatrix>
  void TestLargeMultPointers(const std::string& text, const TMatrix& A,
                             const TMatrix& B, TMatrix& C) {
    Timer t;
    MatrixMultPointers(A, B, C);
    t.Stop();
    size_t h = MatrixHash(C);
    std::cout << text << " pntr\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  template <class TMatrix>
  void TestLargeMultLoops(const std::string& text, const TMatrix& A,
                          const TMatrix& B, TMatrix& C) {
    Timer t;
    MatrixMultLoops(A, B, C);
    t.Stop();
    size_t h = MatrixHash(C);
    std::cout << text << " loop\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  template <class TMatrix>
  void TestSmallMultBase(const std::string& text, const TMatrix& A, TMatrix& B,
                         TMatrix& C) {
    Timer t;
    B = A;
    for (unsigned i = 0; i < small_matrix_runs; ++i) {
      A.Mult(B, C);
      B.swap(C);
      ApplyMod(B);
    }
    t.Stop();
    size_t h = MatrixHash(B);
    std::cout << text << " base\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  template <class TMatrix>
  void TestSmallMultPointers(const std::string& text, const TMatrix& A,
                             TMatrix& B, TMatrix& C) {
    Timer t;
    B = A;
    for (unsigned i = 0; i < small_matrix_runs; ++i) {
      MatrixMultPointers(A, B, C);
      B.swap(C);
      ApplyMod(B);
    }
    t.Stop();
    size_t h = MatrixHash(B);
    std::cout << text << " pntr\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  template <class TMatrix>
  void TestSmallMultLoops(const std::string& text, const TMatrix& A, TMatrix& B,
                          TMatrix& C) {
    Timer t;
    B = A;
    for (unsigned i = 0; i < small_matrix_runs; ++i) {
      MatrixMultLoops(A, B, C);
      B.swap(C);
      ApplyMod(B);
    }
    t.Stop();
    size_t h = MatrixHash(B);
    std::cout << text << " loop\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  template <class TMatrix>
  void TestBoolMultBase(const std::string& text, const TMatrix& A,
                        const TMatrix& B, TMatrix& C) {
    Timer t;
    A.Mult(B, C);
    t.Stop();
    size_t h = MatrixHash(C, true);
    std::cout << text << " base\t" << h << "\t" << t.GetMilliseconds()
              << std::endl;
    results.insert(h);
  }

  void TestLargeMultAll() {
    TestLargeMultBase("uint64t l ", mluA, mluB, mluC);
    TestLargeMultPointers("uint64t l ", mluA, mluB, mluC);
    TestLargeMultLoops("uint64t l ", mluA, mluB, mluC);
    TestLargeMultBase("modular l ", mlmA, mlmB, mlmC);
    TestLargeMultPointers("modular l ", mlmA, mlmB, mlmC);
    TestLargeMultLoops("modular l ", mlmA, mlmB, mlmC);
  }

  void TestSmallMultAll() {
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

  void TestBoolMultAll() {
    TestBoolMultBase("uint64t b ", mluA, mluB, mluC);
    TestLargeMultBase("modular b ", mlm2A, mlm2B, mlm2C);
    TestLargeMultBase("modbool b ", mlmbA, mlmbB, mlmbC);
    TestLargeMultBase("bmatrix b ", mlbA, mlbB, mlbC);
  }

  bool TestMultAll() {
    Init();
    TestLargeMultAll();
    TestSmallMultAll();
    TestBoolMultAll();
    return (results.size() == 3);
  }
};
