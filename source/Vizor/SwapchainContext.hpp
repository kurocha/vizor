//
//  SwapchainContext.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"

namespace Vizor
{
	class SwapchainContext
	{
	public:
		SwapchainContext(const vk::Instance & instance, const vk::PhysicalDevice & physical_device);
		virtual ~SwapchainContext();

	private:
		vk::Queue _presentation_queue;
	};
}
