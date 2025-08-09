#pragma once

#include "tester/binary_search_tree/scenario/result.h"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace tester {
namespace bst {
namespace {

struct ImplementationStats {
  size_t successful_runs = 0;
  std::chrono::nanoseconds total_time_ns = std::chrono::nanoseconds(0);
};

using ResultMap =
    std::unordered_map<std::string, std::vector<scenario::Result>>;
using StatsMap = std::unordered_map<std::string, ImplementationStats>;

}  // namespace

// Helper to run a single scenario with a single implementation
template <
    bool extra_checks, typename Scenario,
    template <typename, typename, typename, typename> class Implementation>
void run_single(size_t size, ResultMap& scenario_results,
                StatsMap& implementation_stats,
                std::string_view implementation_filter) {
  using IType = Implementation<typename Scenario::Data, typename Scenario::Key,
                               typename Scenario::AggregatorsTuple,
                               typename Scenario::DeferredTuple>;
  using Tree = typename IType::TreeType;

  const auto implementation_id = IType::id();
  if (!implementation_filter.empty() &&
      implementation_filter != implementation_id) {
    return;
  }

  if constexpr ((Scenario::requires_key == Tree::has_key) &&
                (!Scenario::requires_insert || Tree::support_insert) &&
                (!Scenario::requires_remove || Tree::support_remove) &&
                (!Scenario::requires_remove_node ||
                 Tree::support_remove_by_node) &&
                (!Scenario::requires_split || Tree::support_split) &&
                (!Scenario::requires_join || Tree::support_join)) {
    auto result = Scenario::template run<extra_checks, Implementation>(size);

    scenario_results[result.scenario_id].push_back(result);
    if (result.success) {
      implementation_stats[result.implementation_id].successful_runs++;
      implementation_stats[result.implementation_id].total_time_ns +=
          result.duration;
    } else {
      std::cout << "Error in scenario: " << result.scenario_id
                << " implementation: " << result.implementation_id
                << " error: " << result.error_message << std::endl;
    }
  }
}

// Helper to run all scenarios for a single implementation
template <
    bool extra_checks,
    template <typename, typename, typename, typename> class Implementation,
    typename ScenariosTuple>
void run_implementation(size_t size, ResultMap& scenario_results,
                        StatsMap& implementation_stats,
                        std::string_view implementation_filter) {
  std::apply(
      [&](auto... scenarios) {
        (run_single<extra_checks, decltype(scenarios), Implementation>(
             size, scenario_results, implementation_stats,
             implementation_filter),
         ...);
      },
      ScenariosTuple{});
}

template <
    bool extra_checks, typename ScenariosTuple,
    template <typename, typename, typename, typename> class... Implementations>
bool run_each(size_t size, std::string_view implementation_filter = {}) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  // Maps to store results
  ResultMap scenario_results;
  StatsMap implementation_stats;

  // Run each implementation with all scenarios
  (run_implementation<extra_checks, Implementations, ScenariosTuple>(
       size, scenario_results, implementation_stats, implementation_filter),
   ...);

  // Verify hashes for each scenario
  bool all_success = true;
  bool all_hashes_match = true;
  for (const auto& [scenario_id, results] : scenario_results) {
    if (results.empty()) continue;

    for (const auto& result : results) {
      if (!result.success) all_success = false;
    }

    const auto& first_hash = results[0].control_hash;
    bool scenario_hashes_match = true;

    for (size_t i = 1; i < results.size(); ++i) {
      if (results[i].control_hash != first_hash) {
        scenario_hashes_match = false;
        break;
      }
    }

    if (!scenario_hashes_match) {
      all_hashes_match = false;
      std::cout << "Hash mismatch in scenario: " << scenario_id << "\n";
      for (const auto& result : results) {
        std::cout << "  " << result.implementation_id << ": "
                  << result.control_hash << "\n";
      }
    }
  }

  // Print implementation statistics
  std::cout << "\nImplementation Statistics:\n";
  std::cout << "Name, Successful Runs, Total Time (ms)\n";
  std::cout << "-------------------------------------\n";

  // Convert to vector for sorting
  std::vector<std::pair<std::string_view, ImplementationStats>> sorted_stats(
      implementation_stats.begin(), implementation_stats.end());

  // Sort by successful runs (descending) and total time (ascending)
  std::sort(sorted_stats.begin(), sorted_stats.end(),
            [](const auto& a, const auto& b) {
              if (a.second.successful_runs != b.second.successful_runs) {
                return a.second.successful_runs > b.second.successful_runs;
              }
              return a.second.total_time_ns < b.second.total_time_ns;
            });

  // Print sorted results
  for (const auto& [id, stats] : sorted_stats) {
    std::cout << std::setw(20) << std::left << id << " , " << std::setw(5)
              << std::right << stats.successful_runs << " , " << std::setw(5)
              << std::right << std::fixed << std::setprecision(2)
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     stats.total_time_ns)
                     .count()
              << "\n";
  }

  return all_success && all_hashes_match;
}

}  // namespace bst
}  // namespace tester
