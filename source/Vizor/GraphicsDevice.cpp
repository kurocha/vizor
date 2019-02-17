//
//  GraphicsDevice.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "GraphicsDevice.hpp"

#include <Logger/Console.hpp>
#include <Units/Bytes.hpp>
#include <Streams/Safe.hpp>

namespace Vizor
{
	using namespace Logger;
	
	GraphicsDevice::~GraphicsDevice()
	{
	}
	
	vk::Device GraphicsDevice::device()
	{
		if (!_device) {
			setup_device();
		}
		
		return _device.get();
	}
	
	void GraphicsDevice::setup_device()
	{
		Layers layers;
		Extensions extensions;
		
		setup_device(layers, extensions);
	}
	
	std::uint32_t GraphicsDevice::find_graphics_queue_family_index() const
	{
		auto queue_family_properties = _physical_device.getQueueFamilyProperties();
		
		for (std::size_t index = 0; index < queue_family_properties.size(); index += 1) {
			auto & properties = queue_family_properties[index];
			
			if (properties.queueCount == 0) continue;
			
			if (properties.queueFlags & vk::QueueFlagBits::eGraphics) {
				return index;
			}
		}
		
		throw std::runtime_error("Could not find graphics queue!");
	}
	
	void GraphicsDevice::setup_device(Layers & layers, Extensions & extensions)
	{
		float queue_priority = 0.0f;
		auto graphics_queue_family_index = find_graphics_queue_family_index();
		
		auto queue_info = vk::DeviceQueueCreateInfo()
			.setQueueCount(1)
			.setQueueFamilyIndex(graphics_queue_family_index)
			.setPQueuePriorities(&queue_priority);
		
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
		
		setup_device(device_create_info);
		
		_graphics_queue = _device->getQueue(graphics_queue_family_index, 0);
	}
	
	void GraphicsDevice::setup_device(vk::DeviceCreateInfo & device_create_info)
	{
		// Looks like this is taking about 30ms:
		_device = _physical_device.createDeviceUnique(device_create_info, allocation_callbacks());
	}
}
