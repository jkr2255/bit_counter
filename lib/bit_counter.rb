require "bit_counter/version"
require 'bit_utils'

#
# module for bit counting
# @deprecated use `bit_utils`.
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
    BitUtils.popcount(num)
  end
end
