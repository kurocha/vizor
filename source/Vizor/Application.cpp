//
//  Application.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Application.hpp"

#include <Logger/Console.hpp>
#include <Units/Bytes.hpp>
#include <Streams/Safe.hpp>

namespace Vizor
{
	using namespace Logger;
	
	Application::Application(const Prototype & prototype)
	{
		_application_info
			.setPEngineName("Vizor")
			.setApiVersion(VK_MAKE_VERSION(1, 0, 2));
		
		std::vector<const char *> extensions;
		
		if (prototype.enable_swapchain) {
			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			
			extensions.push_back(
#if defined(VK_USE_PLATFORM_WIN32_KHR)
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
				VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
				VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
			);
		}
		
		create_instance(prototype.enable_validations, extensions);
		setup_physical_device();
		setup_graphics_device();
	}
	
	Application::~Application()
	{
		Console::debug("Shutting down hardware context...");
	}
	
	void Application::create_instance(bool enable_validations, std::vector<const char *> extensions)
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

		_instance = vk::createInstanceUnique(instance_create_info, _allocation_callbacks);
		Console::debug("Allocated vulkan instance:", *_instance);

		if (enable_validations) {
			_debug_report = DebugReport(*_instance);
		}
	}
	
	void Application::setup_physical_device()
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
		Console::debug("Physical Device:", Streams::safe(properties.deviceName),
			"API Version:", properties.apiVersion,
			"Driver Version:", properties.driverVersion,
			"Vendor ID:", properties.vendorID,
			"Device ID:", properties.deviceID
		);
	}

	void Application::setup_graphics_device(std::vector<const char *> extensions)
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
  
		// Enable all features by default:
		auto features = _physical_device.getFeatures();
		device_info.setPEnabledFeatures(&features);
  
		// Looks like this is taking about 30ms:
		_device = _physical_device.createDeviceUnique(device_info, _allocation_callbacks);
		Console::debug("Setup graphics device:", *_device);
	}
}
