#include "BsPrecompileHeader.h"
#include "Application.h"

namespace Pixie
{
	Application::Application()
	{}

	Application::~Application()
	{}

	void Application::Run()
	{
		int countdown = 10;
		while (countdown >= 0)
		{
			std::cout << countdown-- << std::endl;
		}
	}
}