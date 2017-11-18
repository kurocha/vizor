//
//  Context.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "DebugReport.hpp"

namespace Vizor
{
	class Context
	{
	public:
		Context(bool enable_validations = false);
		virtual ~Context();
		
		vk::Optional<const vk::AllocationCallbacks> allocator() {
			return _allocator;
		}
		
		vk::UniqueDeviceMemory allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		vk::UniqueDeviceMemory allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		vk::UniqueDeviceMemory allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags = vk::MemoryPropertyFlags());
		
	protected:
		vk::ApplicationInfo _application_info;
		
		int find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags);
		
		void create_instance(bool enableValidations, std::vector<const char *> extensions = {});
		void setup_physical_device();
		void setup_graphics_device(std::vector<const char *> extensions = {});
		
		vk::Optional<const vk::AllocationCallbacks> _allocator = nullptr;
		
		// Layers which are part of the current instance/device:
		std::vector<const char*> _layers;

		vk::UniqueInstance _instance;
		DebugReport _debug_report;

		vk::UniqueDevice _device;

		vk::PhysicalDevice _physical_device;
		vk::PhysicalDeviceMemoryProperties _memory_properties;
		
		std::uint32_t _graphics_queue = 0;
	};
}
