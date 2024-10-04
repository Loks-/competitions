#pragma once

/**
 * @brief Checks if a character is a digit.
 *
 * This function checks if the input character is a digit (0-9).
 * It is a constexpr function, which allows it to be used in other constexpr
 * contexts.
 *
 * @param c The character to check.
 * @return true if the character is a digit, false otherwise.
 */
constexpr bool IsDigit(char c) { return (c >= '0') && (c <= '9'); }
