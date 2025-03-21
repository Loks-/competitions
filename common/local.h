#pragma once

/**
 * @brief Indicates whether the code is running locally or externally.
 *
 * This constant is used to control behavior that differs between local
 * development and external runs (e.g., in competitions or production). It
 * should be:
 * - Set to true for local development and testing
 * - Set to false for external runs
 *
 * Example usage:
 * @code
 * if (kIsLocalRun) {
 *   // Development-only code
 *   debug_log("Running in local mode");
 * }
 * @endcode
 */
[[maybe_unused]] static constexpr bool kIsLocalRun = true;
