#pragma once

enum class FunctionType {
  NONE,
  SUCCESSOR = 5,
  PREDECESSOR = 6,
  SUM = 7,
  PRODUCT = 9,
  DIVISION = 10,
  EQUALITY = 11,
  STRICT_LESS = 12,
  MODULATE = 13,
  DEMODULATE = 14,
  SEND = 15,
  NEGATE = 16,
  S_COMBINATOR = 18,
  C_COMBINATOR = 19,
  B_COMBINATOR = 20,
  K_COMBINATOR = 21,   // Also works as 'True' from boolean operators
  FALSE__SECOND = 22,  // Also works as 'False' from boolean operators
  POWER_OF_TWO = 23,
  I_COMBINATOR = 24,
  CONS__PAIR = 25,
  CAR__FIRST = 26,
  CDR__TAIL = 27,
  NIL__EMPTY_LIST = 28,
  IS_NIL = 29,
  VECTOR = 31,
  DRAW = 32,
  MULTIPLE_DRAW = 34,
  IF0 = 37,
  INTERACT = 38,
  GALAXY = 42,
  // ...
  MODEM = 101,
  F38 = 102,
  END
};
