#include "bit_counter.h"

VALUE rb_mBitCounter;

static VALUE bitcounter_cimpl_count_fixnum(VALUE self, VALUE num){
    long l_num = NUM2LONG(num);
    int val = POPCOUNTL(l_num);
    if(l_num < 0) val -= sizeof(long) * CHAR_BIT;
    return INT2FIX(val);
}

#ifdef HAVE___GET_CPUID
static VALUE bitcounter_cimpl_cpu_popcnt_p(VALUE self){
    unsigned int eax, ebx, ecx = 0, edx;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    return (ecx & bit_POPCNT) ? Qtrue : Qfalse;
}
#else
static VALUE bitcounter_cimpl_cpu_popcnt_p(VALUE self){
    return Qfalse;
}
#endif

#ifdef HAVE_POPCNT_GCC_ASM
static VALUE bitcounter_cimpl_count_fixnum_asm(VALUE self, VALUE num){
    long l_num = NUM2LONG(num);
    long val;
    __asm__ volatile ("POPCNT %1, %0;": "=r"(val): "r"(l_num) : );
    if(l_num < 0) val -= sizeof(long) * CHAR_BIT;
    return INT2FIX(val);
}
#define ASM_POPCOUNT 1
#else
static VALUE bitcounter_cimpl_count_fixnum_asm(VALUE self, VALUE num){
    /* dummy function for C compiler, never called from Ruby */
    return Qnil;
}
#define ASM_POPCOUNT 0
#endif

/*  for bignum */

#ifdef HAVE_RB_BIG_PACK
static VALUE bitcounter_cimpl_count_bignum(VALUE self, VALUE num){
    int negated = 0;
    unsigned long * packed;
    size_t words, i;
    LONG_LONG ret = 0;
    if(FIXNUM_P(num)){
        return bitcounter_cimpl_count_fixnum(self, num);
    }
    Check_Type(num, T_BIGNUM);
    negated = RBIGNUM_NEGATIVE_P(num);
    words = BIGNUM_IN_ULONG(num);
    packed = ALLOC_N(unsigned long, words);
    rb_big_pack(num, packed, words);
    for(i = 0; i < words; ++i){
        ret += POPCOUNTL(packed[i]);
    }
    if(negated){
        ret -= words * sizeof(unsigned long) * CHAR_BIT;
    }
    xfree(packed);
    return LL2NUM(ret);
}
#else
static VALUE bitcounter_cimpl_count_bignum(VALUE self, VALUE num){
    /* stub function */
    if(FIXNUM_P(num)){
        return bitcounter_cimpl_count_fixnum(self, num);
    }
    Check_Type(num, T_BIGNUM);

    return Qnil;
}
#endif

void
Init_bit_counter(void)
{
  VALUE rb_mCImpl;
  VALUE have_cpu_popcnt;
  rb_mBitCounter = rb_define_module("BitCounter");
  rb_mCImpl = rb_define_module_under(rb_mBitCounter, "CImpl");
  have_cpu_popcnt = bitcounter_cimpl_cpu_popcnt_p(rb_mCImpl);
  rb_define_module_function(rb_mCImpl, "cpu_popcnt?", bitcounter_cimpl_cpu_popcnt_p, 0);
  if(ASM_POPCOUNT && have_cpu_popcnt){
      rb_define_method(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum_asm, 1);
  }else{
      rb_define_method(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum, 1);
  }
  rb_define_method(rb_mCImpl, "test", bitcounter_cimpl_count_bignum, 1);
}
