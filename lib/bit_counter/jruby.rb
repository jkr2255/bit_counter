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

end
