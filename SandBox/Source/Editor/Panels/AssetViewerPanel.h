#pragma once
#include <Pixie.h>

namespace Pixie
{
	class Texture;
	class Shader;
	class TextResource;

	class AssetViewerPanel
	{
	public:
		AssetViewerPanel() {};
		~AssetViewerPanel() {}

		static void Draw();

	private:
		static void DrawTextureEditor(const std::string& assetKey, std::shared_ptr<Texture> texture);
		static void DrawShaderEditor(const std::string& assetKey, std::shared_ptr <Shader> shader);
		static void DrawTextInspector(const std::string& assetKey, std::shared_ptr <TextResource> resource);
	};

}