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
	
	void Application::prepare(vk::ApplicationInfo & application_info) const
	{
		application_info
			.setPEngineName("Vizor")
			.setApiVersion(VK_MAKE_VERSION(1, 0, 2));
	}
	
	void Application::prepare(vk::InstanceCreateInfo & instance_create_info) const
	{
	}
	
	vk::Instance Application::instance() {
		if (!_instance) {
			_instance = setup_instance();
		}
		
		return _instance.get();
	}
	
	void Application::prepare(Layers & layers, Extensions & extensions) const
	{
	}
	
	vk::UniqueInstance Application::setup_instance()
	{
		Layers layers;
		Extensions extensions;
		
		vk::ApplicationInfo application_info;
		
		prepare(application_info);
		prepare(layers, extensions);
		
		auto instance_create_info = vk::InstanceCreateInfo()
			.setPApplicationInfo(&application_info)
			.setEnabledLayerCount(layers.size())
			.setPpEnabledLayerNames(layers.data())
			.setEnabledExtensionCount(extensions.size())
			.setPpEnabledExtensionNames(extensions.data());
		
		prepare(instance_create_info);
		
		for(auto & layer : layers) {
			Console::debug("Layer:", layer);
		}
		
		for (auto & extension : extensions) {
			Console::debug("Extension:", extension);
		}
		
		return vk::createInstanceUnique(instance_create_info, _allocation_callbacks);
	}
}
