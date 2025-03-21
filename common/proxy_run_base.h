#pragma once

#include <map>
#include <string>

/**
 * @brief Type alias for a pointer to a solution function.
 *
 * The solution function is expected to return an integer.
 * This type is used to store pointers to solution functions that can be
 * called dynamically based on the solution name.
 */
using TSolutionPtr = int (*)();

/**
 * @brief Type alias for a map of solution names to solution function pointers.
 *
 * This map provides a way to associate string identifiers with their
 * corresponding solution functions, allowing for dynamic solution selection
 * at runtime.
 */
using TSolutionsMap = std::map<std::string, TSolutionPtr>;

/**
 * @brief Global map of solution names to their corresponding solution
 * functions.
 *
 * This map is populated with the available solutions and is used to look up
 * and execute solutions by name. It provides a central registry for all
 * available solutions in the system.
 *
 * @note This map should be populated before any calls to ProxyRun functions.
 */
extern TSolutionsMap solutions_map;
