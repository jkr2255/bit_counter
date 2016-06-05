require 'bit_utils'

# :nodoc:
class Fixnum
  def bit_count
    BitUtils.popcount_fixnum(self)
  end
end

# :nodoc:
class Bignum
  def bit_count
    BitUtils.popcount_bignum(self)
  end
end
