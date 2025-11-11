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
	void DrawTextureEditor(Texture* texture);
	void DrawShaderEditor(Shader* shader, std::string& filePath);
	void DrawTextInspector(TextResource* resource);
};

