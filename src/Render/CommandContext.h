#pragma once
#include <unordered_map>
#include <vulkan/vulkan_core.h>

class CommandContext final
{
public:
	CommandContext();
	CommandContext(CommandContext& Copy) = delete;
	CommandContext& operator=(CommandContext& Copy) = delete;
	~CommandContext();

	VkResult CreateCommand();
	void DestroyAllCommand();

	void GraphicsQueuePresent(VkSwapchainKHR* pSwapChain, uint32_t* pImageIndex);

	void ResetGraphicsCommand();
	void SubmitGraphicsCommand(VkFence Fence);

	void GraphicsCmdBegin();
	void GraphicsCmdEnd();
	void GraphicsCmdBeginRender(int ImageIndex, VkRenderPass RenderPass, VkFramebuffer FrameBuffer, VkRect2D* RenderArea);
	void GraphicsCmdEndRender();
	void GraphicsCmdPushConstants(VkPipelineLayout PipelineLayout, void* Data);
	void GraphicsCmdBindPipeline(VkPipeline GraphicsPipeline);
	void GraphicsCmdBindViewport(VkViewport* pViewport);
	void GraphicsCmdBindScissor(VkRect2D* pScissor);
	void GraphicsCmdBindVertices(VkBuffer VertexBuffer);
	void GraphicsCmdBindIndices(VkBuffer IndexBuffer);
	void GraphicsCmdDrawVertices(uint32_t VertexCount, uint32_t InstanceCount);
	void GraphicsCmdDrawIndices(uint32_t IndexCount, uint32_t InstanceCount, uint32_t VertexOffset);
	void GraphicsCmdBindDiscriptor(VkPipelineLayout PipelineLayout, VkDescriptorSet DescriptorSet);

	void ResetTransferCommand();
	void SubmitTransferCommand(VkFence Fence);
	void TransferCmdBegin();
	void TransferCmdEnd();
	void TransferCmdCopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize BufferSize);
	void TransferCmdTransitionImageLayout(VkImage Image, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout);

private:
	VkResult CreateCommandPool();
	VkResult CreateCommandBuffer();

	VkQueue GraphicsQueue = nullptr;
	VkQueue ComputeQueue = nullptr;
	VkQueue TransferQueue = nullptr;

	std::unordered_map<uint8_t, VkCommandPool> CommandPoolMap;

	VkCommandBuffer GraphicsCommandBuffer = nullptr;
	VkCommandBuffer TransferCommandBuffer = nullptr;
	VkCommandBuffer ComputeCommandBuffer = nullptr;

};
