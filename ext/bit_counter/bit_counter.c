#include "bit_counter.h"

VALUE rb_mBitCounter;

#ifdef HAVE___BUILTIN_POPCOUNTL
static VALUE bitcounter_cimpl_count_fixnum(VALUE self, VALUE num){
    long l_num = NUM2LONG(num);
    int val = __builtin_popcountl(l_num);
    if(l_num < 0) val -= sizeof(long) * CHAR_BIT;
    return INT2FIX(val);
}
#elif SIZEOF_LONG == 8
static VALUE bitcounter_cimpl_count_fixnum(VALUE self, VALUE num){
    long l_num = NUM2LONG(num);
    unsigned long x = l_num;
    x = ((x & 0xaaaaaaaaaaaaaaaaUL) >> 1)
      +  (x & 0x5555555555555555UL);
    x = ((x & 0xccccccccccccccccUL) >> 2)
      +  (x & 0x3333333333333333UL);
    x = ((x & 0xf0f0f0f0f0f0f0f0UL) >> 4)
      +  (x & 0x0f0f0f0f0f0f0f0fUL);
    x = ((x & 0xff00ff00ff00ff00UL) >> 8)
      +  (x & 0x00ff00ff00ff00ffUL);
    x = ((x & 0xffff0000ffff0000UL) >> 16)
      +  (x & 0x0000ffff0000ffffUL);
    x = ((x & 0xffffffff00000000UL) >> 32)
      +  (x & 0x00000000ffffffffUL);
    if(l_num < 0) x -= sizeof(long) * CHAR_BIT;
    return LONG2FIX(x);
}
#elif SIZEOF_LONG == 4
static VALUE bitcounter_cimpl_count_fixnum(VALUE self, VALUE num){
    long l_num = NUM2LONG(num);
    unsigned long x = l_num;
    x = ((x & 0xaaaaaaaaUL) >> 1)
      +  (x & 0x55555555UL);
    x = ((x & 0xccccccccUL) >> 2)
      +  (x & 0x33333333UL);
    x = ((x & 0xf0f0f0f0UL) >> 4)
      +  (x & 0x0f0f0f0fUL);
    x = ((x & 0xff00ff00UL) >> 8)
      +  (x & 0x00ff00ffUL);
    x = ((x & 0xffff0000UL) >> 16)
      +  (x & 0x0000ffffUL);
    if(l_num < 0) x -= sizeof(long) * CHAR_BIT;
    return LONG2FIX(x);
}
#else
#error Unsupported architecture
#endif

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
#endif

void
Init_bit_counter(void)
{
  VALUE rb_mCImpl;
  VALUE have_cpu_popcnt;
  rb_mBitCounter = rb_define_module("BitCounter");
  rb_mCImpl = rb_define_module_under(rb_mBitCounter, "CImpl");
  have_cpu_popcnt = bitcounter_cimpl_cpu_popcnt_p(rb_mCImpl);
  rb_define_module_function(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum, 1);
  rb_define_module_function(rb_mCImpl, "cpu_popcnt?", bitcounter_cimpl_cpu_popcnt_p, 0);
#ifdef HAVE_POPCNT_GCC_ASM
  if(have_cpu_popcnt){
      rb_define_module_function(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum_asm, 1);
  }else{
      rb_define_module_function(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum, 1);
  }
#else
  rb_define_module_function(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum, 1);
#endif
}
