#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/Shader.hpp"
#include "maths/Mat4.hpp"

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(void)
{
	float vertices[] = {
		// Front
		-0.5f, -0.5f, 0.5f, // Bottom Left
		0.5f, -0.5f, 0.5f,  // Bottom Right
		0.5f, 0.5f, 0.5f,   // Top Right
		-0.5f, 0.5f, 0.5f,  // Top Left

		// Back
		-0.5f, -0.5f, -0.5f, // Bottom Left
		0.5f, -0.5f, -0.5f,  // Bottom Right
		0.5f, 0.5f, -0.5f,   // Top Right
		-0.5f, 0.5f, -0.5f,  // Top Left
	};

	unsigned int indices[] = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Right
		1, 5, 6,
		6, 2, 1,

		// Back
		5, 4, 7,
		7, 6, 5,

		// Left
		4, 0, 3,
		3, 7, 4,

		// Bottom
		4, 5, 1,
		1, 0, 4,

		// Top
		3, 2, 6,
		6, 7, 3,
	};

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
	}

	// GLFW Hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	GLFWwindow *window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	Shader shader("./src/shaders/default.vs", "./src/shaders/default.fs");

	unsigned int VBO, VAO, EBO;

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex Attributes
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		shader.use();

		glBindVertexArray(VAO);
		// Wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return EXIT_SUCCESS;
}