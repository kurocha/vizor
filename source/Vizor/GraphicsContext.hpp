//
//  GraphicsContext.hpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 17/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "DeviceContext.hpp"

namespace Vizor
{
	class GraphicsContext : public DeviceContext
	{
	public:
		GraphicsContext(const DeviceContext & device_context, vk::Queue graphics_queue) : DeviceContext(device_context), _graphics_queue(graphics_queue) {}
		virtual ~GraphicsContext() {}
		
		vk::Queue graphics_queue() {return _graphics_queue;}
		
	protected:
		vk::Queue _graphics_queue = nullptr;
	};
}
