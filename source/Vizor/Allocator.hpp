//
//  Allocator.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"

namespace Vizor
{
	class Allocator : public DeviceContext
	{
	public:
		Allocator(const DeviceContext & device_context);
		virtual ~Allocator();
		
		vk::UniqueDeviceMemory allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		vk::UniqueDeviceMemory allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		vk::UniqueDeviceMemory allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		
	protected:
		int find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags);
		
	private:
		vk::PhysicalDeviceMemoryProperties _memory_properties;
	};
}
