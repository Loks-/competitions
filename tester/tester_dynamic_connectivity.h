#pragma once

#include "common/base.h"

#include <string>
#include <vector>

class TesterDynamicConnectivity {
 protected:
  unsigned gsize, max_edges, main_loop;
  std::vector<unsigned> vsf, vst, vme, vmf, vmt;

 public:
  TesterDynamicConnectivity(unsigned _gsize, unsigned _max_edges, unsigned _main_loop);

 protected:
   template<class TSolver>
   size_t Test(const std::string& name) const;

//   uint64_t TestBoruvka() const;
//   uint64_t TestKruskal() const;
//   uint64_t TestPrimBaseBinaryHeap() const;

//   template <template <class> class THeap>
//   uint64_t TestPrimDHeap(const std::string& name) const;

//   template <class THeap>
//   uint64_t TestPrimKPM(const std::string& name) const;

//   template <class THeap>
//   uint64_t TestPrimKVM(const std::string& name) const;

 public:
  bool TestAll();
};
