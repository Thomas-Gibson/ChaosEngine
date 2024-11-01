#pragma once

#include "chaos_api.h"

using s8 = char;
using s16 = short;
using s32 = int;
using s64 = long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using r32 = float;
using r64 = double;

using wstring = const wchar_t*;
using lstring = const char*;

using function = void*;

struct Color
{
	u8 r, g, b;
};
