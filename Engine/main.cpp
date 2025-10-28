#include <iostream>
#include <GLFW/glfw3.h>

// MID GREEN DESATURATED (0.4, 0.5, 0.2, 1)

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

		// messy oldfashiond bad way of rendering
		glBegin(GL_TRIANGLES);

		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(-0.5f, -0.5f);

		glEnd();

		//swap front and back buffers
		glfwSwapBuffers(window);

		// poll for and process events CANT CLOSE THE WINDOW WITHOUT THIS
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}