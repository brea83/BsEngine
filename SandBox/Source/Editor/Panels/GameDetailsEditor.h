#pragma once
#include <memory>
namespace Pixie
{
	class Game;

	class GameDetailsEditor
	{
	public:
		static void Draw(std::shared_ptr<Game> game, bool* showCloseButton);
	};
}
