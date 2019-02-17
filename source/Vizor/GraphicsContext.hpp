//
//  GraphicsContext.hpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 17/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"

namespace Vizor
{
	class GraphicsContext : public Context
	{
	public:
		using Context::Context;
		
		virtual ~GraphicsContext();
		
		vk::PhysicalDevice physical_device();
		vk::Device device();
		
	protected:
		virtual std::uint32_t find_graphics_queue_family_index(const vk::PhysicalDevice & physical_device) const;
		virtual void prepare(Layers & layers, Extensions & extensions) const;
		virtual void prepare(vk::DeviceCreateInfo & device_create_info) const;
		
	private:
		vk::PhysicalDevice _physical_device;
		vk::UniqueDevice _device;
		
		std::uint32_t _graphics_queue_family_index;
		
		virtual vk::PhysicalDevice setup_physical_device();
		virtual vk::UniqueDevice setup_device();
	};
}
