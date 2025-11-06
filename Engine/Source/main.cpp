#pragma once
#include "BsPrecompileHeader.h"
//#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlfwWrapper.h"
#include "EngineContext.h"
#include "Scene.h"
#include "Graphics/Primitives/Triangle.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Primitives/Transform.h"


//void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

int main()
{


	
	EngineContext* engine = EngineContext::GetEngine();
	if (!engine->Init())
	{
		return -1;
	}
	GLFWwindow* glfw = engine->GetMainWindow()->GetGlfwWindow();

#pragma region AddMeshesToStartScene
	Scene* startingScene = engine->GetScene();
	Triangle* triangle1 = new Triangle();
	Transform* triTransform = triangle1->GetTransform();
	triTransform->SetPosition(glm::vec3 ( - 0.5f, -1.0f, 0.0f));

	Cube* cube = new Cube(false);
	Transform* rectTransform = cube->GetTransform();
	//rectTransform->SetScale( glm::vec3(0.5f, 0.5f, 0.5f));
	rectTransform->SetPosition(glm::vec3(1.0f, 0.5f, 0.0f));
	
	
	startingScene->AddRenderable(triangle1);
	
	startingScene->AddRenderable(cube);
#pragma endregion 1 triangles, 1 cube

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// loop until the user closes window
	while (!glfwWindowShouldClose(glfw))
	{
		// input
		ProcessInput(glfw);


		//this is where we do the engine context
		engine->Draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(glfw);
		glfwPollEvents();
	}

	// clean up GLFW
	glfwTerminate();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}