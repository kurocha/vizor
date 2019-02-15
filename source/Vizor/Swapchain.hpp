//
//  Swapchain.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 20/11/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "Platform/Window.hpp"

namespace Vizor
{
	class Context;
	
	// https://vulkan-tutorial.com/Drawing_a_triangle/Presentation
	class Swapchain
	{
	public:
		struct Prototype {
			Prototype(const vk::Instance & instance, const vk::PhysicalDevice & physical_device, const vk::Device & device);
			
			vk::Instance instance;
			vk::PhysicalDevice physical_device;
			vk::Device device;
			
			// PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
			// PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
			// PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
			// PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
      // 
			// PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
			// PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
			// PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
			// PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
			// PFN_vkQueuePresentKHR vkQueuePresentKHR;
		};
		
		Swapchain(const Prototype & prototype, const Platform::Window & window);
		virtual ~Swapchain();
		
		struct Buffer {
			vk::Image image;
			vk::ImageView image_view;
		};
		
		auto color_format() const noexcept { return _color_format; }
		auto color_space() const noexcept { return _color_space; }
		
		void resize(std::uint32_t width, std::uint32_t height);
		
	private:
		const Prototype & _prototype;
		
		void setup_surface();
		vk::UniqueSurfaceKHR _surface;
		vk::Format _color_format;
		vk::ColorSpaceKHR _color_space;
		
		void setup_queues();
		std::uint32_t _graphics_queue_family_index, _present_queue_family_index;
		vk::Queue _graphics_queue, _present_queue;
		
		//void setup_buffers(std::uint32_t & width, std::uint32_t & height, bool sync = true);
		vk::SwapchainKHR _handle;
		
		std::vector<vk::Image> _images;
		std::vector<Buffer> _buffers;
	};
}
