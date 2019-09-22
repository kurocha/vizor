//
//  RenderPassContext.hpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "DeviceContext.hpp"

namespace Vizor
{
	class RenderPassContext : public DeviceContext
	{
	public:
		RenderPassContext(const DeviceContext & device_context, vk::RenderPass render_pass) : DeviceContext(device_context), _render_pass(render_pass) {}
		virtual ~RenderPassContext() {}
		
	protected:
		vk::RenderPass _render_pass;
	};
}
