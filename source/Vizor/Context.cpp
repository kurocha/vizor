//
//  Context.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Context.hpp"

#include <Logger/Console.hpp>
#include <Units/Bytes.hpp>

namespace Vizor
{
	using namespace Logger;
	
	std::size_t find_graphics_queue(const vk::PhysicalDevice & physical_device)
	{
		auto queues = physical_device.getQueueFamilyProperties();
		
		auto queue_iter = std::find_if(queues.begin(), queues.end(),
			[](const vk::QueueFamilyProperties & properties) {
				return (properties.queueFlags & vk::QueueFlagBits::eGraphics);
			}
		);
		
		assert(queue_iter != queues.end());
		
		return queue_iter - queues.begin();
	}
	
	Context::Context(bool enable_validations)
	{
		_application_info
			.setPEngineName("Vizor")
			.setApiVersion(VK_MAKE_VERSION(1, 0, 2));
		
		create_instance(enable_validations, {});
		setup_physical_device();
		setup_graphics_device();
	}
	
	Context::~Context()
	{
		Console::debug("Shutting down hardware context...");
	}
	
	int Context::find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags)
	{
		auto count = _memory_properties.memoryTypeCount;
		auto types = _memory_properties.memoryTypes;

		std::uint32_t bits = requirements.memoryTypeBits;

		for (std::size_t i = 0; i < count; i++) {
			bool matches = (bits & 1) == 1;
			auto common_properties = flags & types[i].propertyFlags;
			
			if (matches && common_properties == flags) {
				return i;
			}
			
			bits >>= 1;
		}
		
		throw std::runtime_error("could not find compatible memory type");
	}

	vk::UniqueDeviceMemory Context::allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags)
	{
		auto info = vk::MemoryAllocateInfo()
			.setAllocationSize(requirements.size)
			.setMemoryTypeIndex(find_memory_type(requirements, flags));

		Console::debug("Allocating", Units::Bytes{info.allocationSize}, "from memory type", info.memoryTypeIndex, "memory type bits", requirements.memoryTypeBits);
		auto memory = _device->allocateMemoryUnique(info, _allocator);
		Console::debug("...done.");

		return memory;
	}

	vk::UniqueDeviceMemory Context::allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags)
	{
		auto memory = allocate(_device->getImageMemoryRequirements(image), flags);
		_device->bindImageMemory(image, *memory, 0);
		return memory;
	}

	vk::UniqueDeviceMemory Context::allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags)
	{
		auto memory = allocate(_device->getBufferMemoryRequirements(buffer), flags);
		_device->bindBufferMemory(buffer, *memory, 0);
		return memory;
	}
	
	void Context::create_instance(bool enable_validations, std::vector<const char *> extensions)
	{
		assert(!_instance);

		Console::debug("Setting up vulkan instance...");

		if (enable_validations) {
			extensions.push_back("VK_EXT_debug_report");
			_layers.push_back("VK_LAYER_LUNARG_standard_validation");
		}

		auto instance_create_info = vk::InstanceCreateInfo()
			.setEnabledLayerCount(_layers.size())
			.setPpEnabledLayerNames(_layers.data())
			.setPApplicationInfo(&_application_info)
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data());

		Console::debug(_layers.size(), "layers...");
		for(auto & layer : _layers) {
			Console::debug("Layer:", layer);
		}

		Console::debug(extensions.size(), "extensions...");
		for (auto & extension : extensions) {
			Console::debug("Extension:", extension);
		}

		_instance = vk::createInstanceUnique(instance_create_info, _allocator);
		Console::debug("Allocated vulkan instance:", *_instance);

		if (enable_validations) {
			_debug_report = DebugReport(*_instance);
		}
	}
	
	void Context::setup_physical_device()
	{
		assert(_instance);
		assert(!_physical_device);

		// Get Physical Devices
		auto devices = _instance->enumeratePhysicalDevices();

		assert(devices.size() > 0);
		_physical_device = devices[0];

		auto properties = _physical_device.getProperties();

		Console::debug("Setup physical device:", _physical_device);

		// Print Device Details
		Console::debug("Physical Device Details:",
			"\n\tPhysicalDevice Name:", properties.deviceName,
			"\n\tAPI Version:", properties.apiVersion,
			"\n\tDriver Version:", properties.driverVersion,
			"\n\tVendorID:", properties.vendorID,
			"\n\tDeviceID:", properties.deviceID
		);

		_memory_properties = _physical_device.getMemoryProperties();

		Console::debug("Physical Device Memory Types:");
		for (std::size_t i = 0; i < _memory_properties.memoryTypeCount; i++) {
			auto type = _memory_properties.memoryTypes[i];
			Console::debug("memoryTypes[", i, "]", "heapIndex:", type.heapIndex, "flags:", to_string(type.propertyFlags));
		}
		
		for (std::size_t i = 0; i < _memory_properties.memoryHeapCount; i++) {
			auto heap = _memory_properties.memoryHeaps[i];
			Console::debug("memoryHeaps[", i, "]", Units::Bytes{heap.size}, to_string(heap.flags));
		}
	}

	void Context::setup_graphics_device(std::vector<const char *> extensions)
	{
		assert(_instance && _physical_device);
		assert(!_device);

		_graphics_queue = find_graphics_queue(_physical_device);
		Console::debug("Found graphics queue", _graphics_queue);

		float queue_priority = 0.0f;

		auto queue_info = vk::DeviceQueueCreateInfo()
			.setQueueCount(1)
			.setQueueFamilyIndex(_graphics_queue)
			.setPQueuePriorities(&queue_priority);

		auto device_info = vk::DeviceCreateInfo()
			.setEnabledLayerCount(_layers.size())
			.setPpEnabledLayerNames(_layers.data())
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data())
			.setPQueueCreateInfos(&queue_info)
			.setQueueCreateInfoCount(1);
		
		auto features = _physical_device.getFeatures();
		Console::debug("Pipeline statistics query?", features.pipelineStatisticsQuery ? "yes" : "no");
		device_info.setPEnabledFeatures(&features);

		// Looks like this is taking about 30ms:
		_device = _physical_device.createDeviceUnique(device_info, _allocator);
		Console::debug("Setup graphics device:", *_device);
	}
}
