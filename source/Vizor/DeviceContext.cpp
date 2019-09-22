//
//  DeviceContext.cpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "DeviceContext.hpp"

#include <Logger/Console.hpp>
#include <Units/Bytes.hpp>

namespace Vizor
{
	using namespace Logger;
	
	int DeviceContext::find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags) const
	{
		auto memory_properties = _physical_device.getMemoryProperties();
		
		auto count = memory_properties.memoryTypeCount;
		auto types = memory_properties.memoryTypes;

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
	
	vk::UniqueDeviceMemory DeviceContext::allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags) const
	{
		auto memory_allocate_info = vk::MemoryAllocateInfo()
			.setAllocationSize(requirements.size)
			.setMemoryTypeIndex(find_memory_type(requirements, flags));
			
		Console::debug("Allocating", Units::Bytes{memory_allocate_info.allocationSize}, "from memory type", memory_allocate_info.memoryTypeIndex, "memory type bits", requirements.memoryTypeBits);
		
		auto memory = _device.allocateMemoryUnique(memory_allocate_info, _allocation_callbacks);
		
		Console::debug("...done.");
		
		return memory;
	}
	
	vk::UniqueDeviceMemory DeviceContext::allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags) const
	{
		auto memory = allocate(_device.getImageMemoryRequirements(image), flags);
		
		_device.bindImageMemory(image, *memory, 0);
		
		return memory;
	}
	
	DeviceImage DeviceContext::make_image(const vk::ImageCreateInfo & info, const vk::MemoryPropertyFlags & flags) const
	{
		auto image = _device.createImageUnique(info, _allocation_callbacks);
		
		auto memory = allocate(*image, flags);
		
		return DeviceImage(info, image, memory);
	}
	
	vk::UniqueDeviceMemory DeviceContext::allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags) const
	{
		auto memory = allocate(_device.getBufferMemoryRequirements(buffer), flags);
		
		_device.bindBufferMemory(buffer, *memory, 0);
		
		return memory;
	}
	
	DeviceBuffer DeviceContext::make_buffer(const vk::BufferCreateInfo & info, const vk::MemoryPropertyFlags & flags) const
	{
		auto buffer = _device.createBufferUnique(info, _allocation_callbacks);
		
		auto memory = allocate(*buffer, flags);
		
		return DeviceBuffer(info, buffer, memory);
	}
	
	DeviceBuffer DeviceContext::make_host_buffer_for_image(const DeviceImage & device_image) const
	{
		auto requirements = _device.getImageMemoryRequirements(*device_image.image);
		
		auto info = vk::BufferCreateInfo()
			.setSize(requirements.size)
			.setUsage(vk::BufferUsageFlagBits::eTransferDst);
		
		return make_buffer(info, vk::MemoryPropertyFlagBits::eHostVisible);
	}
	
	DeviceBuffer DeviceContext::make_host_buffer_with_data(const Byte * data, size_t size, vk::BufferUsageFlags usage) const
	{
		auto info = vk::BufferCreateInfo()
			.setSize(size)
			.setUsage(usage);
		
		DeviceBuffer buffer = make_buffer(info, vk::MemoryPropertyFlagBits::eHostVisible);
		
		if (data != nullptr && size > 0)
			write_memory(*buffer.memory, data, size);
		
		return buffer;
	}
	
	void DeviceContext::write_memory(const vk::DeviceMemory & device_memory, const Byte * data, size_t size) const
	{
		Byte *mapped = static_cast<Byte*>(_device.mapMemory(device_memory, 0, size, vk::MemoryMapFlags()));
		
		std::copy(data, data + size, mapped);
		
		_device.unmapMemory(device_memory);
	}
	
	inline vk::ImageSubresourceRange subresource_range(vk::ImageAspectFlags aspects = vk::ImageAspectFlagBits::eColor)
	{
		return vk::ImageSubresourceRange()
			.setAspectMask(aspects)
			.setBaseMipLevel(0).setLevelCount(1)
			.setBaseArrayLayer(0).setLayerCount(1);
	}
	
	DeviceImageView DeviceContext::make_image_view(DeviceImage && image, vk::ImageSubresourceRange range) const
	{
		typedef vk::ComponentSwizzle S;
		
		auto info = vk::ImageViewCreateInfo()
			.setViewType(vk::ImageViewType::e2D)
			.setFormat(image.info.format)
			.setImage(*image.image)
			.setComponents({S::eR, S::eG, S::eB, S::eA})
			.setSubresourceRange(range);
		
		auto view = _device.createImageViewUnique(info, _allocation_callbacks);
		
		return DeviceImageView(image, info, view);
	}
	
	DeviceImageView DeviceContext::make_image_view(DeviceImage && image, vk::ImageAspectFlags aspects) const
	{
		return make_image_view(std::move(image), subresource_range(aspects).setLevelCount(image.info.mipLevels));
	}
	
	vk::UniqueDescriptorSetLayout DeviceContext::create_descriptor_layout(const std::vector<vk::DescriptorSetLayoutBinding> & bindings) const
	{
		auto info = vk::DescriptorSetLayoutCreateInfo()
			.setBindingCount(bindings.size())
			.setPBindings(bindings.data());
		
		return _device.createDescriptorSetLayoutUnique(info, _allocation_callbacks);
	}
	
	vk::UniqueDescriptorPool DeviceContext::create_descriptor_pool(const std::vector<vk::DescriptorPoolSize> & pool_sizes, std::size_t max_sets) const
	{
		auto info = vk::DescriptorPoolCreateInfo()
			.setPoolSizeCount(pool_sizes.size())
			.setPPoolSizes(pool_sizes.data())
			.setMaxSets(max_sets);
		
		return _device.createDescriptorPoolUnique(info, _allocation_callbacks);
	}
	
	std::vector<vk::DescriptorSet> DeviceContext::allocate_descriptor_sets(const vk::DescriptorPool & pool, const std::vector<vk::DescriptorSetLayout> & layouts) const {
		auto info = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool(pool)
			.setDescriptorSetCount(layouts.size())
			.setPSetLayouts(layouts.data());
		
		return _device.allocateDescriptorSets(info);
	}
}
