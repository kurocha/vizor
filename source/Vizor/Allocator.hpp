//
//  Allocator.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace Vizor
{
	class Allocator
	{
	public:
		Allocator(const vk::PhysicalDevice & physical_device, const vk::Device & device);
		virtual ~Allocator();
		
		vk::Optional<const vk::AllocationCallbacks> allocation_callbacks() {
			return _allocation_callbacks;
		}
		
		vk::UniqueDeviceMemory allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		vk::UniqueDeviceMemory allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		vk::UniqueDeviceMemory allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		
	private:
		vk::PhysicalDevice _physical_device;
		vk::PhysicalDeviceMemoryProperties _memory_properties;
		
		vk::Device _device;
		vk::Optional<const vk::AllocationCallbacks> _allocation_callbacks = nullptr;
		
		int find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags);
	};
}
