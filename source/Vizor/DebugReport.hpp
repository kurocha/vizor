//
//  DebugReport.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace Vizor
{
	class DebugReport
	{
	public:
		DebugReport() {};
		DebugReport(vk::Instance instance, vk::Optional<const vk::AllocationCallbacks> allocation_callbacks = nullptr);
		~DebugReport();
		
		DebugReport(const DebugReport &) = delete;
		DebugReport & operator=(const DebugReport &) = delete;
		
		DebugReport(DebugReport && other) : _instance(other._instance), _allocation_callbacks(other._allocation_callbacks), _callback(other._callback)
		{
			other._callback = nullptr;
		}
		
		DebugReport & operator=(DebugReport && other)
		{
			_instance = other._instance;
			_allocator = other._allocator;
			std::swap(_callback, other._callback);
			
			return *this;
		}
		
	protected:
		vk::Instance _instance;
		vk::Optional<const vk::AllocationCallbacks> _allocation_callbacks = nullptr;
		
		VkDebugReportCallbackEXT _callback = nullptr;
	};
}
