/* header for long popcount (without CPU-native) */

#ifndef POPCOUNTL_H
#define POPCOUNTL_H 1

#ifdef HAVE___BUILTIN_POPCOUNTL
#define POPCOUNTL(x) __builtin_popcountl(x)
#elif SIZEOF_LONG == 8
static inline int POPCOUNTL(unsigned long x){
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
    return (int) x;
}
#elif SIZEOF_LONG == 4
static inline int POPCOUNTL(unsigned long x){
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
    return (int) x;
}
#else
#error Unsupported architecture
#endif
#endif