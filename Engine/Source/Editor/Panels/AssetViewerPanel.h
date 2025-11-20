#pragma once
#include "ImGuiPanel.h"

class Texture;
class Shader;
class TextResource;

class AssetViewerPanel// : public ImGuiPanel
{
public:
	AssetViewerPanel() { };
	~AssetViewerPanel() { }
	// Inherited via ImGuiPanel
	static void Draw();

	//static std::string SelectedAsset;

private:
	static void DrawTextureEditor(const std::string& assetKey, std::shared_ptr<Texture> texture);
	static void DrawShaderEditor(const std::string& assetKey, std::shared_ptr <Shader> shader);
	static void DrawTextInspector(const std::string& assetKey, std::shared_ptr <TextResource> resource);
};

