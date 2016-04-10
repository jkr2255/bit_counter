require "bundler/gem_tasks"
require "rspec/core/rake_task"

RSpec::Core::RakeTask.new(:spec)

if RUBY_PLATFORM =~ /java/
  task :default => [:spec]
else
  require "rake/extensiontask"

  task :build => :compile

  Rake::ExtensionTask.new("bit_counter") do |ext|
    ext.lib_dir = "lib/bit_counter"
  end

  task :default => [:clobber, :compile, :spec]
end
