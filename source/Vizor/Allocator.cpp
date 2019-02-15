//
//  Allocator.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 7/12/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Allocator.hpp"

#include <Logger/Console.hpp>
#include <Units/Bytes.hpp>
#include <Streams/Safe.hpp>

namespace Vizor
{
	using namespace Logger;
	
	Allocator::Allocator(const vk::PhysicalDevice & physical_device, const vk::Device & device) : _physical_device(physical_device), _device(device)
	{
		_memory_properties = _physical_device.getMemoryProperties();

		Console::debug("Memory Types:");
		for (std::size_t i = 0; i < _memory_properties.memoryTypeCount; i++) {
			auto type = _memory_properties.memoryTypes[i];
			Console::debug("memoryTypes[", i, "]", "heapIndex:", type.heapIndex, "flags:", to_string(type.propertyFlags));
		}
		
		for (std::size_t i = 0; i < _memory_properties.memoryHeapCount; i++) {
			auto heap = _memory_properties.memoryHeaps[i];
			Console::debug("memoryHeaps[", i, "]", Units::Bytes{heap.size}, to_string(heap.flags));
		}
	}
	
	Allocator::~Allocator()
	{
	}
	
	int Allocator::find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags)
	{
		auto count = _memory_properties.memoryTypeCount;
		auto types = _memory_properties.memoryTypes;

		std::uint32_t bits = requirements.memoryTypeBits;

		for (std::size_t i = 0; i < count; i++) {
			bool matches = (bits & 1) == 1;
			auto common_properties = flags & types[i].propertyFlags;
			
			if (matches && common_properties == flags) {
				return i;
			}
			
			bits >>= 1;
		}
		
		throw std::runtime_error("could not find compatible memory type");
	}

	vk::UniqueDeviceMemory Allocator::allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags)
	{
		auto info = vk::MemoryAllocateInfo()
			.setAllocationSize(requirements.size)
			.setMemoryTypeIndex(find_memory_type(requirements, flags));

		Console::debug("Allocating", Units::Bytes{info.allocationSize}, "from memory type", info.memoryTypeIndex);
		auto memory = _device.allocateMemoryUnique(info, _allocation_callbacks);
		Console::debug("...done.");

		return memory;
	}

	vk::UniqueDeviceMemory Allocator::allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags)
	{
		auto memory = allocate(_device.getImageMemoryRequirements(image), flags);
		_device.bindImageMemory(image, *memory, 0);
		return memory;
	}

	vk::UniqueDeviceMemory Allocator::allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags)
	{
		auto memory = allocate(_device.getBufferMemoryRequirements(buffer), flags);
		_device.bindBufferMemory(buffer, *memory, 0);
		return memory;
	}
}
