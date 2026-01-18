#include "RenderInspectorPanel.h"
#include "EngineContext.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/FrameBuffer.h"
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

namespace Pixie
{
    bool RenderInspectorPanel::Draw()
    {
        ImGui::Begin("Render Inspector");

        std::unordered_map<std::string, std::shared_ptr<FrameBuffer>> frameBuffers = EngineContext::GetEngine()->GetRenderer()->GetAllRenderBuffers();

        ImVec2 regionAvailable = ImGui::GetContentRegionAvail();
        float heightPerBuffer = regionAvailable.y / frameBuffers.size();

        

        for (auto pair : frameBuffers)
        {
            DrawFrameBuffer(pair.second, pair.first, heightPerBuffer );
        }

        ImGui::End();
        return true;
    }

    void RenderInspectorPanel::DrawFrameBuffer(std::shared_ptr<FrameBuffer> frameBuffer, const std::string& shaderName, float heightRestriction)
    {
        ImGui::PushID(shaderName.c_str());
        ImGui::SeparatorText(shaderName.c_str());
        

        const FrameBufferSpecification& spec = frameBuffer->GetSpecification();
        glm::vec2 size = glm::vec2(spec.Width, spec.Height);
        std::vector<uint32_t> colorAttachments = frameBuffer->GetColorAttachments();
        
        int numRows = colorAttachments.size() ? 1 : 0;
        if (frameBuffer->GetDepthAttatchmentID() != 0)
            numRows += 1;
        
        std::vector<uint32_t> depthLayerViews = frameBuffer->GetDepthLayerViews();
        int depthCount = depthLayerViews.size();
        if (depthCount == 0 && frameBuffer->GetDepthAttatchmentID() != 0)
        {
            depthCount = 1;
        }

        std::string swapChainText;
        if (spec.SwapChainTarget)
            swapChainText = "Is Swap Chain Target";
        else
            swapChainText = "Is Not Swap Chain Target";
        
        if (ImGui::BeginTable("FrameBuffer Metadata", 4))
        {
            ImGui::TableSetupColumn("One");
            ImGui::TableSetupColumn("Two");
            ImGui::TableSetupColumn("Three");
            ImGui::TableSetupColumn("Four");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(swapChainText.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Samples: {%d}", spec.Samples);

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("DepthLayers: {%d}", (depthLayerViews.size()));

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("Attatchment Size: ");
            ImGui::SameLine();
            ImGui::BeginDisabled();
            ImGui::DragFloat2("##size", glm::value_ptr(size));
            ImGui::EndDisabled();

            ImGui::EndTable();
        }

        ImGui::Separator();
        // display the attatched textures

        float bufferAspectRatio = size.x / size.y;
        
        float heightPerRow = heightRestriction;
            heightPerRow -= ImGui::GetFrameHeight() * numRows;
            heightPerRow /= numRows;

        ImVec2 regionAvailable = ImGui::GetContentRegionAvail();
        regionAvailable.y = regionAvailable.y > heightPerRow ? heightPerRow : regionAvailable.y;
        

        if (colorAttachments.size())
        {
            DebugImageSpecification imageSpec = GetImageSize(regionAvailable, colorAttachments.size(), bufferAspectRatio);
            ImGui::SeparatorText("Color Attachments");
            if (ImGui::BeginTable("ColorBuffers", colorAttachments.size()))
            {
                ImGui::TableNextRow();

                for (int i = 0; i < colorAttachments.size(); i++)
                {
                    ImGui::PushID(i);
                    ImGui::TableSetColumnIndex(i);
                    uint32_t colorID = colorAttachments[i];
                    if (colorID != 0)
                    {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imageSpec.Padding.x);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + imageSpec.Padding.y);
                        ImGui::Image((void*)colorID, imageSpec.ImageSize, { 0, 1 }, { 1, 0 });
                    }
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }

        }

        if (depthCount)
        {
            DebugImageSpecification imageSpec = GetImageSize(regionAvailable, depthCount, bufferAspectRatio);
            ImGui::SeparatorText("Depth Attachments");

            if (frameBuffer->IsDepthTexture3D())
            {
                if (ImGui::BeginTable("DepthBuffer", depthLayerViews.size()))
                {
                    ImGui::TableNextRow();

                    for (int i = 0; i < depthLayerViews.size(); i++)
                    {
                        ImGui::TableSetColumnIndex(i);
                        uint32_t depthID = depthLayerViews[i];
                        if (depthID != 0)
                        {
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imageSpec.Padding.x);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + imageSpec.Padding.y);
                            ImGui::Image((void*)depthID, imageSpec.ImageSize, { 0, 1 }, { 1, 0 });
                        }
                    }

                    ImGui::EndTable();
                }
            }
            else
            {
                uint32_t depthID = frameBuffer->GetDepthAttatchmentID();
                if (depthID != 0)
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + imageSpec.Padding.x);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + imageSpec.Padding.y);
                    ImGui::Image((void*)depthID, imageSpec.ImageSize, { 0, 1 }, { 1, 0 });
                }
            }
            

        }

        ImGui::PopID();
    }

    RenderInspectorPanel::DebugImageSpecification RenderInspectorPanel::GetImageSize(ImVec2 regionAvailable, int numTextures, float bufferAspectRatio)
    {
        DebugImageSpecification spec;
        spec.ImageSize = ImVec2(1.0f, 1.0f);
        spec.Padding.x = 0;
        spec.Padding.y = 0;

        regionAvailable.x = regionAvailable.x / numTextures;
        float regionAspectRatio = regionAvailable.x / regionAvailable.y;

        if (regionAspectRatio > bufferAspectRatio)
        {
            float imageWidth = regionAvailable.y * bufferAspectRatio;
            spec.Padding.x = (regionAvailable.x - imageWidth) / 2;

            spec.ImageSize.x = imageWidth;

            if (bufferAspectRatio == 1)
            {
                spec.ImageSize.y = imageWidth < regionAvailable.y ? imageWidth : regionAvailable.y;
                spec.Padding.y = (regionAvailable.y - spec.ImageSize.y) / 2;
            }
            else
            {
                spec.ImageSize.y = regionAvailable.y;
            }
        }
        else
        {
            float imageHeight = regionAvailable.x / bufferAspectRatio;
            spec.Padding.y = (regionAvailable.y - imageHeight) / 2;

            spec.ImageSize.y = imageHeight;
            if (bufferAspectRatio == 1)
            {
                spec.ImageSize.x = imageHeight < regionAvailable.x ? imageHeight : regionAvailable.x;
                spec.Padding.x = (regionAvailable.x - spec.ImageSize.x) / 2;
            }
            else
            {
                spec.ImageSize.x = regionAvailable.x;
            }
        }

        return spec;
    }

}