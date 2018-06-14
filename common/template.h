#pragma once

template<bool> class TFakeBool {};
using TFakeFalse = TFakeBool<false>;
using TFakeTrue = TFakeBool<true>;

template<class T> class TFakeType{};
