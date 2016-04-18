#ifndef BIGNUM_IN_ULONG_H
#define BIGNUM_IN_ULONG_H 1

#ifdef HAVE_RB_ABSINT_NUMWORDS
#define BIGNUM_IN_ULONG(v) rb_absint_numwords(v, sizeof(unsigned long), NULL)
#else
static size_t BIGNUM_IN_ULONG(VALUE v){
    static ID size_id = 0;
    VALUE ret_val;
    if(!size_id){
        size_id = rb_intern("size");
    }
    ret_val = rb_funcall(v, size_id, 0);
    return (NUM2SIZET(ret_val) + sizeof(size_t) - 1) / sizeof(size_t);
}
#endif

#endif
