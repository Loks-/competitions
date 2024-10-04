#pragma once

#include "common/base.h"

#include <string>
#include <vector>

namespace nstring {

/**
 * @brief A class to convert numbers to their word representation.
 */
class NumberToWords {
 public:
  /**
   * @brief Converts a number to its word representation.
   * @param n The number to convert.
   * @return The word representation of the number.
   */
  static constexpr std::string Convert(uint64_t n) {
    const std::vector<std::string> vbase{"", "Thousand", "Million", "Billion",
                                         "Trillion"};
    if (n == 0) {
      return ConvertZero();
    }
    const uint64_t h = 1000;
    std::string s;
    for (unsigned i = 0; n; ++i, n /= h) {
      const uint64_t nh = n % h;
      if (nh == 0) {
        continue;
      }
      std::string st = ConvertThousand(nh);
      if (i) {
        st = st + " " + vbase[i];
      }
      s = (s.empty() ? st : st + " " + s);
    }
    return s;
  }

 protected:
  /**
   * @brief Converts the number zero to its word representation.
   * @return The string "Zero".
   */
  static constexpr std::string ConvertZero() { return "Zero"; }

  /**
   * @brief Converts a number less than 100 to its word representation.
   * @param n The number to convert (must be less than 100).
   * @return The word representation of the number.
   */
  static constexpr std::string ConvertHundred(uint64_t n) {
    assert(n < 100);
    switch (n) {
      case 0:
        return "";
      case 1:
        return "One";
      case 2:
        return "Two";
      case 3:
        return "Three";
      case 4:
        return "Four";
      case 5:
        return "Five";
      case 6:
        return "Six";
      case 7:
        return "Seven";
      case 8:
        return "Eight";
      case 9:
        return "Nine";
      case 10:
        return "Ten";
      case 11:
        return "Eleven";
      case 12:
        return "Twelve";
      case 13:
        return "Thirteen";
      case 14:
        return "Fourteen";
      case 15:
        return "Fifteen";
      case 16:
        return "Sixteen";
      case 17:
        return "Seventeen";
      case 18:
        return "Eighteen";
      case 19:
        return "Nineteen";
      case 20:
        return "Twenty";
      case 30:
        return "Thirty";
      case 40:
        return "Forty";
      case 50:
        return "Fifty";
      case 60:
        return "Sixty";
      case 70:
        return "Seventy";
      case 80:
        return "Eighty";
      case 90:
        return "Ninety";
      default:
        return ConvertHundred(n - (n % 10)) + " " + ConvertHundred(n % 10);
    }
  }

  /**
   * @brief Converts a number less than 1000 to its word representation.
   * @param n The number to convert (must be less than 1000).
   * @return The word representation of the number.
   */
  static constexpr std::string ConvertThousand(uint64_t n) {
    assert(n < 1000);
    std::string s;
    if (n >= 100) {
      s = ConvertHundred(n / 100) + " " + "Hundred";
    }
    if (n % 100) {
      if (!s.empty()) {
        s += " ";
      }
      s += ConvertHundred(n % 100);
    }
    return s;
  }
};
}  // namespace nstring
