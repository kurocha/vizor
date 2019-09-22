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
	
	// A context is an interface which contains references to various related objects. It should not directly create or manage the life-cycles of objects, but rather is created from a concrete implementation which does that, e.g. `GraphicsDevice::context()` creates a `GraphicsContext`.
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
}
