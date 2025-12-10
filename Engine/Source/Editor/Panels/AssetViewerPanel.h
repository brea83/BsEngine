#pragma once
#include "ImGuiPanel.h"

namespace Pixie
{
	class Texture;
	class Shader;
	class TextResource;
}
class AssetViewerPanel// : public ImGuiPanel
{
public:
	AssetViewerPanel() { };
	~AssetViewerPanel() { }
	// Inherited via ImGuiPanel
	static void Draw();

	//static std::string SelectedAsset;

private:
	static void DrawTextureEditor(const std::string& assetKey, std::shared_ptr<Pixie::Texture> texture);
	static void DrawShaderEditor(const std::string& assetKey, std::shared_ptr <Pixie::Shader> shader);
	static void DrawTextInspector(const std::string& assetKey, std::shared_ptr <Pixie::TextResource> resource);
};

