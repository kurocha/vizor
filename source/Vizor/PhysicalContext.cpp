//
//  PhysicalContext.cpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "PhysicalContext.hpp"

namespace Vizor
{
	vk::FormatFeatureFlags select_format_features(vk::ImageTiling tiling, vk::FormatProperties properties)
	{
		switch (tiling) {
			case vk::ImageTiling::eLinear:
				return properties.linearTilingFeatures;
			case vk::ImageTiling::eOptimal:
				return properties.optimalTilingFeatures;
			default:
				throw std::runtime_error("could not determine format features");
		}
	}
	
	vk::Format PhysicalContext::find_supported_format(const std::vector<vk::Format> & formats, vk::FormatFeatureFlags required_features, vk::ImageTiling tiling) const
	{
		for (const auto & format : formats) {
			vk::FormatProperties properties = _physical_device.getFormatProperties(format);
			
			vk::FormatFeatureFlags format_features = select_format_features(tiling, properties);
			
			if ((format_features & required_features) == required_features) {
				return format;
			}
		}
		
		throw std::runtime_error("could not find supported format");
	}
	
	vk::Format PhysicalContext::best_depth_format() const
	{
		// Since all depth formats may be optional, we need to find a suitable depth format to use. Start with the highest precision packed format.
		std::vector<vk::Format> depth_formats = {
			vk::Format::eD32Sfloat,
			vk::Format::eD32SfloatS8Uint,
			vk::Format::eD24UnormS8Uint,
			vk::Format::eD16Unorm,
			vk::Format::eD16UnormS8Uint
		};
		
		return find_supported_format(depth_formats, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
	}
}
