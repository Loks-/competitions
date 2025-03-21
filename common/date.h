#pragma once

#include "common/base.h"

/**
 * @brief A class representing a date with various utility functions.
 */
class Date {
 protected:
  static constexpr unsigned kDaysInWeek = 7;
  static constexpr unsigned kMonthsInYear = 12;
  static constexpr unsigned kDaysInYear = 365;
  static constexpr unsigned kYearsInCycle = 400;
  static constexpr unsigned kDaysInCycle =
      kDaysInYear * kYearsInCycle + kYearsInCycle / 4 - kYearsInCycle / 100 +
      kYearsInCycle / 400;

 public:
  /**
   * @brief Get the number of days in a week.
   *
   * @return The number of days in a week.
   */
  [[nodiscard]] static consteval unsigned days_in_week() { return kDaysInWeek; }

  /**
   * @brief Get the number of months in a year.
   *
   * @return The number of months in a year.
   */
  [[nodiscard]] static consteval unsigned months_in_year() {
    return kMonthsInYear;
  }

  /**
   * @brief Get the number of years in a cycle.
   *
   * @return The number of years in a cycle.
   */
  [[nodiscard]] static consteval unsigned years_in_cycle() {
    return kYearsInCycle;
  }

  /**
   * @brief Check if a month is valid.
   *
   * @param month The month to check.
   * @return true if the month is valid, false otherwise.
   */
  [[nodiscard]] static constexpr bool is_month_valid(unsigned month) {
    return (0 < month) && (month <= kMonthsInYear);
  }

  /**
   * @brief Assert that a month is valid.
   *
   * @param month The month to check.
   */
  static constexpr void assert_month([[maybe_unused]] unsigned month) {
    assert(is_month_valid(month));
  }

  /**
   * @brief Check if a year is a leap year.
   *
   * @param year The year to check.
   * @return true if the year is a leap year, false otherwise.
   */
  [[nodiscard]] static constexpr bool is_leap_year(unsigned year) {
    return (year % 4)     ? false
           : (year % 100) ? true
           : (year % 400) ? false
                          : true;
  }

  /**
   * @brief Check if a given year has a leap day before the given month.
   *
   * @param month The month to check.
   * @param year The year to check.
   * @return true if the year has a leap day before the given month, false
   * otherwise.
   */
  [[nodiscard]] static constexpr bool has_leap_day(unsigned month,
                                                   unsigned year) {
    assert_month(month);
    return is_leap_year(year) && (month >= 3);
  }

  /**
   * @brief Get the total number of leap years up to a given year.
   *
   * @param year The year to check.
   * @return The total number of leap years.
   */
  [[nodiscard]] static constexpr unsigned total_leap_years(unsigned year) {
    return year / 4 - year / 100 + year / 400;
  }

  /**
   * @brief Get the number of days in a given month.
   *
   * @param month The month to check.
   * @param leap_year Whether the year is a leap year.
   * @return The number of days in the month.
   */
  [[nodiscard]] static constexpr unsigned days_in_month(unsigned month,
                                                        bool leap_year) {
    constexpr unsigned kDaysInMonth[kMonthsInYear]{31, 28, 31, 30, 31, 30,
                                                   31, 31, 30, 31, 30, 31};
    assert_month(month);
    return kDaysInMonth[month - 1] + (((month == 2) && leap_year) ? 1 : 0);
  }

  /**
   * @brief Get the number of days in a given month of a given year.
   *
   * @param month The month to check.
   * @param year The year to check.
   * @return The number of days in the month.
   */
  [[nodiscard]] static constexpr unsigned days_in_month(unsigned month,
                                                        unsigned year) {
    return days_in_month(month, is_leap_year(year));
  }

  /**
   * @brief Get the number of days before a given month of a given year.
   *
   * @param month The month to check.
   * @param year The year to check.
   * @return The number of days before the month.
   */
  [[nodiscard]] static constexpr unsigned days_before_month(unsigned month,
                                                            unsigned year) {
    constexpr unsigned kDaysBeforeMonth[kMonthsInYear + 1]{
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    assert_month(month);
    return kDaysBeforeMonth[month - 1] + (has_leap_day(month, year) ? 1 : 0);
  }

  /**
   * @brief Get the number of days in a given year.
   *
   * @param year The year to check.
   * @return The number of days in the year.
   */
  [[nodiscard]] static constexpr unsigned days_in_year(unsigned year) {
    return kDaysInYear + (is_leap_year(year) ? 1 : 0);
  }

  /**
   * @brief Check if a date is valid.
   *
   * @param day The day to check.
   * @param month The month to check.
   * @param year The year to check.
   * @return true if the date is valid, false otherwise.
   */
  [[nodiscard]] static constexpr bool is_date_valid(unsigned day,
                                                    unsigned month,
                                                    unsigned year) {
    return is_month_valid(month) && (0 < day) &&
           (day <= days_in_month(month, year)) && (0 < year);
  }

  /**
   * @brief Assert that a date is valid.
   *
   * @param day The day to check.
   * @param month The month to check.
   * @param year The year to check.
   */
  static constexpr void assert_date([[maybe_unused]] unsigned day,
                                    [[maybe_unused]] unsigned month,
                                    [[maybe_unused]] unsigned year) {
    assert(is_date_valid(day, month, year));
  }

  /**
   * @brief Get the raw day number of a given date.
   *
   * @param day The day of the date.
   * @param month The month of the date.
   * @param year The year of the date.
   * @return The raw day number of the date. Raw day 0 is 01/01/01.
   */
  [[nodiscard]] static constexpr unsigned raw_day(unsigned day, unsigned month,
                                                  unsigned year) {
    assert_date(day, month, year);
    return (year - 1) * kDaysInYear + total_leap_years(year - 1) +
           days_before_month(month, year) + (day - 1);
  }

 public:
  static constexpr unsigned kSunday = 0;
  static constexpr unsigned kMonday = 1;
  static constexpr unsigned kTuesday = 2;
  static constexpr unsigned kWednesday = 3;
  static constexpr unsigned kThursday = 4;
  static constexpr unsigned kFriday = 5;
  static constexpr unsigned kSaturday = 6;

  /**
   * @brief Get the day of the week for a given raw day number.
   *
   * @param raw_day The raw day number.
   * @return The day of the week.
   */
  [[nodiscard]] static constexpr unsigned day_of_week(unsigned raw_day) {
    return (raw_day + 1) % kDaysInWeek;
  }

  /**
   * @brief Get the day of the week for a given date.
   *
   * @param day The day of the date.
   * @param month The month of the date.
   * @param year The year of the date.
   * @return The day of the week.
   */
  [[nodiscard]] static constexpr unsigned day_of_week(unsigned day,
                                                      unsigned month,
                                                      unsigned year) {
    return day_of_week(raw_day(day, month, year));
  }

 public:
  /**
   * @brief Default constructor initializing date to 01/01/01.
   */
  constexpr Date() : day_(1), month_(1), year_(1) {}

  /**
   * @brief Constructor initializing date to a given day, month, and year.
   *
   * @param day The day of the date.
   * @param month The month of the date.
   * @param year The year of the date.
   */
  constexpr Date(unsigned day, unsigned month, unsigned year)
      : day_(day), month_(month), year_(year) {
    assert(is_date_valid());
  }

  /**
   * @brief Get the day of the date.
   *
   * @return The day of the date.
   */
  [[nodiscard]] constexpr unsigned day() const { return day_; }

  /**
   * @brief Get the month of the date.
   *
   * @return The month of the date.
   */
  [[nodiscard]] constexpr unsigned month() const { return month_; }

  /**
   * @brief Get the year of the date.
   *
   * @return The year of the date.
   */
  [[nodiscard]] constexpr unsigned year() const { return year_; }

  /**
   * @brief Get the raw day number of the date.
   *
   * @return The raw day number of the date.
   */
  [[nodiscard]] constexpr unsigned raw_day() const {
    return raw_day(day_, month_, year_);
  }

  /**
   * @brief Check if the date is valid.
   *
   * @return true if the date is valid, false otherwise.
   */
  [[nodiscard]] constexpr bool is_date_valid() const {
    return is_date_valid(day_, month_, year_);
  }

  /**
   * @brief Get the day of the week of the date.
   *
   * @return The day of the week of the date.
   */
  [[nodiscard]] constexpr unsigned day_of_week() const {
    return day_of_week(day_, month_, year_);
  }

  /**
   * @brief Get the date of the first day next year.
   *
   * @return The date of the first day next year.
   */
  [[nodiscard]] constexpr Date next_year() const {
    return Date(1, 1, year_ + 1);
  }

  /**
   * @brief Get the date of the first day next month.
   *
   * @return The date of the first day next month.
   */
  [[nodiscard]] constexpr Date next_month() const {
    return (month_ == kMonthsInYear) ? next_year() : Date(1, month_ + 1, year_);
  }

  /**
   * @brief Equality operator.
   *
   * @param other The date to compare with.
   * @return true if the dates are equal, false otherwise.
   */
  [[nodiscard]] constexpr bool operator==(const Date& other) const {
    return (day_ == other.day_) && (month_ == other.month_) &&
           (year_ == other.year_);
  }

  /**
   * @brief Less-than operator.
   *
   * @param other The date to compare with.
   * @return true if this date is less than the other date, false otherwise.
   */
  [[nodiscard]] constexpr bool operator<(const Date& other) const {
    return (year_ != other.year_)     ? (year_ < other.year_)
           : (month_ != other.month_) ? (month_ < other.month_)
                                      : day_ < other.day_;
  }

  /**
   * @brief Less-than-or-equal operator.
   *
   * @param other The date to compare with.
   * @return true if this date is less than or equal to the other date, false
   * otherwise.
   */
  [[nodiscard]] constexpr bool operator<=(const Date& other) const {
    return (year_ != other.year_)     ? (year_ < other.year_)
           : (month_ != other.month_) ? (month_ < other.month_)
                                      : day_ <= other.day_;
  }

 protected:
  unsigned day_, month_, year_;
};
