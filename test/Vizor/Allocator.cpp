//
//  Test.Context.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Vizor/Application.hpp>
#include <Vizor/Context.hpp>
#include <Vizor/Allocator.hpp>

namespace Vizor
{
	UnitTest::Suite ContextTestSuite {
		"Vizor::Context",
		
		{"it can allocate device local memory",
			[](UnitTest::Examiner & examiner) {
				Application application;
				Allocator allocator(application.physical_device(), application.device());
				
				const std::size_t base_size = 1024*1024;
				std::size_t i = 0;
				
				try {
					while (i < 15) {
						auto device_memory = allocator.allocate({static_cast<vk::DeviceSize>(base_size << i), 8, 0xFFFF}, vk::MemoryPropertyFlagBits::eDeviceLocal);
						
						i += 1;
					}
				} catch (vk::OutOfDeviceMemoryError error) {
					// We failed to allocate memory, we are done.
				}
				
				examiner.expect(i) >= 8;
			}
		},
		
		{"it can allocate host visible memory",
			[](UnitTest::Examiner & examiner) {
				Application application;
				Allocator allocator(application.physical_device(), application.device());
				
				const std::size_t base_size = 1024*1024;
				std::size_t i = 0;
				
				try {
					while (i < 15) {
						auto device_memory = allocator.allocate({static_cast<vk::DeviceSize>(base_size << i), 8, 0xFFFF}, vk::MemoryPropertyFlagBits::eHostVisible);
						
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
