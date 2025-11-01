#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "Shaders/Shader.h"
#include "EngineContext.h"
#include "Scene.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Transform.h"


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main()
{
#pragma region WindowInitialization
	GLFWwindow* window;
	/* Init the GLFW library*/

	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << std::endl;
		return -1;
	}
	// window settings set up as done in Learn OpenGl's tutorial
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a windowed mode window and its openGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "BsEngine V1", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}
	//make the windows's current context current
	glfwMakeContextCurrent(window);
	// setting up window resize callback
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
#pragma endregion WindowInitialization

	// initialize GLAD to load openGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile shader program day 2 method, not using full engine architecture yet
	//Shader* triangleShader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
	//create triangle object
	//Triangle* triangle = new Triangle();

	//new engine architecture method
	EngineContext* engine = new EngineContext();
	Scene* startingScene = engine->GetScene();
	//Triangle* triangle1 = new Triangle();

	std::vector<Renderable::Vertex> newVerticies = {
		 Renderable::Vertex{{-0.5f,  0.0f, 0.0,},   /* top */   {1.0, 0.0, 0.0}},
		Renderable::Vertex{{-1.0f, -1.0f, 0.0f, },/* left */  {0.0, 1.0, 0.0}},
		Renderable::Vertex{{ 0.0f, -1.0f, 0.0f, },/* right*/  {0.0, 0.0, 1.0}},
	};
	Triangle* triangle2 = new Triangle( newVerticies);

	//std::vector< Renderable::Vertex> newVerticies2 = {
	//	  Renderable::Vertex{{0.5f,  0.0f, 0.0},   /* top */   {1.0, 0.0, 0.0}},
	//	  Renderable::Vertex{{0.0f,  1.0f, 0.0f}, /* left */  {0.0, 1.0, 0.0}},
	//	  Renderable::Vertex{{1.0f,  1.0f, 0.0f}, /* right*/  {0.0, 0.0, 1.0}},
	//};
	Rectangle* rectangle = new Rectangle();
	Transform* rectTransform = rectangle->GetTransform();
	//rectTransform->Rotate(-55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	rectTransform->Rotate(45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	//rectTransform->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	//startingScene->AddRenderable(triangle1);
	startingScene->AddRenderable(triangle2);
	startingScene->AddRenderable(rectangle);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// loop until the user closes window
	while (!glfwWindowShouldClose(window))
	{
		// input
		ProcessInput(window);


		//this is where we do the engine context
		engine->Draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// CLEAN UP
	//triangleShader->~Shader();
	//triangle->~Triangle();

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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}