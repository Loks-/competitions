#pragma once

inline unsigned GrowBaseCost(unsigned new_size) { return (1u << new_size) - 1; }

inline unsigned CompleteCost() { return 4; }
