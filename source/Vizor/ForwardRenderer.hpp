//
//  ForwardRenderer.hpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "GraphicsContext.hpp"

namespace Vizor
{
	class ForwardRenderer : public GraphicsContext
	{
	public:
		ForwardRenderer(const GraphicsContext & graphics_context, vk::Format color_format, vk::Format depth_format) : GraphicsContext(graphics_context), _color_format(color_format), _depth_format(depth_format) {}
		
		ForwardRenderer(const GraphicsContext & graphics_context, vk::Format color_format) : GraphicsContext(graphics_context), _color_format(color_format), _depth_format(graphics_context.best_depth_format()) {}
		
		virtual ~ForwardRenderer();
		
		vk::RenderPass render_pass();
		
		DeviceImageView make_depth_buffer(const vk::Extent3D & extent) const;
		
	protected:
		vk::Format _color_format;
		vk::Format _depth_format;
		
		DeviceImageView _depth_buffer;
		
	private:
		void setup_render_pass();
		
		vk::UniqueRenderPass _render_pass;
	};
}
