#pragma once

#include "common/base.h"
#include "common/template.h"

class Date {
 protected:
  static const unsigned days_in_week = 7;
  static const unsigned months_in_year = 12;
  static const unsigned days_in_year = 365;
  static const unsigned years_in_cycle = 400;
  static const unsigned days_in_cycle =
      days_in_year * years_in_cycle + years_in_cycle / 4 -
      years_in_cycle / 100 + years_in_cycle / 400;

 public:
  consteval static unsigned DaysInWeek() { return days_in_week; }
  consteval static unsigned MonthsInYear() { return months_in_year; }
  consteval static unsigned YearsInCycle() { return years_in_cycle; }

  constexpr static bool IsMonthValid(unsigned month) {
    return (0 < month) && (month <= months_in_year);
  }

  constexpr static void AssertMonth(unsigned month) {
    FakeUse(month);
    assert(IsMonthValid(month));
  }

  constexpr static bool IsLeapYear(unsigned year) {
    return (year % 4)     ? false
           : (year % 100) ? true
           : (year % 400) ? false
                          : true;
  }

  constexpr static bool HasLeapDay(unsigned month, unsigned year) {
    AssertMonth(month);
    return IsLeapYear(year) && (month >= 3);
  }

  constexpr static unsigned TotalLeapYears(unsigned year) {
    return year / 4 - year / 100 + year / 400;
  }

  constexpr static unsigned DaysInMonth(unsigned month, bool leap_year) {
    const unsigned days_in_month[months_in_year]{31, 28, 31, 30, 31, 30,
                                                 31, 31, 30, 31, 30, 31};
    AssertMonth(month);
    return days_in_month[month - 1] + (((month == 2) && leap_year) ? 1 : 0);
  }

  constexpr static unsigned DaysInMonth(unsigned month, unsigned year) {
    return DaysInMonth(month, IsLeapYear(year));
  }

  constexpr static unsigned DaysBeforeMonth(unsigned month, unsigned year) {
    constexpr unsigned days_before_month[months_in_year + 1]{
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    AssertMonth(month);
    return days_before_month[month - 1] + (HasLeapDay(month, year) ? 1 : 0);
  }

  constexpr static unsigned DaysInYear(unsigned year) {
    return days_in_year + (IsLeapYear(year) ? 1 : 0);
  }

  constexpr static bool IsDateValid(unsigned day, unsigned month,
                                    unsigned year) {
    return IsMonthValid(month) && (0 < day) &&
           (day <= DaysInMonth(month, year)) && (0 < year);
  }

  constexpr static void AssertDate(unsigned day, unsigned month,
                                   unsigned year) {
    FakeUse(day + month + year);
    assert(IsDateValid(day, month, year));
  }

 public:
  // Raw day 0 is 01/01/01
  constexpr static unsigned RawDay(unsigned day, unsigned month,
                                   unsigned year) {
    AssertDate(day, month, year);
    return (year - 1) * days_in_year + TotalLeapYears(year - 1) +
           DaysBeforeMonth(month, year) + (day - 1);
  }

 public:
  static const unsigned SUNDAY = 0;
  static const unsigned MONDAY = 1;
  static const unsigned TUESDAY = 2;
  static const unsigned WEDNESDAY = 3;
  static const unsigned THURSDAY = 4;
  static const unsigned FRIDAY = 5;
  static const unsigned SATURDAY = 6;

  constexpr static unsigned DayOfWeek(unsigned raw_day) {
    return (raw_day + 1) % days_in_week;
  }

  constexpr static unsigned DayOfWeek(unsigned day, unsigned month,
                                      unsigned year) {
    return DayOfWeek(RawDay(day, month, year));
  }

 protected:
  unsigned day, month, year;

 public:
  constexpr Date() : day(1), month(1), year(1) {}
  constexpr Date(unsigned _day, unsigned _month, unsigned _year)
      : day(_day), month(_month), year(_year) {
    assert(ValidDate());
  }

  constexpr unsigned Day() const { return day; }
  constexpr unsigned Month() const { return month; }
  constexpr unsigned Year() const { return year; }
  constexpr unsigned RawDay() const { return RawDay(day, month, year); }

  constexpr bool ValidDate() const { return IsDateValid(day, month, year); }
  constexpr unsigned DayOfWeek() const { return DayOfWeek(day, month, year); }

  constexpr Date NextYear() const { return Date(1, 1, year + 1); }
  constexpr Date NextMonth() const {
    return (month == months_in_year) ? NextYear() : Date(1, month + 1, year);
  }

  constexpr bool operator==(const Date& r) const {
    return (day == r.day) && (month == r.month) && (year == r.year);
  }

  constexpr bool operator<(const Date& r) const {
    return (year != r.year)     ? (year < r.year)
           : (month != r.month) ? (month < r.month)
                                : day < r.day;
  }

  constexpr bool operator<=(const Date& r) const {
    return (year != r.year)     ? (year < r.year)
           : (month != r.month) ? (month < r.month)
                                : day <= r.day;
  }
};
