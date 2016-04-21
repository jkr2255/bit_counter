# BitCounter

Counting bits (popcount, Hamming weight) in integer, made faster.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'bit_counter'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install bit_counter

## Usage

Module `BitCounter` appears after `require 'bit_counter'`.

`BitCounter.count(num)` returns bit count of `num`. (`num` must be `Integer` (`Fixnum` or `Bignum`); otherwise raises `TypeError`)

`BitCounter.count_fixnum(num)` and `BitCounter.count_bignum(num)` are provided, but directly using these are not recommended.

To use `Integer#bit_count`, `require 'bit_counter/core_ext'`. (This feature is not default behavior)

### for negative numbers
Theoretically, Ruby treats negative integers as if *infinite* 1-bits are leaded (`printf '%x', -1` gives `..f` ),
so counting 1 bits for negative numbers is meaningless.

In this implementation, `bit_count` for negative number counts *0* bits and negates the result. (distinguishing with positive version) Same in Ruby:
```rb
return -bit_count(~num) if num < 0
```

Note that both `bit_count(0)` and `bit_count(-1)` still return 0.

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Implementations

### JRuby
Java offers `Long.bitCount`, so simply calling this for `Fixnum`.

JRuby uses [`java.lang.BigInteger`](https://docs.oracle.com/javase/7/docs/api/java/math/BigInteger.html) for Ruby's `Bignum`, which also has `bitCount()` method.

### CRuby & Rubinius
In C extensions, `Bignum` is converted to array of `long`, and bit is counted using loops.
Bit counting of `long` is done by using `POPCNT` instruction, if available.

### Limitations
This gem is mainly developed on x64 GCC environment, so in other environments it may not work or be slow.

Pull requests for other environments are welcome.

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/jkr2255/bit_counter.


## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).

