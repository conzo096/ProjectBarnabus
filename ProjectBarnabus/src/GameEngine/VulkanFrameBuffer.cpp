#include "VulkanFrameBuffer.h"

VulkanFrameBuffer::VulkanFrameBuffer(const std::string & frameBufferName)
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
}

unsigned int VulkanFrameBuffer::GetFrameBuffer()
{
	return 0;
}

ITexture * VulkanFrameBuffer::GetFrameTexture()
{
	return nullptr;
}

ITexture * VulkanFrameBuffer::GetDepthTexture()
{
	return nullptr;
}

void VulkanFrameBuffer::LoadFrameBuffer(int w, int h)
{
}

void VulkanFrameBuffer::BindFrameBuffer()
{
}
