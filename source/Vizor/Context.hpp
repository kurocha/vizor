//
//  Context.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace Vizor
{
	class Context
	{
	public:
		Context(const vk::Instance & instance, const vk::PhysicalDevice & physical_device, const vk::Device & device, vk::Optional<const vk::AllocationCallbacks> allocation_callbacks = nullptr) : _instance(instance), _physical_device(physical_device), _device(device), _allocation_callbacks(allocation_callbacks) {}
		
		virtual ~Context();
		
		vk::Instance instance() const noexcept {
			return _instance;
		}
		
		vk::PhysicalDevice physical_device() const noexcept {
			return _physical_device;
		}
		
		vk::Device device() const noexcept {
			return _device;
		}
		
		vk::Optional<const vk::AllocationCallbacks> allocation_callbacks() const noexcept {
			return _allocation_callbacks;
		}
		
	protected:
		vk::Instance _instance;
		vk::PhysicalDevice _physical_device;
		vk::Device _device;
		
		vk::Optional<const vk::AllocationCallbacks> _allocation_callbacks;
	};
}
