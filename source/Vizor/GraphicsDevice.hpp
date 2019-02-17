//
//  GraphicsDevice.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "GraphicsContext.hpp"

namespace Vizor
{
	class GraphicsDevice : public PhysicalContext
	{
	public:
		GraphicsDevice(const PhysicalContext & physical_context) : PhysicalContext(physical_context) {}
		virtual ~GraphicsDevice();
		
		GraphicsDevice(const GraphicsDevice &) = delete;
		
		vk::Device device();
		vk::Queue graphics_queue() {return _graphics_queue;}
		
		GraphicsContext context() {return GraphicsContext(DeviceContext(*this, device()), graphics_queue());}
		
	protected:
		virtual std::uint32_t find_graphics_queue_family_index() const;
		
		virtual void setup_device();
		virtual void setup_device(Layers & layers, Extensions & extensions);
		virtual void setup_device(vk::DeviceCreateInfo & device_create_info);
		
	private:
		vk::UniqueDevice _device;
		vk::Queue _graphics_queue = nullptr;
	};
}
