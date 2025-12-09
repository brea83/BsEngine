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

//#pragma region AddMeshesToStartScene
	PopulateStartingScene(engine);
//#pragma endregion 1 triangles, 1 cube

	// loop until the user closes window
	while (engine->IsRunning())
	{
		// events collected durring the prev frame are distributed before the next frame
		engine->DispatchEvents();

		engine->Update();

		//draw previous frame
		engine->Draw();
		
	}

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

	GameObject house = startingScene->CreateEmptyGameObject("Viking House");
	Transform& transform2 = house.GetComponent<Transform>();
	transform2.SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	transform2.SetScale(glm::vec3(20.0f));
	house.AddComponent<MeshComponent, const  std::string&>("Assets/Meshes/Viking_House.obj");
	

	cube1.AddChild(house);
	//Transform transform = registry.get<Transform>(testEntity);
	//MeshComponent mesh = registry.emplace<MeshComponent, PrimitiveMeshType>(testEntity, PrimitiveMeshType::Cube);

}