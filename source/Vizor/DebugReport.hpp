//
//  DebugReport.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Context.hpp"

namespace Vizor
{
	class DebugReport
	{
	public:
		struct Dispatch : public vk::DispatchLoaderBase {
			Dispatch() {}
			Dispatch(vk::Instance instance);
			
			PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
			PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
		};
		
		DebugReport() {};
		DebugReport(vk::Instance instance, vk::Optional<const vk::AllocationCallbacks> allocation_callbacks = nullptr);
		~DebugReport();
		
		DebugReport(const DebugReport &) = delete;
		DebugReport & operator=(const DebugReport &) = delete;
		
		DebugReport(DebugReport && other) : _instance(other._instance), _callback(std::move(other._callback))
		{
		}
		
		DebugReport & operator=(DebugReport && other)
		{
			_instance = other._instance;
			std::swap(_callback, other._callback);
			
			return *this;
		}
		
	protected:
		vk::Instance _instance;
		Dispatch _dispatch;
		
		using UniqueDebugReportCallbackEXT = vk::UniqueHandle<vk::DebugReportCallbackEXT,Dispatch>;
		
		UniqueDebugReportCallbackEXT _callback;
	};
}
