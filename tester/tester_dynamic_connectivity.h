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

 public:
  bool TestAll();
};
