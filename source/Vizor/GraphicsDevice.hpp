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
		
		std::uint32_t graphics_queue_family_index() const noexcept {return _graphics_queue_family_index;}
		vk::Queue graphics_queue() const noexcept {return _graphics_queue;}
		
		GraphicsContext context() {return GraphicsContext(DeviceContext(*this, device()), graphics_queue());}
		
	protected:
		virtual void prepare(Layers & layers, Extensions & extensions) const noexcept;
		
		virtual void setup_queues();
		
		virtual void setup_device();
		virtual void setup_device(Layers & layers, Extensions & extensions);
		virtual void setup_device(vk::DeviceCreateInfo & device_create_info);
		
		vk::UniqueDevice _device;
		
		std::uint32_t _graphics_queue_family_index = -1;
		vk::Queue _graphics_queue = nullptr;
	};
}
