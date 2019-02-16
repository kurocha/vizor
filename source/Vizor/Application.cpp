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
	
	Application::~Application()
	{
	}
	
	void Application::prepare(vk::ApplicationInfo & application_info) const noexcept
	{
		application_info
			.setPEngineName("Vizor")
			.setApiVersion(VK_MAKE_VERSION(1, 0, 2));
	}
	
	void Application::prepare_instance(Layers & layers, Extensions & extensions) const noexcept
	{
		if (_prototype.enable_validations) {
			extensions.push_back("VK_EXT_debug_report");
			layers.push_back("VK_LAYER_LUNARG_standard_validation");
		}
		
		if (_prototype.enable_swapchain) {
			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			
		#if defined(VK_USE_PLATFORM_WIN32_KHR)
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		#elif defined(VK_USE_PLATFORM_XCB_KHR)
			extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
		#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
		#endif
		}
	}
	
	vk::Instance Application::instance()
	{
		if (!_instance) {
			_instance = setup_instance();
		}
		
		return _instance.get();
	}
	
	vk::PhysicalDevice Application::physical_device()
	{
		if (!_physical_device) {
			_physical_device = setup_physical_device(instance());
		}
		
		return _physical_device;
	}
	
	vk::Device Application::device()
	{
		if (!_device) {
			_device = setup_device(instance(), physical_device());
		}
		
		return _device.get();
	}
	
	vk::UniqueInstance Application::setup_instance() const
	{
		Console::debug("Setting up vulkan instance...");
		
		Layers layers;
		Extensions extensions;
		prepare_instance(layers, extensions);
		
		vk::ApplicationInfo application_info;
		prepare(application_info);
		
		auto instance_create_info = vk::InstanceCreateInfo()
			.setEnabledLayerCount(layers.size())
			.setPpEnabledLayerNames(layers.data())
			.setPApplicationInfo(&application_info)
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data());
		
		for(auto & layer : layers) {
			Console::debug("Layer:", layer);
		}
		
		for (auto & extension : extensions) {
			Console::debug("Extension:", extension);
		}
		
		auto instance = vk::createInstanceUnique(instance_create_info, _allocation_callbacks);
		
		Console::debug("Allocated vulkan instance:", *instance);
		
		return instance;
	}
	
	vk::PhysicalDevice Application::setup_physical_device(const vk::Instance & instance) const
	{
		// Get Physical Devices
		auto devices = instance.enumeratePhysicalDevices();

		assert(devices.size() > 0);
		auto physical_device = devices[0];

		auto properties = physical_device.getProperties();

		Console::debug("Setup physical device:", _physical_device);

		// Print Device Details
		Console::debug("Physical Device:", Streams::safe(properties.deviceName),
			"API Version:", properties.apiVersion,
			"Driver Version:", properties.driverVersion,
			"Vendor ID:", properties.vendorID,
			"Device ID:", properties.deviceID
		);
		
		return physical_device;
	}
	
	std::uint32_t Application::find_graphics_queue_family_index(const vk::PhysicalDevice & physical_device) const
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
			throw std::runtime_error("Could not find graphics queue");
		}
	}
	
	void Application::prepare_device(Layers & layers, Extensions & extensions) const noexcept
	{
		if (_prototype.enable_validations) {
			extensions.push_back("VK_EXT_debug_report");
			layers.push_back("VK_LAYER_LUNARG_standard_validation");
		}
	}
	
	vk::UniqueDevice Application::setup_device(const vk::Instance & instance, const vk::PhysicalDevice & physical_device) const
	{
		auto graphics_queue_family_index = find_graphics_queue_family_index(physical_device);
		
		float queue_priority = 0.0f;
		
		auto queue_info = vk::DeviceQueueCreateInfo()
			.setQueueCount(1)
			.setQueueFamilyIndex(graphics_queue_family_index)
			.setPQueuePriorities(&queue_priority);
		
		Layers layers;
		Extensions extensions;
		prepare_device(layers, extensions);
		
		auto device_create_info = vk::DeviceCreateInfo()
			.setEnabledLayerCount(layers.size())
			.setPpEnabledLayerNames(layers.data())
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data())
			.setPQueueCreateInfos(&queue_info)
			.setQueueCreateInfoCount(1);
		
		// Enable all features by default:
		auto features = physical_device.getFeatures();
		device_create_info.setPEnabledFeatures(&features);
		
		// Looks like this is taking about 30ms:
		return _physical_device.createDeviceUnique(device_create_info, _allocation_callbacks);
	}
}
