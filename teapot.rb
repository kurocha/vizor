# Teapot v2.2.0 configuration generated at 2017-10-14 14:27:46 +1300

required_version "3.0"

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
	target.depends 'Language/C++17'
	
	target.depends 'Library/Logger', public: true
	target.depends 'Library/Units', public: true
	target.depends 'Library/Memory'
	target.depends 'Library/Streams'
		
	target.depends 'Platform/Vulkan', public: true
	
	target.provides 'Library/Vizor' do
		source_root = target.package.path + 'source'
		
		library_path = build static_library: 'Vizor', source_files: source_root.glob('Vizor/**/*.cpp')
		
		append linkflags library_path
		append header_search_paths source_root
	end
end

define_target 'vizor-test' do |target|
	target.depends 'Language/C++17'
	
	target.depends 'Library/UnitTest'
	target.depends 'Library/Vizor'
	
	target.provides 'Test/Vizor' do |*arguments|
		test_root = target.package.path + 'test'
		
		run source_files: test_root.glob('Vizor/**/*.cpp'), arguments: arguments
	end
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
end

define_configuration "vizor" do |configuration|
	configuration.public!
	
	configuration.require 'logger'
	configuration.require 'units'
	configuration.require 'memory'
	
	configuration.require 'vulkan'
end
