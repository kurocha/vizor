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
		
		vk::Instance instance();
		
		vk::Optional<const vk::AllocationCallbacks> allocation_callbacks() const noexcept {
			return _allocation_callbacks;
		}
		
	protected:
		virtual void prepare(vk::ApplicationInfo & application_info) const;
		virtual void prepare(Layers & layers, Extensions & extensions) const;
		virtual void prepare(vk::InstanceCreateInfo & instance_create_info) const;
	
	private:
		AllocationCallbacks _allocation_callbacks;
		vk::UniqueInstance _instance;
		
		bool _validations = false;
		
		virtual vk::UniqueInstance setup_instance();
	};
}
