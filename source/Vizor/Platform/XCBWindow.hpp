//
//  XCBWindow.hpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 4/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#if defined(VK_USE_PLATFORM_XCB_KHR)

#include <vulkan/vulkan.hpp>
#include <string>

namespace Vizor
{
	namespace Platform
	{
		class XCBWindow
		{
		public:
			XCBWindow();
			virtual ~XCBWindow();
			
			auto connection() const noexcept { return _connection; }
			auto screen() const noexcept { return _screen; }
			auto handle() const noexcept { return _handle; }
			
			vk::UniqueSurfaceKHR create_surface(const vk::Instance & instance) const;
			
			void show_window() {
				xcb_map_window(_connection, _handle);
				xcb_flush(_connection);
			}
			
			void hide_window() {
				xcb_unmap_window(_connection, _handle);
			}
			
			void set_title(const std::string title);
			
			// void process_events();
			
		private:
			std::uint32_t _width = 800, _height = 600;
			
			xcb_connection_t * _connection = nullptr;
			xcb_screen_t * _screen = nullptr;
			xcb_window_t _handle = ~0;
			
			xcb_intern_atom_reply_t * _delete_window_reply;
			
			void setup_window(bool fullscreen);
		};
		
		using Window = XCBWindow;
	}
}

#endif
