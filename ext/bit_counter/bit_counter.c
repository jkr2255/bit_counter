#include "bit_counter.h"

VALUE rb_mBitCounter;

#ifdef HAVE___BUILTIN_POPCOUNTL
VALUE bitcounter_cimpl_count_fixnum(VALUE self, VALUE num){
    long l_num = NUM2LONG(num);
    int val = __builtin_popcountl(l_num);
    if(l_num < 0) val -= sizeof(long) * CHAR_BIT;
    return INT2FIX(val);
}
#endif

void
Init_bit_counter(void)
{
  VALUE rb_mCImpl;
  rb_mBitCounter = rb_define_module("BitCounter");
  rb_mCImpl = rb_define_module_under(rb_mBitCounter, "CImpl");
#ifdef HAVE___BUILTIN_POPCOUNTL
  rb_define_module_function(rb_mCImpl, "count_fixnum", bitcounter_cimpl_count_fixnum, 1);
#endif
}
