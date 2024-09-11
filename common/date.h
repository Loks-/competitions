#pragma once

#include "common/base.h"
#include "common/template.h"

/**
 * @brief A class representing a date with various utility functions.
 */
class Date {
 protected:
  static constexpr unsigned days_in_week = 7;
  static constexpr unsigned months_in_year = 12;
  static constexpr unsigned days_in_year = 365;
  static constexpr unsigned years_in_cycle = 400;
  static constexpr unsigned days_in_cycle =
      days_in_year * years_in_cycle + years_in_cycle / 4 -
      years_in_cycle / 100 + years_in_cycle / 400;

 public:
  /**
   * @brief Get the number of days in a week.
   *
   * @return The number of days in a week.
   */
  static consteval unsigned DaysInWeek() { return days_in_week; }

  /**
   * @brief Get the number of months in a year.
   *
   * @return The number of months in a year.
   */
  static consteval unsigned MonthsInYear() { return months_in_year; }

  /**
   * @brief Get the number of years in a cycle.
   *
   * @return The number of years in a cycle.
   */
  static consteval unsigned YearsInCycle() { return years_in_cycle; }

  /**
   * @brief Check if a month is valid.
   *
   * @param month The month to check.
   * @return true if the month is valid, false otherwise.
   */
  static constexpr bool IsMonthValid(unsigned month) {
    return (0 < month) && (month <= months_in_year);
  }

  /**
   * @brief Assert that a month is valid.
   *
   * @param month The month to check.
   */
  static constexpr void AssertMonth(unsigned month) {
    MetaUse(month);
    assert(IsMonthValid(month));
  }

  /**
   * @brief Check if a year is a leap year.
   *
   * @param year The year to check.
   * @return true if the year is a leap year, false otherwise.
   */
  static constexpr bool IsLeapYear(unsigned year) {
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
  static constexpr bool HasLeapDay(unsigned month, unsigned year) {
    AssertMonth(month);
    return IsLeapYear(year) && (month >= 3);
  }

  /**
   * @brief Get the total number of leap years up to a given year.
   *
   * @param year The year to check.
   * @return The total number of leap years.
   */
  static constexpr unsigned TotalLeapYears(unsigned year) {
    return year / 4 - year / 100 + year / 400;
  }

  /**
   * @brief Get the number of days in a given month.
   *
   * @param month The month to check.
   * @param leap_year Whether the year is a leap year.
   * @return The number of days in the month.
   */
  static constexpr unsigned DaysInMonth(unsigned month, bool leap_year) {
    constexpr unsigned days_in_month[months_in_year]{31, 28, 31, 30, 31, 30,
                                                     31, 31, 30, 31, 30, 31};
    AssertMonth(month);
    return days_in_month[month - 1] + (((month == 2) && leap_year) ? 1 : 0);
  }

  /**
   * @brief Get the number of days in a given month of a given year.
   *
   * @param month The month to check.
   * @param year The year to check.
   * @return The number of days in the month.
   */
  static constexpr unsigned DaysInMonth(unsigned month, unsigned year) {
    return DaysInMonth(month, IsLeapYear(year));
  }

  /**
   * @brief Get the number of days before a given month of a given year.
   *
   * @param month The month to check.
   * @param year The year to check.
   * @return The number of days before the month.
   */
  static constexpr unsigned DaysBeforeMonth(unsigned month, unsigned year) {
    constexpr unsigned days_before_month[months_in_year + 1]{
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    AssertMonth(month);
    return days_before_month[month - 1] + (HasLeapDay(month, year) ? 1 : 0);
  }

  /**
   * @brief Get the number of days in a given year.
   *
   * @param year The year to check.
   * @return The number of days in the year.
   */
  static constexpr unsigned DaysInYear(unsigned year) {
    return days_in_year + (IsLeapYear(year) ? 1 : 0);
  }

  /**
   * @brief Check if a date is valid.
   *
   * @param day The day to check.
   * @param month The month to check.
   * @param year The year to check.
   * @return true if the date is valid, false otherwise.
   */
  static constexpr bool IsDateValid(unsigned day, unsigned month,
                                    unsigned year) {
    return IsMonthValid(month) && (0 < day) &&
           (day <= DaysInMonth(month, year)) && (0 < year);
  }

  /**
   * @brief Assert that a date is valid.
   *
   * @param day The day to check.
   * @param month The month to check.
   * @param year The year to check.
   */
  static constexpr void AssertDate(unsigned day, unsigned month,
                                   unsigned year) {
    MetaUse(day + month + year);
    assert(IsDateValid(day, month, year));
  }

  /**
   * @brief Get the raw day number of a given date.
   *
   * @param day The day of the date.
   * @param month The month of the date.
   * @param year The year of the date.
   * @return The raw day number of the date. Raw day 0 is 01/01/01.
   */
  static constexpr unsigned RawDay(unsigned day, unsigned month,
                                   unsigned year) {
    AssertDate(day, month, year);
    return (year - 1) * days_in_year + TotalLeapYears(year - 1) +
           DaysBeforeMonth(month, year) + (day - 1);
  }

 public:
  static constexpr unsigned SUNDAY = 0;
  static constexpr unsigned MONDAY = 1;
  static constexpr unsigned TUESDAY = 2;
  static constexpr unsigned WEDNESDAY = 3;
  static constexpr unsigned THURSDAY = 4;
  static constexpr unsigned FRIDAY = 5;
  static constexpr unsigned SATURDAY = 6;

  /**
   * @brief Get the day of the week for a given raw day number.
   *
   * @param raw_day The raw day number.
   * @return The day of the week.
   */
  static constexpr unsigned DayOfWeek(unsigned raw_day) {
    return (raw_day + 1) % days_in_week;
  }

  /**
   * @brief Get the day of the week for a given date.
   *
   * @param day The day of the date.
   * @param month The month of the date.
   * @param year The year of the date.
   * @return The day of the week.
   */
  static constexpr unsigned DayOfWeek(unsigned day, unsigned month,
                                      unsigned year) {
    return DayOfWeek(RawDay(day, month, year));
  }

 public:
  /**
   * @brief Default constructor initializing date to 01/01/01.
   */
  constexpr Date() : day(1), month(1), year(1) {}

  /**
   * @brief Constructor initializing date to a given day, month, and year.
   *
   * @param _day The day of the date.
   * @param _month The month of the date.
   * @param _year The year of the date.
   */
  constexpr Date(unsigned _day, unsigned _month, unsigned _year)
      : day(_day), month(_month), year(_year) {
    assert(ValidDate());
  }

  /**
   * @brief Get the day of the date.
   *
   * @return The day of the date.
   */
  constexpr unsigned Day() const { return day; }

  /**
   * @brief Get the month of the date.
   *
   * @return The month of the date.
   */
  constexpr unsigned Month() const { return month; }

  /**
   * @brief Get the year of the date.
   *
   * @return The year of the date.
   */
  constexpr unsigned Year() const { return year; }

  /**
   * @brief Get the raw day number of the date.
   *
   * @return The raw day number of the date.
   */
  constexpr unsigned RawDay() const { return RawDay(day, month, year); }

  /**
   * @brief Get the day of the week of the date.
   *
   * @return The day of the week of the date.
   */
  constexpr bool ValidDate() const { return IsDateValid(day, month, year); }

  /**
   * @brief Get the day of the week of the date.
   *
   * @return The day of the week of the date.
   */
  constexpr unsigned DayOfWeek() const { return DayOfWeek(day, month, year); }

  /**
   * @brief Get the date of the first day next year.
   *
   * @return The date of the first day next year.
   */
  constexpr Date NextYear() const { return Date(1, 1, year + 1); }

  /**
   * @brief Get the date of the first day next month.
   *
   * @return The date of the first day next month.
   */
  constexpr Date NextMonth() const {
    return (month == months_in_year) ? NextYear() : Date(1, month + 1, year);
  }

  /**
   * @brief Equality operator.
   *
   * @param r The date to compare with.
   * @return true if the dates are equal, false otherwise.
   */
  constexpr bool operator==(const Date& r) const {
    return (day == r.day) && (month == r.month) && (year == r.year);
  }

  /**
   * @brief Less-than operator.
   *
   * @param r The date to compare with.
   * @return true if this date is less than the other date, false otherwise.
   */
  constexpr bool operator<(const Date& r) const {
    return (year != r.year)     ? (year < r.year)
           : (month != r.month) ? (month < r.month)
                                : day < r.day;
  }

  /**
   * @brief Less-than-or-equal operator.
   *
   * @param r The date to compare with.
   * @return true if this date is less than or equal to the other date, false
   * otherwise.
   */
  constexpr bool operator<=(const Date& r) const {
    return (year != r.year)     ? (year < r.year)
           : (month != r.month) ? (month < r.month)
                                : day <= r.day;
  }

 protected:
  unsigned day, month, year;
};
