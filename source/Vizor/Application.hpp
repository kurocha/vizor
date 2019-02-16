//
//  Application.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "Context.hpp"

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
		
		using Layers = std::vector<const char *>;
		using Extensions = std::vector<const char *>;
		
		Application(const Prototype & prototype = Prototype(), vk::Optional<const vk::AllocationCallbacks> allocation_callbacks = nullptr) : _prototype(prototype), _allocation_callbacks(allocation_callbacks) {}
		
		virtual ~Application();
		
		void prepare(vk::ApplicationInfo & application_info) const noexcept;
		void prepare_instance(Layers & layers, Extensions & extensions) const noexcept;
		vk::Instance instance();
		
		vk::PhysicalDevice physical_device();
		
		void prepare_device(Layers & layers, Extensions & extensions) const noexcept;
		std::uint32_t find_graphics_queue_family_index(const vk::PhysicalDevice & physical_device) const noexcept;
		vk::Device device();
		
		vk::Optional<const vk::AllocationCallbacks> allocation_callbacks() const noexcept {
			return _allocation_callbacks;
		}
		
		Context context()
		{
			return Context(instance(), physical_device(), device(), _allocation_callbacks);
		}
		
	protected:
		Prototype _prototype;
		vk::Optional<const vk::AllocationCallbacks> _allocation_callbacks = nullptr;
		
		vk::UniqueInstance setup_instance() const;
		vk::PhysicalDevice setup_physical_device(const vk::Instance & instance) const;
		vk::UniqueDevice setup_device(const vk::Instance & instance, const vk::PhysicalDevice & physical_device) const;
		
	private:
		vk::UniqueInstance _instance;
		vk::PhysicalDevice _physical_device;
		vk::UniqueDevice _device;
	};
}
