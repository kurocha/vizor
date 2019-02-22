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
	
	vk::Instance Application::instance() {
		if (!_instance) {
			setup_instance();
		}
		
		return _instance.get();
	}
	
	vk::PhysicalDevice Application::physical_device() {
		auto physical_devices = instance().enumeratePhysicalDevices();
		
		for (auto & device : physical_devices) {
			return device;
		}
		
		throw std::runtime_error("Could not find suitable physical device!");
	}
	
	void Application::setup_instance()
	{
		vk::ApplicationInfo application_info;
		
		application_info
			.setPEngineName("Vizor")
			.setApiVersion(VK_MAKE_VERSION(1, 0, 2));
			
		setup_instance(application_info);
	}
	
	void Application::setup_instance(vk::ApplicationInfo & application_info)
	{
		Layers layers;
		Extensions extensions;
		
		if (_enable_validations) {
			layers.push_back("VK_LAYER_LUNARG_standard_validation");
		}
		
		// extensions.push_back("VK_EXT_debug_report");
		
		setup_instance(application_info, layers, extensions);
	}
	
	void Application::setup_instance(vk::ApplicationInfo & application_info, Layers & layers, Extensions & extensions)
	{
		auto instance_create_info = vk::InstanceCreateInfo()
			.setPApplicationInfo(&application_info)
			.setEnabledLayerCount(layers.size())
			.setPpEnabledLayerNames(layers.data())
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data());
		
		for(auto & layer : layers) {
			Console::debug("Layer:", layer);
		}
		
		for (auto & extension : extensions) {
			Console::debug("Extension:", extension);
		}
		
		setup_instance(instance_create_info);
	}
	
	void Application::setup_instance(vk::InstanceCreateInfo & instance_create_info)
	{
		_instance = vk::createInstanceUnique(instance_create_info, allocation_callbacks());
	}
}
