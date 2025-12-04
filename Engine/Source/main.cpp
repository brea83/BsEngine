#pragma once
#include "BsPrecompileHeader.h"
//#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GlfwWrapper.h"
#include "EngineContext.h"
#include "Scene/Scene.h"
#include "Graphics/Primitives/Triangle.h"
#include "Graphics/Primitives/Cube.h"
#include "Graphics/Primitives/Transform.h"
#include "Scene/Components/MeshComponent.h"
#include "Scene/GameObject.h"
#include <EnTT/entt.hpp>

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

	//GameObject* testObject = new GameObject("Viking House");
	//std::shared_ptr<Transform> gameObjectTransform = testObject->GetTransform();
	//gameObjectTransform->SetScale(glm::vec3(20.0f));
	//startingScene->AddGameObject(testObject);
	//testObject->AddComponent<MeshComponent, const  std::string&>("Assets/Meshes/Viking_House.obj");

	//GameObject* testObject2 = new GameObject("Empty");
	//std::shared_ptr<Transform> gameObjectTransform2 = testObject2->GetTransform();
	//gameObjectTransform2->SetScale(glm::vec3(2.0f));
	//gameObjectTransform2->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	//startingScene->AddGameObject(testObject2);
	//
	//testObject->AddChild(testObject2);

	entt::registry& registry = startingScene->GetRegistry();
	GameObject cube1 = startingScene->CreateCube();

	GameObject cube2 = startingScene->CreateCube();
	cube1.AddChild(cube2);
	//Transform transform = registry.get<Transform>(testEntity);
	//MeshComponent mesh = registry.emplace<MeshComponent, PrimitiveMeshType>(testEntity, PrimitiveMeshType::Cube);

}