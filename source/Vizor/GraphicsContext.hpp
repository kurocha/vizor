//
//  GraphicsContext.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"
#include "DebugReport.hpp"

namespace Vizor
{
	class GraphicsContext : public Context
	{
	public:
		GraphicsContext(const vk::Instance & instance, const vk::PhysicalDevice & physical_device, vk::Optional<const vk::AllocationCallbacks> allocation_callbacks = nullptr);
		virtual ~GraphicsContext();
		
	private:
		// Find the queue family for rendering graphics:
		std::uint32_t find_graphics_queue_family_index();
		
		std::uint32_t _graphics_queue_family_index;
	};
}
