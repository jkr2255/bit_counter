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
}
