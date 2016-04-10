require 'bit_counter'

# :nodoc:
class Integer
  #
  # calling BitCounter.#count with self
  #
  def bit_count
    BitCounter.count(self)
  end
end
