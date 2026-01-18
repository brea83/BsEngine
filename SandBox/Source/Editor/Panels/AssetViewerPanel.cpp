#include "BsPrecompileHeader.h"
#include "AssetViewerPanel.h"
#include "ImGui/ImGuiPanel.h"
#include "Resources/AssetLoader.h"
//#include "Graphics/Texture.h"
//#include "Graphics/Shaders/Shader.h"
#include "Resources/TextResource.h"

namespace Pixie
{

	void AssetViewerPanel::Draw()
	{
		ImGui::Begin("Loaded Assets");

		ImGui::SeparatorText("Clean Up Test");
		if (ImGui::Button("Clean"))
		{
			AssetLoader::CleanUp();
		}

		ImGui::SeparatorText("Loaded Assets");
		std::unordered_map<std::string, std::shared_ptr<Resource>>& resources = AssetLoader::GetResources();

		if (resources.size() <= 0)
		{
			ImGui::End();
			return;
		}

		static std::string SelectedAsset;

		for (auto pair : resources)
		{
			std::string name = pair.first;
			ResourceType type = pair.second->Type;

			if (type == ResourceType::TextFile)
			{
				std::shared_ptr<TextResource> text = std::dynamic_pointer_cast<TextResource>(pair.second);
				name = text->GetNameString();
			}

			if (type == ResourceType::Shader)
			{
				std::shared_ptr<Shader> shader = std::dynamic_pointer_cast<Shader>(pair.second);
				name = shader->GetName();
			}

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
				DrawTextInspector(SelectedAsset, std::dynamic_pointer_cast<TextResource>(resource));
				break;
			case ResourceType::Shader:
				DrawShaderEditor(SelectedAsset, std::dynamic_pointer_cast<Shader>(resource));
				break;
			case ResourceType::Texture:
				DrawTextureEditor(SelectedAsset, std::dynamic_pointer_cast<Texture>(resource));
				break;
			case ResourceType::Mesh:
				break;
			default:
				break;
			}
		}

		ImGui::End();
	}

	void AssetViewerPanel::DrawTextureEditor(const std::string& assetKey, std::shared_ptr<Texture> texture)
	{
		int current_min = static_cast<int>(texture->GetMinFilterType());
		int current_mag = static_cast<int>(texture->GetMagFilterType());

		if (ImGui::Combo("Min Filter", &current_min, Texture::Min_FilterModeNames, IM_ARRAYSIZE(Texture::Min_FilterModeNames)))
		{
			texture->UpdateTextureFilters(assetKey, static_cast<Min_FilterType>(current_min), static_cast<Mag_FilterType>(current_mag));
		}

		if (ImGui::Combo("Mag Filter", &current_mag, Texture::Mag_FilterModeNames, IM_ARRAYSIZE(Texture::Mag_FilterModeNames)))
		{
			texture->UpdateTextureFilters(assetKey, static_cast<Min_FilterType>(current_min), static_cast<Mag_FilterType>(current_mag));
		}
	}

	void AssetViewerPanel::DrawShaderEditor(const std::string& assetKey, std::shared_ptr <Shader> shader)
	{
		if (ImGui::Button("Recompile Shader")) shader->ReCompile(assetKey);
	}

	void AssetViewerPanel::DrawTextInspector(const std::string& assetKey, std::shared_ptr <TextResource> resource)
	{
		ImGui::TextWrapped(resource->Text.c_str());
	}
}