#pragma once

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

#ifdef ENV32BIT
	using ptr_t = unsigned __int32;
#endif

#ifdef ENV64BIT
	using ptr_t = unsigned __int64;
#endif