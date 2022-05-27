#include "common/proxy_run_main.h"

#include <iostream>
#include <string>

int main(int nargs, char **pargs) {
  std::string solution_name;
  if (nargs >= 2) {
    solution_name = pargs[1];
  } else {
    std::cout << "Input leetcode solution to run:" << std::endl;
    std::cin >> solution_name;
  }
  return ProxyRun(solution_name);
}
