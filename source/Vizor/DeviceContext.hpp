//
//  DeviceContext.hpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include "PhysicalContext.hpp"

namespace Vizor
{
	typedef std::uint8_t Byte;
	
	struct DeviceImage
	{
		DeviceImage() {}
		DeviceImage(vk::ImageCreateInfo _info, vk::UniqueImage & _image, vk::UniqueDeviceMemory & _memory) :
			info(_info), image(std::move(_image)), memory(std::move(_memory)) {}
		
		vk::ImageCreateInfo info;
		vk::UniqueImage image;
		vk::UniqueDeviceMemory memory;
		
		explicit operator bool() const noexcept {return !!image;}
	};
	
	struct DeviceImageView
	{
		DeviceImageView () {}
		
		DeviceImageView(DeviceImage & _image, vk::ImageViewCreateInfo _info, vk::UniqueImageView & _view) : image(std::move(_image)), info(_info), view(std::move(_view)) {}
		
		DeviceImage image;
		
		vk::ImageViewCreateInfo info;
		vk::UniqueImageView view;
		
		explicit operator bool() const noexcept {return !!view;}
	};

	struct DeviceBuffer
	{
		DeviceBuffer() {}
		
		DeviceBuffer(vk::BufferCreateInfo _info, vk::UniqueBuffer & _buffer, vk::UniqueDeviceMemory & _memory) : info(_info), buffer(std::move(_buffer)), memory(std::move(_memory)) {}
		
		vk::BufferCreateInfo info;
		vk::UniqueBuffer buffer;
		vk::UniqueDeviceMemory memory;
		
		vk::DescriptorBufferInfo descriptor;
		
		explicit operator bool() const noexcept {return !!buffer;}
	};
	
	class DeviceContext : public PhysicalContext
	{
	public:
		DeviceContext(const PhysicalContext & physical_context, vk::Device device) : PhysicalContext(physical_context), _device(device) {}
		virtual ~DeviceContext() {}
		
		vk::Device device() const noexcept {return _device;}
		
		int find_memory_type(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags) const;
		vk::UniqueDeviceMemory allocate(const vk::MemoryRequirements & requirements, const vk::MemoryPropertyFlags & flags) const;
		vk::UniqueDeviceMemory allocate(const vk::Image & image, const vk::MemoryPropertyFlags & flags) const;
		
		DeviceImage make_image(const vk::ImageCreateInfo & info, const vk::MemoryPropertyFlags & flags) const;
		
		vk::UniqueDeviceMemory allocate(const vk::Buffer & buffer, const vk::MemoryPropertyFlags & flags) const;
		
		DeviceBuffer make_buffer(const vk::BufferCreateInfo & info, const vk::MemoryPropertyFlags & flags) const;
		DeviceBuffer make_host_buffer_for_image(const DeviceImage & image) const;
		DeviceBuffer make_host_buffer_with_data(const Byte * data, size_t size, vk::BufferUsageFlags usage) const;
		
		template <typename T>
		inline DeviceBuffer make_host_buffer_with(const T & t, vk::BufferUsageFlags usage) {
			return make_host_buffer_with_data((const Byte *)&t, sizeof(T), usage);
		}
		
		void write_memory(const vk::DeviceMemory & device_memory, const Byte * data, size_t size) const;
		
		DeviceImageView make_image_view(DeviceImage && image, vk::ImageSubresourceRange range) const;
		DeviceImageView make_image_view(DeviceImage && image, vk::ImageAspectFlags aspects = vk::ImageAspectFlagBits::eColor) const;
		
		vk::UniqueDescriptorSetLayout create_descriptor_layout(const std::vector<vk::DescriptorSetLayoutBinding> & bindings) const;
		vk::UniqueDescriptorPool create_descriptor_pool(const std::vector<vk::DescriptorPoolSize> & pool_sizes, std::size_t max_sets = 1) const;
		std::vector<vk::DescriptorSet> allocate_descriptor_sets(const vk::DescriptorPool & pool, const std::vector<vk::DescriptorSetLayout> & layouts) const;
		
	protected:
		vk::Device _device = nullptr;
	};
}
