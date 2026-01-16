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
        float heightPerRow = regionAvailable.y / frameBuffers.size();

        heightPerRow -= ImGui::GetFrameHeight() * 2;

        for (auto pair : frameBuffers)
        {
            DrawFrameBuffer(pair.second, pair.first, heightPerRow );
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



        std::string swapChainText;
        if (spec.SwapChainTarget)
            swapChainText = "Is Swap Chain Target";
        else
            swapChainText = "Is Not Swap Chain Target";
        
        if (ImGui::BeginTable("FrameBuffer Metadata", 3))
        {
            ImGui::TableSetupColumn("One");
            ImGui::TableSetupColumn("Two");
            ImGui::TableSetupColumn("Three");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(swapChainText.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Samples: {%d}", spec.Samples);

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("Attatchment Size: ");
            ImGui::SameLine();
            ImGui::BeginDisabled();
            ImGui::DragFloat2("##size", glm::value_ptr(size));
            ImGui::EndDisabled();

            ImGui::EndTable();
        }

        ImGui::Separator();
        // display the attatche images

        float bufferAspectRatio = size.x / size.y;
        ImVec2 imageSize = ImVec2(size.x, size.y);
        float xPadding = 0;
        float yPadding = 0;

        ImVec2 regionAvailable = ImGui::GetContentRegionAvail();
        regionAvailable.x = regionAvailable.x * 0.5f;
        regionAvailable.y = regionAvailable.y > heightRestriction ? heightRestriction : regionAvailable.y;
        float regionAspectRatio = regionAvailable.x / regionAvailable.y;

        if (regionAspectRatio > bufferAspectRatio)
        {
            float imageWidth = regionAvailable.y * bufferAspectRatio;
            xPadding = (regionAvailable.x - imageWidth) / 2;

            imageSize.x = imageWidth;

            if (bufferAspectRatio == 1)
            {
                imageSize.y = imageWidth < regionAvailable.y ? imageWidth : regionAvailable.y;
                yPadding = (regionAvailable.y - imageSize.y) / 2;
            }
            else
            {
                imageSize.y = regionAvailable.y;
            }
        }
        else
        {
            float imageHeight = regionAvailable.x / bufferAspectRatio;
            yPadding = (regionAvailable.y - imageHeight) / 2;

                imageSize.y = imageHeight;
            if (bufferAspectRatio == 1)
            {
                imageSize.x = imageHeight < regionAvailable.x ? imageHeight : regionAvailable.x;
                xPadding = (regionAvailable.x - imageSize.x) / 2;
            }
            else
            {
                imageSize.x = regionAvailable.x;
            }
        }


        
        if(ImGui::BeginTable("Buffers", 2))
        {
            ImGui::TableSetupColumn("Color Attatchment");
            ImGui::TableSetupColumn("Depth Attatchment");

            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            uint32_t colorID = frameBuffer->GetFirstColorAttachmentID();
            if (colorID != 0)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xPadding);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yPadding);
                ImGui::Image((void*)colorID, imageSize, { 0, 1 }, { 1, 0 });
            }

            ImGui::TableSetColumnIndex(1);
            uint32_t depthID = frameBuffer->GetDepthAttatchmentID();
            if (depthID != 0)
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xPadding);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yPadding);
                ImGui::Image((void*)depthID, imageSize, { 0, 1 }, { 1, 0 });
            }

            ImGui::EndTable();
        }


        ImGui::PopID();
    }

}