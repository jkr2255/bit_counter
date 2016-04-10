require "bit_counter/version"

if RUBY_PLATFORM =~ /java/
  require 'bit_counter/jruby'
else
  require "bit_counter/bit_counter"
end
