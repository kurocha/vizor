//
//  XCBWindow.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 4/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "XCBWindow.hpp"

#if defined(VK_USE_PLATFORM_XCB_KHR)

namespace Vizor
{
	namespace Platform
	{
		XCBWindow::XCBWindow()
		{
			// Open a connection to the X server:
			_connection = xcb_connect(nullptr, nullptr);
			
			// Get the first screen:
			auto setup = xcb_get_setup(_connection);
			auto iterator = xcb_setup_roots_iterator(setup);
			
			_screen = iterator.data;
			
			setup_window(false);
		}
		
		XCBWindow::~XCBWindow()
		{
			if (_handle) {
				xcb_destroy_window(_connection, _handle);
			}
			
			if (_connection) {
				xcb_disconnect(_connection);
			}
		}
		
		vk::UniqueSurfaceKHR XCBWindow::create_surface(const vk::Instance & instance) const
		{
			vk::XcbSurfaceCreateInfoKHR surfaceCreateInfo;
			
			surfaceCreateInfo.connection = _connection;
			surfaceCreateInfo.window = _handle;
			
			return instance.createXcbSurfaceKHRUnique(surfaceCreateInfo, nullptr);
		}
		
		void XCBWindow::set_title(const std::string title)
		{
			xcb_change_property(
				_connection,
				XCB_PROP_MODE_REPLACE,
				_handle,
				XCB_ATOM_WM_NAME,
				XCB_ATOM_STRING,
				8,
				title.size(),
				title.data()
			);
		}
		
		// void XCBWindow::process_events(const EventHandler & handler)
		// {
		// 	xcb_generic_event_t * event;
    // 
		// 	while ((event = xcb_poll_for_event(_connection)))
		// 	{
		// 		handler(event);
		// 		free(event);
		// 	}
		// }
		
		static inline xcb_intern_atom_reply_t* intern_atom_helper(xcb_connection_t * connection, bool only_if_exists, const char *str)
		{
			xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, only_if_exists, strlen(str), str);
			return xcb_intern_atom_reply(connection, cookie, NULL);
		}

		void XCBWindow::setup_window(bool fullscreen)
		{
			assert(_screen);
			
			std::uint32_t value_mask, value_list[32];
			
			_handle = xcb_generate_id(_connection);
			
			value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
			value_list[0] = _screen->black_pixel;
			value_list[1] =
				XCB_EVENT_MASK_KEY_RELEASE |
				XCB_EVENT_MASK_KEY_PRESS |
				XCB_EVENT_MASK_EXPOSURE |
				XCB_EVENT_MASK_STRUCTURE_NOTIFY |
				XCB_EVENT_MASK_POINTER_MOTION |
				XCB_EVENT_MASK_BUTTON_PRESS |
				XCB_EVENT_MASK_BUTTON_RELEASE;
			
			if (fullscreen) {
				_width = _screen->width_in_pixels;
				_height = _screen->height_in_pixels;
			}
			
			xcb_create_window(_connection,
				XCB_COPY_FROM_PARENT,
				_handle, _screen->root,
				0, 0, _width, _height, 0,
				XCB_WINDOW_CLASS_INPUT_OUTPUT,
				_screen->root_visual,
				value_mask, value_list);
				
			/* Magic code that will send notification when window is destroyed */
			xcb_intern_atom_reply_t* reply = intern_atom_helper(_connection, true, "WM_PROTOCOLS");
			_delete_window_reply = intern_atom_helper(_connection, false, "WM_DELETE_WINDOW");
			
			xcb_change_property(_connection,
				XCB_PROP_MODE_REPLACE,
				_handle, reply->atom, 4, 32, 1,
				&_delete_window_reply->atom);
			
			free(reply);
			
			if (fullscreen) {
				xcb_intern_atom_reply_t *atom_wm_state = intern_atom_helper(_connection, false, "_NET_WM_STATE");
				xcb_intern_atom_reply_t *atom_wm_fullscreen = intern_atom_helper(_connection, false, "_NET_WM_STATE_FULLSCREEN");
				
				xcb_change_property(_connection,
						XCB_PROP_MODE_REPLACE,
						_handle, atom_wm_state->atom,
						XCB_ATOM_ATOM, 32, 1,
						&atom_wm_fullscreen->atom);
				
				free(atom_wm_fullscreen);
				free(atom_wm_state);
			}
		}

		// void VulkanExampleBase::handleEvent(const xcb_generic_event_t *event)
		// {
		// 	switch (event->response_type & 0x7f)
		// 	{
		// 	case XCB_CLIENT_MESSAGE:
		// 		if ((*(xcb_client_message_event_t*)event).data.data32[0] ==
		// 			(*atom_wm_delete_window).atom) {
		// 			quit = true;
		// 		}
		// 		break;
		// 	case XCB_MOTION_NOTIFY:
		// 	{
		// 		xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
		// 		handleMouseMove((int32_t)motion->event_x, (int32_t)motion->event_y);
		// 		break;
		// 	}
		// 	break;
		// 	case XCB_BUTTON_PRESS:
		// 	{
		// 		xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
		// 		if (press->detail == XCB_BUTTON_INDEX_1)
		// 			mouseButtons.left = true;
		// 		if (press->detail == XCB_BUTTON_INDEX_2)
		// 			mouseButtons.middle = true;
		// 		if (press->detail == XCB_BUTTON_INDEX_3)
		// 			mouseButtons.right = true;
		// 	}
		// 	break;
		// 	case XCB_BUTTON_RELEASE:
		// 	{
		// 		xcb_button_press_event_t *press = (xcb_button_press_event_t *)event;
		// 		if (press->detail == XCB_BUTTON_INDEX_1)
		// 			mouseButtons.left = false;
		// 		if (press->detail == XCB_BUTTON_INDEX_2)
		// 			mouseButtons.middle = false;
		// 		if (press->detail == XCB_BUTTON_INDEX_3)
		// 			mouseButtons.right = false;
		// 	}
		// 	break;
		// 	case XCB_KEY_PRESS:
		// 	{
		// 		const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
		// 		switch (keyEvent->detail)
		// 		{
		// 			case KEY_W:
		// 				camera.keys.up = true;
		// 				break;
		// 			case KEY_S:
		// 				camera.keys.down = true;
		// 				break;
		// 			case KEY_A:
		// 				camera.keys.left = true;
		// 				break;
		// 			case KEY_D:
		// 				camera.keys.right = true;
		// 				break;
		// 			case KEY_P:
		// 				paused = !paused;
		// 				break;
		// 			case KEY_F1:
		// 				if (settings.overlay) {
		// 					settings.overlay = !settings.overlay;
		// 				}
		// 				break;
		// 		}
		// 	}
		// 	break;	
		// 	case XCB_KEY_RELEASE:
		// 	{
		// 		const xcb_key_release_event_t *keyEvent = (const xcb_key_release_event_t *)event;
		// 		switch (keyEvent->detail)
		// 		{
		// 			case KEY_W:
		// 				camera.keys.up = false;
		// 				break;
		// 			case KEY_S:
		// 				camera.keys.down = false;
		// 				break;
		// 			case KEY_A:
		// 				camera.keys.left = false;
		// 				break;
		// 			case KEY_D:
		// 				camera.keys.right = false;
		// 				break;			
		// 			case KEY_ESCAPE:
		// 				quit = true;
		// 				break;
		// 		}
		// 		keyPressed(keyEvent->detail);
		// 	}
		// 	break;
		// 	case XCB_DESTROY_NOTIFY:
		// 		quit = true;
		// 		break;
		// 	case XCB_CONFIGURE_NOTIFY:
		// 	{
		// 		const xcb_configure_notify_event_t *cfgEvent = (const xcb_configure_notify_event_t *)event;
		// 		if ((prepared) && ((cfgEvent->width != width) || (cfgEvent->height != height)))
		// 		{
		// 				destWidth = cfgEvent->width;
		// 				destHeight = cfgEvent->height;
		// 				if ((destWidth > 0) && (destHeight > 0))
		// 				{
		// 					windowResize();
		// 				}
		// 		}
		// 	}
		// 	break;
		// 	default:
		// 		break;
		// 	}
		// }
	}
}

#endif
