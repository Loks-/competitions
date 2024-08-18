#pragma once

#include "common/hash/hash.h"
#include "common/hash/value.h"

template <class T>
using DHash = nhash::HashSafe<T>;
