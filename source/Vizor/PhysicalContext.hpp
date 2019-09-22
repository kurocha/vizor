//
//  PhysicalContext.hpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"

namespace Vizor
{
	class PhysicalContext : public Context
	{
	public:
		PhysicalContext(const Context & context, vk::PhysicalDevice physical_device) : Context(context), _physical_device(physical_device) {}
		virtual ~PhysicalContext() {}
		
		vk::PhysicalDevice physical_device() const noexcept {return _physical_device;}
		
		vk::Format find_supported_format(const std::vector<vk::Format> & formats, vk::FormatFeatureFlags required_features, vk::ImageTiling tiling = vk::ImageTiling::eOptimal) const;
		vk::Format best_depth_format() const;
		
	protected:
		vk::PhysicalDevice _physical_device = nullptr;
	};
}
