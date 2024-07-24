#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

#include "engine/Shader.hpp"
#include "engine/Camera.hpp"
#include "engine/Texture.hpp"
#include "engine/Mesh.hpp"
#include "maths/Mat4.hpp"
#include "maths/Vec3.hpp"
#include "maths/Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void resize(GLFWwindow *window, int *width, int *height)
{
	glfwGetFramebufferSize(window, width, height);
	glViewport(0, 0, *width, *height);
}

Camera camera = Camera(Vec3(0.0f, 0.0f, 10.0f), -90.0f, 0.0f);

void handleKeyboardInput(GLFWwindow *window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Camera movement with arrow keys
	const float cameraSpeed = 50.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.pitch += cameraSpeed;
		if (camera.pitch > 89.0f)
			camera.pitch = 89.0f;
		camera.updateCameraVectors();
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.pitch -= cameraSpeed;
		if (camera.pitch < -89.0f)
			camera.pitch = -89.0f;
		camera.updateCameraVectors();
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.yaw -= cameraSpeed;
		camera.updateCameraVectors();
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.yaw += cameraSpeed;
		camera.updateCameraVectors();
	}

	camera.processKeyboardInput(window, deltaTime);
}

int main(void)
{
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
	Texture texture("./assets/textures/wood.png");

	glEnable(GL_DEPTH_TEST);

	BoundingBox boundingBox = mesh.getBoundingBox();
	Vec3 bbCenter = (boundingBox.min + boundingBox.max) / 2.0f;
	float modelSize = (boundingBox.max - boundingBox.min).magnitude();

	// for (unsigned int i = 0; i < mesh.vertices.size(); i++)
	// {
	// 	std::cout << "Vertex " << i << ":" << std::endl;
	// 	std::cout << "├╴ Position: " << mesh.vertices[i].position << std::endl;
	// 	std::cout << "├╴ UV: " << mesh.vertices[i].texCoords << std::endl;
	// 	std::cout << "└╴ Normal: " << mesh.vertices[i].normal << std::endl;
	// }

	// for (unsigned int i = 0; i < mesh.indices.size(); i += 3)
	// {
	// 	std::cout << "Face " << i / 3 << ":" << std::endl;
	// 	std::cout << "├╴ Vertex 1: " << mesh.vertices[mesh.indices[i]].position << std::endl;
	// 	std::cout << "├╴ Vertex 2: " << mesh.vertices[mesh.indices[i + 1]].position << std::endl;
	// 	std::cout << "└╴ Vertex 3: " << mesh.vertices[mesh.indices[i + 2]].position << std::endl;
	// }

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate delta time
		static float lastTime = 0.0f;
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Resize
		int width, height;
		resize(window, &width, &height);
		float aspectRatio = (float)width / (float)height;

		handleWindowTitle(window);
		handleKeyboardInput(window, deltaTime);

		// Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		Mat4 projection = Mat4::identity();
		projection *= Mat4::infinitePerspective(maths::radians(45.0f), aspectRatio, 0.1f);

		Mat4 model = Mat4::identity();
		model *= Mat4::scale(Vec3(4.0f / modelSize));
		model *= Mat4::rotation(0.5f * currentTime, Vec3(0.0f, 1.0f, 0.0f));
		model *= Mat4::translation(bbCenter * -1.0f);

		shader.setFloat("time", currentTime);
		shader.setMat4("projection", projection.transpose());
		shader.setMat4("view", camera.getViewMatrix().transpose());
		shader.setMat4("model", model.transpose());
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.position);

		texture.bind();
		mesh.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return EXIT_SUCCESS;
}