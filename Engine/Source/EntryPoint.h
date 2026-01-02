#pragma once
#include "Core.h"
//#include "Application.h"
#include "EngineContext.h"
#include "Log.h"
//#include "Scene/Scene.h"
#ifdef  PIXIE_PLATFORM_WINDOWS

extern Pixie::EngineContext* Pixie::CreateApplication();

int main(int argc, char** argv)
{

	auto engine = Pixie::CreateApplication();
	if (!engine->Init())
	{
		return -1;
	}
	
#pragma region TEMPE POPULATE START SCENE
	//engine->GetScene()->PopulateWithTestObjects();
#pragma endregion 1 cube and 1 viking house

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
