//
//  DebugReport.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "DebugReport.hpp"

#include <Logger/Console.hpp>

namespace Vizor
{
	DebugReport::Dispatch::Dispatch(vk::Instance instance)
	{
		this->vkCreateDebugReportCallbackEXT = PFN_vkCreateDebugReportCallbackEXT(instance.getProcAddr("vkCreateDebugReportCallbackEXT"));
		
		this->vkDestroyDebugReportCallbackEXT = PFN_vkDestroyDebugReportCallbackEXT(instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));
	}
	
	static VkBool32 message_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj_type, uint64_t src_object, size_t location, int32_t code, const char* layer_prefix, const char* message, void* user_Data) {
		
		using namespace Logger;
		
		if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
			Console::error(layer_prefix, message);
		} else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
			Console::warn(layer_prefix, message);
		} else if (std::string("MEM") != layer_prefix) {
			Console::debug(layer_prefix, message);
		}
		return false;
	}
	
	DebugReport::DebugReport(vk::Instance instance, vk::Optional<const vk::AllocationCallbacks> allocation_callbacks) : _instance(instance), _dispatch(instance)
	{
		auto info = vk::DebugReportCallbackCreateInfoEXT(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning
			// | vk::DebugReportFlagBitsEXT::eInformation | vk::DebugReportFlagBitsEXT::ePerformanceWarning
			, &message_callback, nullptr);
		
		_callback = instance.createDebugReportCallbackEXTUnique(info, allocation_callbacks, _dispatch);
	}
	
	DebugReport::~DebugReport()
	{
	}
}
