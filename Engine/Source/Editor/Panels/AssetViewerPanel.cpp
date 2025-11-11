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

	std::unordered_map<std::string, Resource*>& resources = AssetLoader::GetResources();

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
		Resource* resource = resources.at(SelectedAsset);
		ResourceType type = resource->Type;
		
		switch (type)
		{
		case ResourceType::TextFile:
			DrawTextInspector((TextResource*)resource);
			break;
		case ResourceType::Shader:
			DrawShaderEditor((Shader*)resource, SelectedAsset);
			break;
		case ResourceType::Texture:
			DrawTextureEditor((Texture*)resource);
			break;
		case ResourceType::Model:
			break;
		default:
			break;
		}
	}

	ImGui::End();
}

void AssetViewerPanel::DrawTextureEditor(Texture* texture)
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

void AssetViewerPanel::DrawShaderEditor(Shader* shader, std::string& filePath)
{
	if (ImGui::Button("Recompile Shader")) shader->ReCompile(filePath);
}

void AssetViewerPanel::DrawTextInspector(TextResource * resource)
{
	ImGui::TextWrapped(resource->Text.c_str());
}
