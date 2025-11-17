#include "BsPrecompileHeader.h"
#include "AssetViewerPanel.h"
#include "AssetLoader.h"
#include "Graphics/Texture.h"
#include "Graphics/Shaders/Shader.h"
#include "TextResource.h"

std::string AssetViewerPanel::SelectedAsset{ "" };

void AssetViewerPanel::OnImGuiRender()
{
	ImGui::Begin("Loaded Assets");

	ImGui::SeparatorText("Clean Up Test");
	if (ImGui::Button("Clean"))
	{
		AssetLoader::CleanUp();
	}

	ImGui::SeparatorText("Loaded Assets");
	std::unordered_map<std::string, std::shared_ptr<Resource>>& resources = AssetLoader::GetResources();

	for (auto pair : resources)
	{
		if (ImGui::Button(pair.first.c_str()))
		{
			SelectedAsset = pair.first;
		}
	}

	if (resources.find(SelectedAsset) != resources.end())
	{
		ImGui::NewLine();
		ImGui::SeparatorText(SelectedAsset.c_str());
		std::shared_ptr<Resource> resource = resources.at(SelectedAsset);
		ResourceType type = resource->Type;
		
		switch (type)
		{
		case ResourceType::TextFile:
			DrawTextInspector(std::dynamic_pointer_cast<TextResource>(resource));
			break;
		case ResourceType::Shader:
			DrawShaderEditor(std::dynamic_pointer_cast<Shader>(resource), SelectedAsset);
			break;
		case ResourceType::Texture:
			DrawTextureEditor(std::dynamic_pointer_cast<Texture>(resource));
			break;
		case ResourceType::Model:
			break;
		default:
			break;
		}
	}

	ImGui::End();
}

void AssetViewerPanel::DrawTextureEditor(std::shared_ptr<Texture> texture)
{
	int current_min = static_cast<int>(texture->GetMinFilterType());
	int current_mag = static_cast<int>(texture->GetMagFilterType());

	if (ImGui::Combo("Min Filter", &current_min, Texture::Min_FilterModeNames, IM_ARRAYSIZE(Texture::Min_FilterModeNames)))
	{
		texture->CreateTexture(SelectedAsset, static_cast<Min_FilterType>(current_min), static_cast<Mag_FilterType>(current_mag));
	}

	if (ImGui::Combo("Mag Filter", &current_mag, Texture::Mag_FilterModeNames, IM_ARRAYSIZE(Texture::Mag_FilterModeNames)))
	{
		texture->CreateTexture(SelectedAsset, static_cast<Min_FilterType>(current_min), static_cast<Mag_FilterType>(current_mag));
	}
}

void AssetViewerPanel::DrawShaderEditor(std::shared_ptr <Shader> shader, std::string& filePath)
{
	if (ImGui::Button("Recompile Shader")) shader->ReCompile(filePath);
}

void AssetViewerPanel::DrawTextInspector(std::shared_ptr <TextResource> resource)
{
	ImGui::TextWrapped(resource->Text.c_str());
}
