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
	using Layers = std::vector<const char *>;
	using Extensions = std::vector<const char *>;
	using AllocationCallbacks = vk::Optional<const vk::AllocationCallbacks>;
	
	class Context
	{
	public:
		Context(vk::Instance instance, AllocationCallbacks allocation_callbacks = nullptr) : _instance(instance), _allocation_callbacks(allocation_callbacks) {}
		virtual ~Context() {}
		
		vk::Instance instance() const noexcept {return _instance;}
		
		AllocationCallbacks allocation_callbacks() const noexcept {
			return _allocation_callbacks;
		}
		
	protected:
		vk::Instance _instance = nullptr;
		AllocationCallbacks _allocation_callbacks = nullptr;
	};
	
	class PhysicalContext : public Context
	{
	public:
		PhysicalContext(const Context & context, vk::PhysicalDevice physical_device) : Context(context), _physical_device(physical_device) {}
		virtual ~PhysicalContext() {}
		
		vk::PhysicalDevice physical_device() const noexcept {return _physical_device;}
		
	protected:
		vk::PhysicalDevice _physical_device = nullptr;
	};
	
	class DeviceContext : public PhysicalContext
	{
	public:
		DeviceContext(const PhysicalContext & physical_context, vk::Device device) : PhysicalContext(physical_context), _device(device) {}
		virtual ~DeviceContext() {}
		
		vk::Device device() const noexcept {return _device;}
		
	protected:
		vk::Device _device = nullptr;
	};
}
