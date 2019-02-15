//
//  Application.hpp
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
	class Application
	{
	public:
		struct Prototype {
			Prototype() {}
			
			bool enable_validations = false;
			bool enable_swapchain = false;
		};
		
		Application(const Prototype & prototype = Prototype());
		virtual ~Application();
		
		vk::Instance instance() const noexcept {
			return _instance.get();
		}
		
		vk::PhysicalDevice physical_device() const noexcept {
			return _physical_device;
		}
		
	protected:
		vk::ApplicationInfo _application_info;
		
		vk::Optional<const vk::AllocationCallbacks> _allocation_callbacks = nullptr;
		
		void create_instance(bool enableValidations, std::vector<const char *> extensions = {});
		void setup_physical_device();
		void setup_graphics_device(std::vector<const char *> extensions = {});
		
		// Layers which are part of the current instance/device:
		std::vector<const char*> _layers;
		
		vk::UniqueInstance _instance;
		DebugReport _debug_report;
		
		vk::PhysicalDevice _physical_device;
		

	};
}
