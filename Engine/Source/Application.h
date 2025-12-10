#pragma once
#include "Core.h"

namespace Pixie
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in Client (ie Sandbox)
	Application* CreateApplication();
}