require 'bit_counter'

# :nodoc:
class Fixnum
  #
  # calling BitCounter.#count with self
  #
  def bit_count
    BitCounter.count_fixnum(self)
  end
end

# :nodoc:
class Bignum
  #
  # calling BitCounter.#count with self
  #
  def bit_count
    BitCounter.count_bignum(self)
  end
end
