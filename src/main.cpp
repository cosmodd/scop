#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "engine/Shader.hpp"
#include "maths/Mat4.hpp"
#include "maths/Vec3.hpp"
#include "maths/Utils.hpp"

// Function to display current FPS and frame time in the window title
void handleWindowTitle(GLFWwindow *window)
{
	static double previousTime = glfwGetTime();
	static int frameCount = 0;
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - previousTime;
	frameCount++;

	if (deltaTime >= 1.0)
	{
		double fps = frameCount / deltaTime;
		double frameTime = 1000.0 / fps;

		std::stringstream ss;

		ss << "scop";
		ss << " - " << std::fixed << std::setprecision(0) << fps << " FPS";
		ss << " - " << std::fixed << std::setprecision(2) << frameTime << " ms";

		glfwSetWindowTitle(window, ss.str().c_str());
		frameCount = 0;
		previousTime = currentTime;
	}
}

void printInformations(void) {
	std::cout << "Versions:" << std::endl;
	std::cout << "├╴ OpenGL " << glGetString(GL_VERSION) << std::endl;
	std::cout << "└╴ GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "GPU:" << std::endl;
	std::cout << "├╴ Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "└╴ Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

void autoResize(GLFWwindow *window)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

Vec3 cameraPos = Vec3(0.0f, 0.0f, -3.0f);

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 0.05f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += Vec3(0.0f, 0.0f, -cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos += Vec3(0.0f, 0.0f, cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += Vec3(-cameraSpeed, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += Vec3(cameraSpeed, 0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos += Vec3(0.0f, cameraSpeed, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos += Vec3(0.0f, -cameraSpeed, 0.0f);
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
	GLFWwindow *window = glfwCreateWindow(800, 800, "scop", NULL, NULL);
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

	handleWindowTitle(window);
	printInformations();

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

	glEnable(GL_DEPTH_TEST);

	Vec3 cubePositions[] = {
		Vec3(0.0f, 0.0f, 0.0f),
		Vec3(1.0f, 0.0f, 0.0f),
		Vec3(-1.0f, 0.0f, 0.0f),
	};

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		handleWindowTitle(window);
		autoResize(window);
		processInput(window);

		// Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		// model transform
		const float time = glfwGetTime();

		Mat4 view = Mat4::identity();
		view *= Mat4::lookAt(cameraPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

		Mat4 projection = Mat4::identity();
		projection *= Mat4::perspective(maths::radians(45.0f), 1.0f, 0.1f, 100.0f);

		shader.setMat4("projection", projection.transpose());
		shader.setMat4("view", view.transpose());
		shader.setFloat("time", time);

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(Vec3); i++)
		{
			Mat4 model = Mat4::identity();
			model *= Mat4::rotation(time, Vec3(0.0f, 1.0f, 0.0f).normalize());
			model *= Mat4::translation(cubePositions[i]);
			shader.setMat4("model", model.transpose());

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return EXIT_SUCCESS;
}