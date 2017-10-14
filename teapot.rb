# Teapot v2.2.0 configuration generated at 2017-10-14 14:27:46 +1300

required_version "2.0"

# Project Metadata

define_project "vizor" do |project|
	project.title = "Vizor"
	project.summary = 'A brief one line summary of the project.'
	
	project.description = <<-EOF
		Vizor description.
	EOF
	
	project.license = 'MIT License'
	
	project.add_author 'Samuel Williams', email: 'samuel.williams@oriontransfer.co.nz'
	# project.website = 'http://Vizor.com/'
	
	project.version = '0.1.0'
end

# Build Targets

define_target 'vizor-library' do |target|
	target.build do
		source_root = target.package.path + 'source'
		copy headers: source_root.glob('Vizor/**/*.{h,hpp}')
		build static_library: 'Vizor', source_files: source_root.glob('Vizor/**/*.cpp')
	end
	
	target.depends 'Build/Files'
	target.depends 'Build/Clang'
	
	target.depends :platform
	target.depends 'Language/C++14', private: true
	
	target.provides 'Library/Vizor' do
		append linkflags [
			->{install_prefix + 'lib/libVizor.a'},
		]
	end
end

define_target 'vizor-test' do |target|
	target.build do |*arguments|
		test_root = target.package.path + 'test'
		
		run tests: 'Vizor', source_files: test_root.glob('Vizor/**/*.cpp'), arguments: arguments
	end
	
	target.depends 'Library/UnitTest'
	target.depends 'Library/Vizor'
	
	target.provides 'Test/Vizor'
end

define_target 'vizor-executable' do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		build executable: 'Vizor', source_files: source_root.glob('Vizor.cpp')
	end
	
	target.depends 'Build/Files'
	target.depends 'Build/Clang'
	
	target.depends :platform
	target.depends 'Language/C++14', private: true
	
	target.depends 'Library/Vizor'
	target.provides 'Executable/Vizor'
end

define_target 'vizor-run' do |target|
	target.build do |*arguments|
		run executable: 'Vizor', arguments: arguments
	end
	
	target.depends 'Executable/Vizor'
	target.provides 'Run/Vizor'
end

# Configurations

define_configuration 'development' do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	configuration.import "vizor"
	
	# Provides all the build related infrastructure:
	configuration.require 'platforms'
	
	# Provides unit testing infrastructure and generators:
	configuration.require 'unit-test'
	
	# Provides some useful C++ generators:
	configuration.require 'generate-cpp-class'
	
	configuration.require "generate-project"
	configuration.require "generate-travis"
end

define_configuration "vizor" do |configuration|
	configuration.public!
end
