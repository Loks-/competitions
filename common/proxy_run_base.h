#pragma once

#include <map>
#include <string>

using TSolutionPtr = int (*)();
using TSolutionsMap = std::map<std::string, TSolutionPtr>;

extern TSolutionsMap solutions_map;
