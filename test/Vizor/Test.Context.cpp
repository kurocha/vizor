//
//  Test.Context.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Vizor/Context.hpp>

namespace Vizor
{
	UnitTest::Suite ContextTestSuite {
		"Vizor::Context",
		
		{"can create context and allocate memory",
			[](UnitTest::Examiner & examiner) {
				Context context;
				
				const std::size_t base_size = 1024*1024;
				std::size_t i = 0;
				
				try {
					while (i < 15) {
						auto device_memory = context.allocate({static_cast<vk::DeviceSize>(base_size << i), 8, 0xFFFF});
						
						i += 1;
					}
				} catch (vk::OutOfDeviceMemoryError error) {
					// We failed to allocate memory, we are done.
				}
				
				examiner.expect(i) >= 8;
			}
		},
	};
}
