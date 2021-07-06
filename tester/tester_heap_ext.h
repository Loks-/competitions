#pragma once

#include "common/base.h"

#include <string>
#include <unordered_set>
#include <vector>

class TesterHeapExt {
 protected:
  unsigned size, dpm;
  std::vector<size_t> v, vinf;
  std::unordered_set<size_t> hs;

 public:
  TesterHeapExt(unsigned _size, unsigned _dpm);

 protected:
  bool CheckHash() const;

  size_t TestPriorityQueue() const;

  template <class THeap>
  size_t TestBase(const std::string& name) const;

  template <class THeap>
  size_t TestBaseNodesManager(const std::string& name) const;

  template <class THeap>
  size_t TestExt(const std::string& name) const;

  template <class THeap>
  size_t TestExtNodesManager(const std::string& name) const;

  template <class THeap>
  size_t TestKVM(const std::string& name) const;

  template <bool multipass, bool auxiliary>
  size_t TestBasePairing() const;

  template <bool multipass, bool auxiliary>
  size_t TestExtPairing() const;

  template <bool multipass, bool auxiliary>
  size_t TestUKVMPairing() const;

  template <class THeap, class TExtra>
  size_t TestKVME(const std::string& name, const TExtra& e) const;

  void TestVEBT();

 public:
  bool TestAll();
};
