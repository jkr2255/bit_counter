require 'bit_counter'
require 'benchmark'

def count_string(num)
  return -count_string(~num) if num < 0
  num.to_s(2).count '1'
end

def count_arithmeric_32(x)
  return -count_string(~x) if x < 0
  m1 = 0x55555555
  m2 = 0x33333333
  m4 = 0x0f0f0f0f
  x -= (x >> 1) & m1
  x = (x & m2) + ((x >> 2) & m2)
  x = (x + (x >> 4)) & m4
  x += x >> 8
  (x + (x >> 16)) & 0x3f
end

value = 0xFEDC_BA98
count = 10_000_000

Benchmark.bm 10 do |r|
  r.report "String" do
    count.times do
      count_string(value)
    end
  end
  r.report "Arithmeric" do
    count.times do
      count_arithmeric_32(value)
    end
  end
  r.report "Native" do
    count.times do
      BitCounter.count_fixnum(value)
    end
  end
end
