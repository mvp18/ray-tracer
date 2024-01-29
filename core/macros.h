#pragma once

#ifdef _MSC_VER
#define ALIGN(n) __declspec(align(n))
#else
#define ALIGN(n) __attribute__((aligned(n)))
#endif


// The following macro will prevent warnings for unused variables.
// Use with care... Maybe you actually need the variable?
#define CG_UNUSED(expr) do { (void)(expr); } while (false)