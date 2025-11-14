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
#include "Graphics/Model.h"


//void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

int main()
{


	
	EngineContext* engine = EngineContext::GetEngine();
	if (!engine->Init())
	{
		return -1;
	}
	//GLFWwindow* glfw = engine->GetGlfwWindow();

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

	Model* testModel = new Model("Assets/Meshes/GuitarBackpack.fbx");
	Transform* modelTransform = testModel->GetTransform();
	//modelTransform->SetScale(glm::vec3(0.01f));
	startingScene->AddRenderable(testModel);
#pragma endregion 1 triangles, 1 cube

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// loop until the user closes window
	while (engine->IsRunning()/*!glfwWindowShouldClose(glfw)*/)
	{
		// input moved into engine events
		//ProcessInput(glfw);


		//this is where we do the engine context
		engine->Draw();

		// glfw: swap buffers and poll IO events moved into window class OnUpdate()
		engine->Update();
	}

	// clean up GLFW
	//glfwTerminate();

	return 0;
}

// process all input: moved into engine context
//void ProcessInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}