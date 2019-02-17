//
//  Application.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"

namespace Vizor
{
	class Application
	{
	public:
		Application(bool validations = false, AllocationCallbacks allocation_callbacks = nullptr) : _validations(validations), _allocation_callbacks(allocation_callbacks) {}
		virtual ~Application();
		
		Application(const Application &) = delete;
		
		vk::Instance instance();
		
		vk::Optional<const vk::AllocationCallbacks> allocation_callbacks() const noexcept {
			return _allocation_callbacks;
		}
		
		// Selects the first available physical device.
		virtual vk::PhysicalDevice physical_device();
		
		PhysicalContext context() {return PhysicalContext(Context(instance(), allocation_callbacks()), physical_device());}
		
	protected:
		virtual void setup_instance();
		virtual void setup_instance(vk::ApplicationInfo & application_info);
		virtual void setup_instance(vk::ApplicationInfo & application_info, Layers & layers, Extensions & extensions);
		virtual void setup_instance(vk::InstanceCreateInfo & instance_create_info);
		
	private:
		bool _validations = false;
		
		AllocationCallbacks _allocation_callbacks;
		vk::UniqueInstance _instance;
	};
}
