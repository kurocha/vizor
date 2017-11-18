# Vizor

Vizor is a high level support library for Vulkan. It doesn't hide the implementation details, but provides higher level building blocks, while still encouraging you to use low-level interfaces where it makes sense to do so.

[![Build Status](https://travis-ci.org/kurocha/vizor.svg?branch=master)](https://travis-ci.org/kurocha/vizor)

## Setup

The build tool [teapot] needs to be installed (which requires [Ruby]):

	$ gem install teapot

[teapot]: https://teapot.nz/
[Ruby]: https://www.ruby-lang.org/en/downloads/

### Dependencies

Fetch all the necessary project dependencies:

	$ cd vizor
	$ teapot fetch

### Tests

Run the tests to confirm basic functionality:

	$ cd vizor
	$ teapot Test/Vizor

## Usage

You can run the tool by executing the following:

	$ teapot Run/Vizor

### Application Design

`Vizor::Context` provides high level access to the renderer, including `vk::Device` and `vk::PhysicalDevice`.

	Vizor::Context context;
	
	// Output to window:
	Vizor::Window window(context);
	
	// Output to framebuffer:
	Vizor::Framebuffer framebuffer(context);
	
	Vizor::MemoryManager memory_manager(context);
	memory_manager->allocate(memory_type, 1024*1024*24);
	
	Vizor::Texture texture(context);
	texture->load(pixel_buffer);
	
	Vizor::Pipeline pipeline;
	
## Contributing

1. Fork it.
2. Create your feature branch (`git checkout -b my-new-feature`).
3. Commit your changes (`git commit -am 'Add some feature'`).
4. Push to the branch (`git push origin my-new-feature`).
5. Create new Pull Request.

## License

Released under the MIT license.

Copyright, 2017, by Samuel Williams. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
