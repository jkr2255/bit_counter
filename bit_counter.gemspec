# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'bit_counter/version'

Gem::Specification.new do |spec|
  spec.name          = "bit_counter"
  spec.version       = BitCounter::VERSION
  spec.authors       = ["Jkr2255"]
  spec.email         = ["magnesium.oxide.play@gmail.com"]

  spec.summary       = %q{Calculate bit count (popcount, Hamming weight) faster in many environments.}
  spec.homepage      = "https://github.com/jkr2255/bit_counter"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
if RUBY_PLATFORM =~ /java/
  spec.platform      = 'java'
else
  spec.extensions    = ["ext/bit_counter/extconf.rb"]
end

  spec.add_development_dependency "bundler", "~> 1.11"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "rspec", "~> 3.0"
end
