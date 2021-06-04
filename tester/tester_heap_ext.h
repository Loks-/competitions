#pragma once

#include "common/base.h"

#include <string>
#include <vector>

class TesterHeapExt {
 protected:
  unsigned size, dpm;
  std::vector<size_t> v, vinf;

 public:
  TesterHeapExt(unsigned _size, unsigned _dpm);

 protected:
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

 public:
  bool TestAll() const;
};
