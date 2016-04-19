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

#ifdef BIG_PACK
static VALUE bitcounter_cimpl_count_bignum(VALUE self, VALUE num){
    int negated = 0;
    unsigned long * packed;
    VALUE abs_num;
    size_t words, i;
    LONG_LONG ret = 0;
    if(FIXNUM_P(num)){
        return bitcounter_cimpl_count_fixnum(self, num);
    }
    Check_Type(num, T_BIGNUM);
    if(RBIGNUM_NEGATIVE_P(num)){
        negated = 1;
        abs_num = BIG_NEG(num);
    }else{
        abs_num = num;
    }
    words = BIGNUM_IN_ULONG(abs_num);
    packed = ALLOC_N(unsigned long, words);
    BIG_PACK(abs_num, packed, words);
    for(i = 0; i < words; ++i){
        ret += POPCOUNTL(packed[i]);
    }
    if(negated) ret = -ret;
    xfree(packed);
    return LL2NUM(ret);
}
#else
static VALUE bitcounter_cimpl_count_bignum(VALUE self, VALUE num){
    /* as a last resort, using Array#pack */
    int negated = 0;
    VALUE abs_num, arr, packed;
    unsigned char * p;
    unsigned long * lp;
    long length, i;
    LONG_LONG ret = 0;
    static ID id_pack = 0;
    if(FIXNUM_P(num)){
        return bitcounter_cimpl_count_fixnum(self, num);
    }
    Check_Type(num, T_BIGNUM);
    if(!id_pack){
        id_pack = rb_intern("pack");
    }
    if(RBIGNUM_NEGATIVE_P(num)){
        negated = 1;
        abs_num = BIG_NEG(num);
    }else{
        abs_num = num;
    }
    arr = rb_ary_new3(1, abs_num);
    packed = rb_funcall(arr, id_pack, 1, rb_str_new2("w"));
    StringValue(packed);
    length = RSTRING_LEN(packed);
    p = RSTRING_PTR(packed);
    lp = (unsigned long *) p;
    for(i = 0; i < (length / sizeof(unsigned long)); ++i){
        ret += POPCOUNTL(lp[i] & BER_MASK);
    }
    i *= sizeof(unsigned long);
    for( ; i < length; ++i){
        ret += POPCOUNTL(p[i] & 0x7f);
    }
    if(negated) ret = -ret;
    return LL2NUM(ret);
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
  rb_define_method(rb_mCImpl, "count_bignum", bitcounter_cimpl_count_bignum, 1);
}
