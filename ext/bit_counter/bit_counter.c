#include "bit_counter.h"

VALUE rb_mBitCounter;

void
Init_bit_counter(void)
{
  rb_mBitCounter = rb_define_module("BitCounter");
}
