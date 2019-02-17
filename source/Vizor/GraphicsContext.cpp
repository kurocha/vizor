//
//  GraphicsContext.cpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 17/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "GraphicsContext.hpp"

namespace Vizor
{
	GraphicsContext::~GraphicsContext()
	{
	}
	
	vk::PhysicalDevice GraphicsContext::physical_device()
	{
		if (!_physical_device) {
			_physical_device = setup_physical_device();
		}
		
		return _physical_device;
	}
	
	vk::Device GraphicsContext::device()
	{
		if (!_device) {
			_device = setup_device();
		}
		
		return _device.get();
	}
	
	vk::PhysicalDevice GraphicsContext::setup_physical_device()
	{
		// Get Physical Devices
		auto physical_devices = this->instance().enumeratePhysicalDevices();
		
		for (auto & device : physical_devices) {
			return device;
		}
		
		throw std::runtime_error("Couldnot find physical device!");
	}

	std::uint32_t GraphicsContext::find_graphics_queue_family_index(const vk::PhysicalDevice & physical_device) const
	{
		auto queue_family_properties = physical_device.getQueueFamilyProperties();
		
		for (std::size_t index = 0; index < queue_family_properties.size(); index += 1) {
			auto & properties = queue_family_properties[index];
			
			if (properties.queueCount == 0) continue;
			
			if (properties.queueFlags & vk::QueueFlagBits::eGraphics) {
				return index;
			}
		}
		
		throw std::runtime_error("Could not find graphics queue!");
	}
	
	void GraphicsContext::prepare(Layers & layers, Extensions & extensions) const
	{
	}
	
	void GraphicsContext::prepare(vk::DeviceCreateInfo & device_create_info) const
	{
	}
	
	vk::UniqueDevice GraphicsContext::setup_device()
	{
		float queue_priority = 0.0f;
		auto physical_device = this->physical_device();
		
		_graphics_queue_family_index = find_graphics_queue_family_index(physical_device);
		
		auto queue_info = vk::DeviceQueueCreateInfo()
			.setQueueCount(1)
			.setQueueFamilyIndex(_graphics_queue_family_index)
			.setPQueuePriorities(&queue_priority);
		
		Layers layers;
		Extensions extensions;
		prepare(layers, extensions);
		
		auto device_create_info = vk::DeviceCreateInfo()
			.setEnabledLayerCount(layers.size())
			.setPpEnabledLayerNames(layers.data())
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data())
			.setPQueueCreateInfos(&queue_info)
			.setQueueCreateInfoCount(1);
		
		// Enable all features by default:
		auto features = _physical_device.getFeatures();
		device_create_info.setPEnabledFeatures(&features);
		
		prepare(device_create_info);
		
		// Looks like this is taking about 30ms:
		return physical_device.createDeviceUnique(device_create_info, this->allocation_callbacks());
	}
}
