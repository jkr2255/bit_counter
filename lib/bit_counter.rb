require "bit_counter/version"

if RUBY_PLATFORM =~ /java/
  require 'bit_counter/jruby'
else
  require "bit_counter/bit_counter"
  require 'bit_counter/cruby'
end

#
# module for bit counting
#
module BitCounter

  module_function

  #
  # counts bits in Integer.
  # @return [Integer] num the number of bits.
  #   if positive number is given, count 1 bits.
  #   if negative number is given, count 0 bits and -(count) is returned.
  # @raise [TypeError] when non-Integer was given.
  #
  def count(num)
    case num
    when Fixnum
      count_fixnum(num)
    when Bignum
      count_bignum(num)
    else
      raise TypeError
    end
  end
end
