require 'bit_counter'
require 'benchmark'

def count_string(num)
  return -count_string(~num) if num < 0
  num.to_s(2).count '1'
end

$slicing_bits = 1.size * 8 - 1
$slice_mask = (1 << $slicing_bits) - 1


value = 7**100000
count = 1_000

if value.respond_to?(:bit_length)
  puts "for #{value.bit_length} bits"
end

Benchmark.bm 10 do |r|
  r.report "String" do
    count.times do
      count_string(value)
    end
  end
  r.report "C" do
    count.times do
      BitCounter.count_bignum(value)
    end
  end
end
