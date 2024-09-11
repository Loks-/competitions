#pragma once

#include <map>
#include <string>

/**
 * @brief Type alias for a pointer to a solution function.
 *
 * The solution function is expected to return an integer.
 */
using TSolutionPtr = int (*)();

/**
 * @brief Type alias for a map of solution names to solution function pointers.
 */
using TSolutionsMap = std::map<std::string, TSolutionPtr>;

/**
 * @brief Global map of solution names to their corresponding solution
 * functions.
 *
 * This map is populated with the available solutions and is used to look up
 * and execute solutions by name.
 */
extern TSolutionsMap solutions_map;
