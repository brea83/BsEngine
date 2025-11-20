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
#include "GameObject.h"

//void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void PopulateStartingScene(EngineContext* engine);

int main()
{

	EngineContext* engine = EngineContext::GetEngine();
	if (!engine->Init())
	{
		return -1;
	}
	//GLFWwindow* glfw = engine->GetGlfwWindow();

//#pragma region AddMeshesToStartScene
	PopulateStartingScene(engine);
//#pragma endregion 1 triangles, 1 cube

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

void PopulateStartingScene(EngineContext* engine)
{
	Scene* startingScene = engine->GetScene();
	//Triangle* triangle1 = new Triangle(/*Scene::NextUID++*/);
	//std::shared_ptr<Transform> triTransform = triangle1->GetTransform();
	//triTransform->SetPosition(glm::vec3(-0.5f, -1.0f, 0.0f));

	//Cube* cube = new Cube(/*Scene::NextUID++,*/ false);
	//std::shared_ptr<Transform> rectTransform = cube->GetTransform();
	////rectTransform->SetScale( glm::vec3(0.5f, 0.5f, 0.5f));
	//rectTransform->SetPosition(glm::vec3(1.0f, 0.5f, 0.0f));


	//startingScene->AddRenderable(triangle1);

	//startingScene->AddRenderable(cube);

	//Model* testModel = new Model(/*Scene::NextUID++, */"Assets/Meshes/GuitarBackpack.fbx");
	//Model* testModel = new Model(/*Scene::NextUID++, */"Assets/Meshes/Viking_House.obj");

	GameObject* testObject = new GameObject();
	std::shared_ptr<Transform> gameObjectTransform = testObject->GetTransform();
	gameObjectTransform->SetScale(glm::vec3(20.0f));
	startingScene->AddGameObject(testObject);
	testObject->AddComponent<Model, const  std::string&>("Assets/Meshes/Viking_House.obj");
}