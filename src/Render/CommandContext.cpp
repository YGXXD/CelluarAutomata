#include "CommandContext.h"
#include "RenderContext.h"
#include "RenderUtil.h"

CommandContext::CommandContext()
{
	CreateCommand();
}

CommandContext::~CommandContext()
{
	DestroyAllCommand();
}

VkResult CommandContext::CreateCommand()
{
	vkGetDeviceQueue(RenderContext::Get().LogicDevice, RenderContext::Get().QueueFamilyIndexContent.Graphics, 0, &GraphicsQueue);
	vkGetDeviceQueue(RenderContext::Get().LogicDevice, RenderContext::Get().QueueFamilyIndexContent.Compute, 0, &ComputeQueue);
	vkGetDeviceQueue(RenderContext::Get().LogicDevice, RenderContext::Get().QueueFamilyIndexContent.Transfer, 0, &TransferQueue);

	VkResult VR = CreateCommandPool();
	if(VR != VK_SUCCESS)
		return VR;

	VR = CreateCommandBuffer();
	if(VR != VK_SUCCESS)
		return VR;

	return VK_SUCCESS;
}

VkResult CommandContext::CreateCommandPool()
{
	VkCustomQueueFamilyIndexContent IndexContent = RenderContext::Get().QueueFamilyIndexContent;
	CommandPoolMap[IndexContent.Graphics] = nullptr;
	VkCommandPoolCreateInfo GraphicsCPCreateInfo = { };
	GraphicsCPCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	GraphicsCPCreateInfo.queueFamilyIndex = IndexContent.Graphics;
	GraphicsCPCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkResult VR = vkCreateCommandPool(RenderContext::Get().LogicDevice, &GraphicsCPCreateInfo, nullptr, &CommandPoolMap[IndexContent.Graphics]);	
	if(VR != VK_SUCCESS)
		return VR;

	if(!CommandPoolMap.count(IndexContent.Transfer))
	{
		CommandPoolMap[IndexContent.Transfer] = nullptr;
		VkCommandPoolCreateInfo TransferCPCreateInfo = { };
		TransferCPCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		TransferCPCreateInfo.queueFamilyIndex = IndexContent.Graphics;
		TransferCPCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	
		VkResult VR = vkCreateCommandPool(RenderContext::Get().LogicDevice, &GraphicsCPCreateInfo, nullptr, &CommandPoolMap[IndexContent.Transfer]);	
		if(VR != VK_SUCCESS)
			return VR;
	}

	if(!CommandPoolMap.count(IndexContent.Transfer))
	{
		CommandPoolMap[IndexContent.Transfer] = nullptr;
		VkCommandPoolCreateInfo GraphicsCPCreateInfo = { };
		GraphicsCPCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		GraphicsCPCreateInfo.queueFamilyIndex = IndexContent.Transfer;
		GraphicsCPCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkResult VR = vkCreateCommandPool(RenderContext::Get().LogicDevice, &GraphicsCPCreateInfo, nullptr, &CommandPoolMap[IndexContent.Transfer]);	
		if(VR != VK_SUCCESS)
			return VR;
	}
	return VK_SUCCESS;
}

VkResult CommandContext::CreateCommandBuffer()
{
	VkCustomQueueFamilyIndexContent IndexContent = RenderContext::Get().QueueFamilyIndexContent;
	VkCommandBufferAllocateInfo CommandBufferInfo = { };
	CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	CommandBufferInfo.commandPool = CommandPoolMap[IndexContent.Graphics];
	CommandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	CommandBufferInfo.commandBufferCount = 1;

	VkResult VR = vkAllocateCommandBuffers(RenderContext::Get().LogicDevice, &CommandBufferInfo, &GraphicsCommandBuffer);
	if(VR != VK_SUCCESS)
		return VR;
	ResetGraphicsCommand();

	CommandBufferInfo.commandPool = CommandPoolMap[IndexContent.Transfer];
	VR = vkAllocateCommandBuffers(RenderContext::Get().LogicDevice, &CommandBufferInfo, &TransferCommandBuffer);
	if(VR != VK_SUCCESS)
		return VR;
	ResetTransferCommand();

	CommandBufferInfo.commandPool = CommandPoolMap[IndexContent.Compute];
	VR = vkAllocateCommandBuffers(RenderContext::Get().LogicDevice, &CommandBufferInfo, &ComputeCommandBuffer);
	if(VR != VK_SUCCESS)
		return VR;

	return VK_SUCCESS;
}

void CommandContext::DestroyAllCommand()
{
	VkCustomQueueFamilyIndexContent IndexContent = RenderContext::Get().QueueFamilyIndexContent;
	vkFreeCommandBuffers(RenderContext::Get().LogicDevice, CommandPoolMap[IndexContent.Graphics], 1, &GraphicsCommandBuffer);
	vkFreeCommandBuffers(RenderContext::Get().LogicDevice, CommandPoolMap[IndexContent.Transfer], 1, &TransferCommandBuffer);
	vkFreeCommandBuffers(RenderContext::Get().LogicDevice, CommandPoolMap[IndexContent.Compute], 1, &ComputeCommandBuffer);

	for(auto& CommandPool : CommandPoolMap)
		vkDestroyCommandPool(RenderContext::Get().LogicDevice, CommandPool.second, nullptr);
}	

void CommandContext::GraphicsQueuePresent(VkSwapchainKHR* pSwapChain, uint32_t* pImageIndex)
{
	VkPresentInfoKHR PresentInfo = { };
	PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	PresentInfo.swapchainCount = 1;
	PresentInfo.pSwapchains = pSwapChain;
	PresentInfo.pImageIndices = pImageIndex;

	vkQueuePresentKHR(GraphicsQueue, &PresentInfo);
}

void CommandContext::ResetGraphicsCommand()
{
	vkResetCommandBuffer(GraphicsCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

void CommandContext::GraphicsCmdBegin()
{
	VkCommandBufferBeginInfo BeginInfo = { };
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(GraphicsCommandBuffer, &BeginInfo);
}

void CommandContext::SubmitGraphicsCommand(VkFence Fence)
{
	VkSubmitInfo SubmitInfo = { };
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &GraphicsCommandBuffer;
	vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, Fence);
}

void CommandContext::GraphicsCmdEnd()
{
	vkEndCommandBuffer(GraphicsCommandBuffer);
}

void CommandContext::GraphicsCmdBeginRender(int ImageIndex, VkRenderPass RenderPass, VkFramebuffer FrameBuffer, VkRect2D* RenderArea)
{
	VkClearValue ClearValue[2];
	ClearValue[0].color = {0.1f, 0.1f, 0.1f, 1.f};

	ClearValue[1].depthStencil.depth = 1.f;
	ClearValue[1].depthStencil.stencil = 0;

	VkRenderPassBeginInfo RenderPassBeginInfo = { };
	RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	RenderPassBeginInfo.renderPass = RenderPass;
	RenderPassBeginInfo.framebuffer = FrameBuffer;
	RenderPassBeginInfo.renderArea = *RenderArea; 
	RenderPassBeginInfo.clearValueCount = 2;
	RenderPassBeginInfo.pClearValues = ClearValue;

	vkCmdBeginRenderPass(GraphicsCommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandContext::GraphicsCmdBindPipeline(VkPipeline GraphicsPipeline)
{
	vkCmdBindPipeline(GraphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);
}

void CommandContext::GraphicsCmdEndRender()
{
	vkCmdEndRenderPass(GraphicsCommandBuffer);
}

void CommandContext::GraphicsCmdPushConstants(VkPipelineLayout PipelineLayout, void* Data)
{
	VkPushConstantRange ConstantRange = PushConstant::ConstansBinding();
	vkCmdPushConstants(GraphicsCommandBuffer, PipelineLayout, ConstantRange.stageFlags, ConstantRange.offset, ConstantRange.size, Data);
}

void CommandContext::GraphicsCmdBindViewport(VkViewport* pViewport)
{
	vkCmdSetViewport(GraphicsCommandBuffer, 0, 1, pViewport);
}

void CommandContext::GraphicsCmdBindScissor(VkRect2D* pScissor)
{
	vkCmdSetScissor(GraphicsCommandBuffer, 0, 1, pScissor);
}

void CommandContext::GraphicsCmdBindVertices(VkBuffer VertexBuffer)
{
	VkDeviceSize Offset = 0;
	vkCmdBindVertexBuffers(GraphicsCommandBuffer, 0, 1, &VertexBuffer, &Offset);
}

void CommandContext::GraphicsCmdBindIndices(VkBuffer IndexBuffer)
{
	vkCmdBindIndexBuffer(GraphicsCommandBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void CommandContext::GraphicsCmdDrawVertices(uint32_t VertexCount, uint32_t InstanceCount)
{
	vkCmdDraw(GraphicsCommandBuffer, VertexCount, InstanceCount, 0, 0);
}

void CommandContext::GraphicsCmdDrawIndices(uint32_t IndexCount, uint32_t InstanceCount, uint32_t VertexOffset)
{
	vkCmdDrawIndexed(GraphicsCommandBuffer, IndexCount, InstanceCount, 0, VertexOffset, 0);
}

void CommandContext::GraphicsCmdBindDiscriptor(VkPipelineLayout PipelineLayout, VkDescriptorSet DescriptorSet)
{
	vkCmdBindDescriptorSets(GraphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
}

void CommandContext::ResetTransferCommand()
{
	vkResetCommandBuffer(TransferCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

void CommandContext::SubmitTransferCommand(VkFence Fence)
{
	VkSubmitInfo SubmitInfo = { };
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &TransferCommandBuffer;
	vkQueueSubmit(TransferQueue, 1, &SubmitInfo, Fence);
}

void CommandContext::TransferCmdBegin()
{
	VkCommandBufferBeginInfo BeginInfo = { };
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(TransferCommandBuffer, &BeginInfo);
}

void CommandContext::TransferCmdEnd()
{
	vkEndCommandBuffer(TransferCommandBuffer);
}

void CommandContext::TransferCmdCopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize BufferSize)
{
	VkBufferCopy BufferCopyInfo = { };
	BufferCopyInfo.srcOffset = 0;
	BufferCopyInfo.dstOffset = 0;
	BufferCopyInfo.size = BufferSize;

	vkCmdCopyBuffer(TransferCommandBuffer, SrcBuffer, DstBuffer, 1, &BufferCopyInfo);
}

void CommandContext::TransferCmdTransitionImageLayout(VkImage Image, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout)
{
	VkImageMemoryBarrier Barrier = { };
	Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	Barrier.oldLayout = OldLayout;
	Barrier.newLayout = NewLayout;
	Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	Barrier.image = Image;
	Barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	Barrier.subresourceRange.baseMipLevel = 0;
	Barrier.subresourceRange.levelCount = 1;
	Barrier.subresourceRange.baseArrayLayer = 0;
	Barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags SourceStage = { };
	VkPipelineStageFlags DestinationStage = { };

	if(OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
	{
		Barrier.srcAccessMask = 0;
		Barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		SourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		DestinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if(OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
	{
		Barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		Barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		SourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		DestinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else 
	{
		throw "Transition Image Layout Failed";
	}

	vkCmdPipelineBarrier(
		TransferCommandBuffer,
		SourceStage, DestinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &Barrier);
}
