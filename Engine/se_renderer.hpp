#pragma once

#include "se_device.hpp"
#include "se_swap_chain.hpp"
#include "se_window.hpp"
#include "se_offscreen_renderer.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace se
{
  class SERenderer
  {
  public:
    SERenderer(SEWindow &window, SEDevice &device);
    ~SERenderer();

    SERenderer(const SERenderer &) = delete;
    SERenderer &operator=(const SERenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return seSwapChain->getRenderPass(); }
    float getAspectRatio() const { return seSwapChain->extentAspectRatio(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
      assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
      return commandBuffers[seSwapChain->getCurrentFrame()];
    }

    int getFrameIndex() const
    {
      assert(isFrameStarted && "Cannot get frame index when frame not in progress");
      return seSwapChain->getCurrentFrame();
    }

   
    SEOffscreenRenderer* getOffscreenRenderer() { return offscreenRenderer.get(); }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    VkCommandBuffer beginOffscreenFrame();
    void endOffscreenFrame();

    void beginOffscreenRenderPass(VkCommandBuffer commandBuffer);
    void endOffscreenRenderPass(VkCommandBuffer commandBuffer);

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    SEWindow &seWindow;
    SEDevice &seDevice;
    std::unique_ptr<SESwapChain> seSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    std::unique_ptr<SEOffscreenRenderer> offscreenRenderer;

    uint32_t currentImageIndex;
    bool isFrameStarted{ false };
    bool isOffscreenFrameStarted{ false };
  };
}