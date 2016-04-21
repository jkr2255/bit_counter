require 'bit_counter'
require 'benchmark'

def count_string(num)
  return -count_string(~num) if num < 0
  num.to_s(2).count '1'
end

$table = 128.times.map { |i| BitCounter.count(i) }

def count_from_pack(num)
  return -count_string(~num) if num < 0
  [num].pack('w').chars.reduce(0) { |cnt, c| cnt + $table[c.ord & 0x7f] }
end


value = 7**100000
count = 1_000

if value.respond_to?(:bit_length)
  puts "for #{value.bit_length} bits, #{count} times"
end

Benchmark.bm 10 do |r|
  r.report "String" do
    count.times do
      count_string(value)
    end
  end
  r.report "Pack" do
    count.times do
      count_from_pack(value)
    end
  end
  r.report "C" do
    count.times do
      BitCounter.count_bignum(value)
    end
  end
end

value = 2**80 - 6
count = 1_000_000

if value.respond_to?(:bit_length)
  puts "for #{value.bit_length} bits, #{count} times"
end

Benchmark.bm 10 do |r|
  r.report "String" do
    count.times do
      count_string(value)
    end
  end
  r.report "Pack" do
    count.times do
      count_from_pack(value)
    end
  end
  r.report "C" do
    count.times do
      BitCounter.count_bignum(value)
    end
  end
end
