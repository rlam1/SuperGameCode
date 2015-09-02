#pragma once
// Operations on bitifield values
namespace Bitfield{

const int downBIT = 1 << 0;
const int upBIT = 1 << 1;
const int leftBIT = 1 << 2;
const int rightBIT = 1 << 3;

inline int set(int field, int value);
inline bool test(int field, int value);
inline void clear(int field, int value);
}
