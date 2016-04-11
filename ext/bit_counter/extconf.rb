require "mkmf"

have_func('__builtin_popcountl(1)')
create_makefile("bit_counter/bit_counter")
