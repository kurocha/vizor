//
//  ForwardRenderer.cpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 22/9/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include "ForwardRenderer.hpp"

namespace Vizor
{
	ForwardRenderer::~ForwardRenderer()
	{
	}
	
	vk::RenderPass ForwardRenderer::render_pass()
	{
		if (!_render_pass) {
			setup_render_pass();
		}
		
		return _render_pass.get();
	}
	
	DeviceImageView ForwardRenderer::make_depth_buffer(const vk::Extent3D & extent) const
	{
		auto image_info = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setExtent(extent)
			.setMipLevels(1)
			.setArrayLayers(1)
			.setTiling(vk::ImageTiling::eOptimal)
			.setInitialLayout(vk::ImageLayout::eUndefined);
		
		// This is faster to render (much faster in some cases) but slower to allocate (by quite a bit):
		//vk::MemoryPropertyFlags flags = vk::MemoryPropertyFlagBits::eDeviceLocal;
		vk::MemoryPropertyFlags flags{};
		
		auto info = vk::ImageCreateInfo(image_info)
			.setFormat(_depth_format)
			.setUsage(vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eDepthStencilAttachment);
		
		return make_image_view(make_image(info, flags), vk::ImageAspectFlagBits::eDepth);
	}
	
	void ForwardRenderer::setup_render_pass()
	{
		std::array attachments = {
			vk::AttachmentDescription()
				.setFormat(_color_format)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
			
			vk::AttachmentDescription()
				.setFormat(_depth_format)
				.setSamples(vk::SampleCountFlagBits::e1)
				.setLoadOp(vk::AttachmentLoadOp::eClear)
				.setStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
				.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
				.setInitialLayout(vk::ImageLayout::eUndefined)
				.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal),
		};
		
		auto color_attachment_reference = vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);
		auto depth_attachment_reference = vk::AttachmentReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

		auto subpass = vk::SubpassDescription()
			.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
			.setColorAttachmentCount(1).setPColorAttachments(&color_attachment_reference)
			.setPDepthStencilAttachment(&depth_attachment_reference);
		
		auto subpass_dependency = vk::SubpassDependency()
			.setSrcSubpass(VK_SUBPASS_EXTERNAL)
			.setDstSubpass(0)
			.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setSrcAccessMask(vk::AccessFlags())
			.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
			.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);
		
		auto renderpass_create_info = vk::RenderPassCreateInfo()
			.setAttachmentCount(attachments.size()).setPAttachments(attachments.data())
			.setSubpassCount(1).setPSubpasses(&subpass)
			.setDependencyCount(1).setPDependencies(&subpass_dependency);

		_render_pass = _device.createRenderPassUnique(renderpass_create_info, _allocation_callbacks);
	}
}
