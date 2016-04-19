#ifndef BIT_COUNTER_H
#define BIT_COUNTER_H 1

#include "ruby.h"

#include <limits.h>

#ifdef HAVE_CPUID_H
#include <cpuid.h>
#endif

#include "popcountl.h"
#include "bignum_in_ulong.h"
#include "big_pack.h"

#ifdef HAVE_RB_BIG_XOR
#define BIG_NEG(val) rb_big_xor(val, INT2FIX(-1))
#else
#define BIG_NEG(val) rb_funcall(val, rb_intern("~"), 0)
#endif

#define ALLOCA_THRESHOLD (512 / sizeof(unsigned long))

#endif /* BIT_COUNTER_H */
