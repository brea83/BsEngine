#pragma once
namespace Pixie
{
	class Game;
	class StartUpWindow
	{
	public:
		static bool DrawGameStartupWindow(std::shared_ptr<Game> game);
	private:
		//void DrawGui(std::shared_ptr<Game> game);

		static bool OnNewGameButtonPressed(std::shared_ptr<Game> game);
		static bool OnLoadGameButtonPressed(std::shared_ptr<Game> game);
		static void OnQuitButtonPressed();
	};

}