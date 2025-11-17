#pragma once
#include "ImGuiPanel.h"

class Texture;
class Shader;
class TextResource;

class AssetViewerPanel : public ImGuiPanel
{
public:
	AssetViewerPanel() { };
	~AssetViewerPanel() { }
	// Inherited via ImGuiPanel
	void OnImGuiRender() override;

	static std::string SelectedAsset;

private:
	void DrawTextureEditor(std::shared_ptr<Texture> texture);
	void DrawShaderEditor(std::shared_ptr <Shader> shader, std::string& filePath);
	void DrawTextInspector(std::shared_ptr <TextResource> resource);
};

