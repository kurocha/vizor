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
		DebugReport(vk::Instance instance, const vk::AllocationCallbacks * allocator = nullptr);
		~DebugReport();
		
		DebugReport(const DebugReport &) = delete;
		DebugReport & operator=(const DebugReport &) = delete;
		
		DebugReport(DebugReport && other) : _instance(other._instance), _allocator(other._allocator), _callback(other._callback)
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
		const vk::AllocationCallbacks * _allocator = nullptr;
		
		VkDebugReportCallbackEXT _callback = nullptr;
	};
}
