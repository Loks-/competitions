#pragma once

class TEmpty {};

template <bool>
class TFakeBool {};
using TFakeFalse = TFakeBool<false>;
using TFakeTrue = TFakeBool<true>;

template <unsigned>
class TFakeUnsigned {};

template <class T>
class TFakeType {};

template <class T>
inline void FakeUse(const T&) {}
