
#include <Vizor/Context.hpp>
#include <Vizor/Swapchain.hpp>
#include <Vizor/Platform/Window.hpp>

#include <Time/Interval.hpp>

int main(int argc, char ** argv)
{
	Vizor::Platform::Window window;
	
	window.set_title("Hello World");
	
	Vizor::Context::Prototype context_prototype;
	context_prototype.enable_validations = true;
	context_prototype.enable_swapchain = true;
	
	Vizor::Context context(context_prototype);
	
	Vizor::Swapchain::Prototype swapchain_prototype(context.instance(), context.physical_device(), context.device());
	Vizor::Swapchain swapchain(swapchain_prototype, window);
	
	window.show_window();
	
	Time::Interval(5).sleep();
	
	return 0;
}