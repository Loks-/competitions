#pragma once

#include "common/base.h"

#include <string>
#include <vector>

class TesterHeap {
 protected:
  std::vector<size_t> vinit, vloop;

 public:
  TesterHeap(unsigned size_init, unsigned size_loop);

 protected:
  size_t TestPriorityQueue() const;
  size_t TestBinomialUKeyValueMap() const;
  size_t TestFibonacciUKeyValueMap() const;

  template <class THeap>
  size_t TestBase(const std::string& name) const;

  template <class THeap>
  size_t TestNodesManager(const std::string& name) const;

  template <unsigned d>
  size_t TestDHeapUKeyPosMap() const;

  template <unsigned d>
  size_t TestDHeapUKeyValueMap() const;

  template <bool multipass, bool auxiliary>
  size_t TestBasePairing() const;

  template <bool multipass, bool auxiliary>
  size_t TestExtPairing() const;

  template <bool multipass, bool auxiliary>
  size_t TestPairingUKeyValueMap() const;

 public:
  bool TestAll() const;
};
