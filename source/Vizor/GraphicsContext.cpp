//
//  GraphicsContext.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "GraphicsContext.hpp"

namespace Vizor
{
	std::uint32_t GraphicsContext::find_graphics_queue_family_index(const vk::PhysicalDevice & physical_device)
	{
		auto queue_families = physical_device.getQueueFamilyProperties();
		
		auto iterator = std::find_if(queue_families.begin(), queue_families.end(),
			[](const vk::QueueFamilyProperties & properties) {
				return (properties.queueFlags & vk::QueueFlagBits::eGraphics);
			}
		);
		
		if (iterator != queue_families.end()) {
			return static_cast<std::uint32_t>(iterator - queue_families.begin());
		} else {
			throw std::runtime_error("Could not find graphics queue")
		}
	}
	
	GraphicsContext::GraphicsContext(const vk::Instance & instance, const vk::PhysicalDevice & physical_device, bool validations, vk::Optional<const vk::AllocationCallbacks> allocation_callbacks) : Context(instance, physical_device, nullptr, allocation_callbacks)
	{
		_graphics_queue_family_index = find_graphics_queue_family();
		
		float queue_priority = 0.0f;
		
		auto queue_info = vk::DeviceQueueCreateInfo()
			.setQueueCount(1)
			.setQueueFamilyIndex(_graphics_queue_family_index)
			.setPQueuePriorities(&queue_priority);
		
		auto device_info = vk::DeviceCreateInfo()
			.setEnabledLayerCount(_layers.size())
			.setPpEnabledLayerNames(_layers.data())
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data())
			.setPQueueCreateInfos(&queue_info)
			.setQueueCreateInfoCount(1);
		
		// Enable all features by default:
		auto features = _physical_device.getFeatures();
		device_info.setPEnabledFeatures(&features);
		
		// Looks like this is taking about 30ms:
		return _physical_device.createDeviceUnique(device_create_info, allocation_callbacks);
	}
	
	GraphicsContext::~GraphicsContext()
	{
	}
}
