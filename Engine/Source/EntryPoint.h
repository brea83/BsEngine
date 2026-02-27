#pragma once
#include "Core.h"
#include "EngineContext.h"

#ifdef  PIXIE_PLATFORM_WINDOWS

extern Pixie::EngineContext* Pixie::CreateApplication(bool bEditorMode);

int main(int argc, char** argv)
{
	//TODO change default to false to require manual enabling of the editor mode
	bool startInEditMode = true;
	int i = 0;
	while (i < argc)
	{
		if (argv[i] == "Editor")
		{
			startInEditMode = true;
		}

		std::cout << "Argument " << i + 1
			<< ": " << argv[i]
			<< std::endl;
		i++;
	}

	auto engine = Pixie::CreateApplication(startInEditMode);
	if (!engine->Init())
	{
		return -1;
	}

	// loop until the user closes window
	while (engine->IsRunning()/*!glfwWindowShouldClose(glfw)*/)
	{
		// events collected durring the prev frame are distributed before the next frame
		engine->DispatchEvents();

		engine->Update();

		//draw previous frame
		engine->Draw();
	}

	return 0;
}

#endif //  PIXIE_PLATFORM_WINDOWS
