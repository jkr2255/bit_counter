#
# module for bit counting
#
module BitCounter

  module CImpl

    # tentative code
    def count_bignum(num)
      raise TypeError unless num.is_a?(::Bignum)
      return -count_bignum(~num) if num < 0
      num.to_s(2).count('1')
    end
  end

  extend CImpl

end
