#include <iostream>
#include <glfw3.h>

int main()
{
	GLFWwindow* window;
	/* Init the GLFW library*/

	if (!glfwInit())
	{
		return -1;
	}

	// create a windowed mode window and its openGL context

	window = glfwCreateWindow(640, 480, "BsEngine V1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the windows's current context current
	glfwMakeContextCurrent(window);

	// loop until the user closes window
	while (!glfwWindowShouldClose(window))
	{
		//render
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.4, 0.5, 0.2, 1);

		//swap front and back buffers
		glfwSwapBuffers(window);

		// poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}