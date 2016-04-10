require 'spec_helper'

describe BitCounter do
  it 'has a version number' do
    expect(BitCounter::VERSION).not_to be nil
  end

  describe '#count' do
    it 'is a module function' do
      expect(BitCounter.methods).to include(:count)
      expect(BitCounter.singleton_methods).to include(:count)
    end

    it 'raises TypeError if other than Integer was passed' do
      expect { BitCounter.count(2.0) }.to raise_error(TypeError)
      expect { BitCounter.count(nil) }.to raise_error(TypeError)
    end

    expecteds = {
      0 => 0,
      1 => 1,
      0xf0 => 4,
      # threshold in 32-bit Fixnum
      0x3fff_ffff => 30,
      0x4000_0000 => 1,
      # threshold in 64-bit fixnum
      0x3fff_ffff_ffff_ffff => 62,
      0x4000_0000_0000_0000 => 1,
      # Bignum
      0x3333_3333_3333_3333_3333_3333 => 48,
      # negative
      -1 => 0,
      -2 => -1,
      -0x10 => -4,
      # threshold in 32-bit Fixnum
      -0x4000_0000 => -30,
      -0x4000_0001 => -1,
      # threshold in 64-bit fixnum
      -0x4000_0000_0000_0000 => -62,
      -0x4000_0000_0000_0001 => -1,
      # Bignum
      -0x3333_3333_3333_3333_3333_3334 => -48
    }

    expecteds.each_pair do |num, cnt|
      it "returns #{cnt} when #{num} is given" do
        expect(BitCounter.count(num)).to eq cnt
      end
    end
  end
end
