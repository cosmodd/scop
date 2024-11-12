#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <map>

#include "engine/Shader.hpp"
#include "engine/OrbitCamera.hpp"
#include "engine/Texture.hpp"
#include "engine/Mesh.hpp"
#include "maths/Mat4.hpp"
#include "maths/Vec3.hpp"
#include "maths/Utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void printInformations(void)
{
	std::cout << "Versions:" << std::endl;
	std::cout << "├╴ OpenGL " << glGetString(GL_VERSION) << std::endl;
	std::cout << "└╴ GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "GPU:" << std::endl;
	std::cout << "├╴ Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "└╴ Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

void error(const std::string &message)
{
	std::cerr << "\e[101;1m ERR \e[0m " << message << std::endl;
	exit(EXIT_FAILURE);
}

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

void resize(GLFWwindow *window, int *width, int *height)
{
	glfwGetFramebufferSize(window, width, height);
	glViewport(0, 0, *width, *height);
}

std::map<int, bool> pressedKeys;

bool rotateObject = true;
bool showNormals = false;
OrbitCamera camera = OrbitCamera(Vec3(0.0f), 15.0f);

bool isKeyPressed(GLFWwindow *window, int key)
{
	if (glfwGetKey(window, key) == GLFW_PRESS && !pressedKeys[key])
	{
		pressedKeys[key] = true;
		return true;
	}
	else if (glfwGetKey(window, key) == GLFW_RELEASE)
		pressedKeys[key] = false;

	return false;
}

void handleKeyboardInput(GLFWwindow *window, float deltaTime)
{

	if (isKeyPressed(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (isKeyPressed(window, GLFW_KEY_R))
		rotateObject = !rotateObject;

	if (isKeyPressed(window, GLFW_KEY_N))
		showNormals = !showNormals;

	camera.processKeyboardInput(window, deltaTime);
}

// Only move camera when mouse is pressed
// Calculate the offset from start position (when pressed) to current position
// When mouse is released, reset
void handleMouseInput(GLFWwindow *window, double xpos, double ypos)
{
	static bool moving = false;
	static float startX = 0.0f;
	static float startY = 0.0f;
	static float startPitch = 0.0f;
	static float startYaw = 0.0f;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !moving)
	{
		moving = true;
		startX = xpos;
		startY = ypos;
		startPitch = camera.pitch;
		startYaw = camera.yaw;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && moving)
		moving = false;

	if (!moving)
		return;

	const float sensitivity = 0.5f;
	float yaw = startYaw + (xpos - startX) * sensitivity;
	float pitch = startPitch + (ypos - startY) * sensitivity;

	camera.yaw = yaw;
	camera.pitch = maths::clamp(pitch, -89.0f, 89.0f);
	camera.updateCamera();
}

Mesh mesh;
Texture texture;

void handleFileDrop(GLFWwindow *window, int count, const char **paths) {
	(void) window;
	bool alreadyLoadedObject = false;
	bool alreadyLoadedTexture = false;

	for (int i = 0; i < count; i++) {
		std::string path = paths[i];
		std::string file = path.substr(path.find_last_of('/') + 1);
		std::string extension = file.substr(file.find_first_of('.') + 1);

		if (extension == "obj") {
			std::cout << "Loading mesh: " << path << std::endl;
			mesh = loadMesh(path);
			alreadyLoadedObject = true;
			break;
		} else if (extension == "png" || extension == "jpg" || extension == "jpeg") {
			std::cout << "Loading texture: " << path << std::endl;
			texture = Texture(path);
			alreadyLoadedTexture = true;
			if (showNormals)
				showNormals = false;
			break;
		}
	}
}

int main(int ac, char **av)
{
	Vec3 lightPos(10.0f, 5.0f, 10.0f);

	if (ac < 2)
	{
		std::cerr << "Usage: " << av[0] << " <objectPath> [texturePath]" << std::endl;
		return EXIT_FAILURE;
	}

	const std::string objectPath = av[1];
	const std::string texturePath = (ac == 3) ? av[2] : "./assets/textures/wood.png";

	if (!glfwInit())
	{
		error("Failed to initialize GLFW");
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
		error("Failed to create GLFW window");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		error("Failed to initialize GLAD");
		return EXIT_FAILURE;
	}

	mesh = loadMesh(objectPath);
	texture = Texture(texturePath);
	Shader shader("./src/shaders/default.vs", "./src/shaders/default.fs");

	if (ac < 3)
		showNormals = true;

	handleWindowTitle(window);
	printInformations();

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(window, handleMouseInput);
	glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
		(void)window;
		(void)xoffset;
		camera.processMouseScroll(yoffset);
	});
	glfwSetDropCallback(window, handleFileDrop);

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
		model *= Mat4::scale(Vec3(10.0f / mesh.size));
		if (rotateObject)
			model *= Mat4::rotation(0.2f * currentTime, Vec3(0.0f, 1.0f, 0.0f));
		model *= Mat4::translation(mesh.center * -1.0f);

		shader.setFloat("time", currentTime);
		shader.setMat4("projection", projection.transpose());
		shader.setMat4("view", camera.getViewMatrix().transpose());
		shader.setMat4("model", model.transpose());
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.position);
		shader.setBool("showNormal", showNormals);

		texture.bind();
		mesh.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return EXIT_SUCCESS;
}