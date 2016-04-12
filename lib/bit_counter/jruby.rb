require 'java'

#
# module for bit counting
#
module BitCounter

  #
  # module for Java-specific codes of BitCounter.
  # @note not intended for direct use.
  #
  module JavaImpl
    def count_fixnum(num)
      raise TypeError unless num.is_a?(::Fixnum)
      count = Java::JavaLang::Long.bitCount(num)
      num >= 0 ? count : count - 64
    end

    def count_bignum(num)
      raise TypeError unless num.is_a?(::Bignum)
      count = num.to_java.bitCount
      num >= 0 ? count : -count
    end
  end

  extend JavaImpl

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
