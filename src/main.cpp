#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>

#include "maths/Vec3.hpp"
#include "maths/Mat4.hpp"

// Function to printout opengl errors
GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
			case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cerr << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;

}

GLuint compileShader(GLenum type, const char* source) {
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);
	int result = -1;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length = 1024;
		char message[1024];
		glGetShaderInfoLog(id, 1024, nullptr, message);
		std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cerr << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

GLuint createShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
	GLuint program = glCreateProgram();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);

	int result = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		int length = 1024;
		char message[1024];
		glGetProgramInfoLog(program, 1024, nullptr, message);
		std::cerr << "Failed to link program!" << std::endl;
		std::cerr << message << std::endl;
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

struct Texture {
	GLuint id;
	int width;
	int height;
};

Texture loadTexture(const char* filePath) {
	Texture texture;

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	unsigned char* data = stbi_load(filePath, &texture.width, &texture.height, nullptr, 4);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Failed to load texture: " << filePath << std::endl;
	}

	stbi_image_free(data);
	return texture;
}

void handleFPS(GLFWwindow* window) {
	static double previousSeconds = glfwGetTime();
	static int frameCount = 0;
	double elapsedSeconds = glfwGetTime() - previousSeconds;

	if (elapsedSeconds > 0.25) {
		previousSeconds = glfwGetTime();
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		char title[256];
		sprintf(title, "OpenGL | FPS: %.2f | Frame Time: %.2f ms", fps, msPerFrame);
		glfwSetWindowTitle(window, title);
		frameCount = 0;
	}

	frameCount++;
}

struct Camera {
	maths::Vec3 position;
	maths::Vec3 direction;
	float yaw;
	float pitch;
};

Camera camera = {
	.position = { 0.0f, 0.0f, -3.0f },
	.direction = { 0.0f, 0.0f, 1.0f },
	.yaw = 90.0f / 180.0f * M_PI,
	.pitch = 0.0f
};

void handleInputs(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	maths::Vec3 cameraUp = maths::Vec3(0.0f, 1.0f, 0.0f);
	maths::Vec3 cameraRight = camera.direction.cross(cameraUp).normalize();
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_W:
				camera.position = camera.position + camera.direction;
				break;
			case GLFW_KEY_S:
				camera.position = camera.position - camera.direction;
				break;
			case GLFW_KEY_A:
				camera.position = camera.position - cameraRight;
				break;
			case GLFW_KEY_D:
				camera.position = camera.position + cameraRight;
				break;
			case GLFW_KEY_R:
				camera.position = camera.position + cameraUp;
				break;
			case GLFW_KEY_F:
				camera.position = camera.position - cameraUp;
				break;
			case GLFW_KEY_RIGHT:
				camera.yaw += M_PI / 180.0f;
				break;
			case GLFW_KEY_LEFT:
				camera.yaw -= M_PI / 180.0f;
				break;
			case GLFW_KEY_UP:
				camera.pitch += M_PI / 180.0f;
				break;
			case GLFW_KEY_DOWN:
				camera.pitch -= M_PI / 180.0f;
				break;
		}
	}
}

int main() {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); 
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr);\

	if (!window) {
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize glad" << std::endl;
		return EXIT_FAILURE;
	}

	// Print the OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwSetKeyCallback(window, handleInputs);

	GLuint program = createShader(
		readFile("./assets/shaders/default.vert").c_str(),
		readFile("./assets/shaders/default.frag").c_str()
	);

	Texture blackStoneTexture = loadTexture("./assets/textures/netherrack.png");

	std::vector<float> cubeData {
		// Position				// Texture
		// Back face
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	// Bottom-left
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	// Bottom-right
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,	// Top-right
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	// Top-left
		// Front face
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
		// Left face
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	// Bottom-left
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	// Bottom-right
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f,	// Top-right
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	// Top-left
		// Right face
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	// Bottom-left
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	// Bottom-right
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,	// Top-right
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	// Top-left
		// Bottom face
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	// Bottom-left
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	// Bottom-right
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f,	// Top-right
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	// Top-left
		// Top face
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f,	// Bottom-left
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,	// Bottom-right
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,	// Top-right
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f	// Top-left
	};

	std::vector<unsigned int> cubeIndices {
		// Back face
		0, 1, 2,
		2, 3, 0,
		// Front face
		4, 5, 6,
		6, 7, 4,
		// Left face
		8, 9, 10,
		10, 11, 8,
		// Right face
		12, 13, 14,
		14, 15, 12,
		// Bottom face
		16, 17, 18,
		18, 19, 16,
		// Top face
		20, 21, 22,
		22, 23, 20
	};

	std::vector<float>* data = &cubeData;
	std::vector<unsigned int>* indices = &cubeIndices;

	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), data->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		camera.direction.x = cosf(camera.yaw) * cosf(camera.pitch);
		camera.direction.y = sinf(camera.pitch);
		camera.direction.z = sinf(camera.yaw) * cosf(camera.pitch);
		maths::Mat4 view = maths::Mat4::lookAt(
			camera.position,
			camera.position + camera.direction,
			maths::Vec3(0.0f, 1.0f, 0.0f)
		);
		view = view.transpose();

		maths::Mat4 projection = maths::Mat4::perspective(45.0f, 1.0f, 0.1f, 100.0f);
		projection = projection.transpose();

		handleFPS(window);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glUniform1f(glGetUniformLocation(program, "u_time"), glfwGetTime());
		glUniformMatrix4fv(glGetUniformLocation(program, "u_view"), 1, GL_FALSE, view.getElements());
		glUniformMatrix4fv(glGetUniformLocation(program, "u_projection"), 1, GL_FALSE, projection.getElements());

		glBindTexture(GL_TEXTURE_2D, blackStoneTexture.id);
		glBindVertexArray(vao);

		int count = 2;
		for (int x = -count; x <= count; x++) {
			for (int y = -count; y <= count; y++) {
				maths::Mat4 model = maths::Mat4::translation(maths::Vec3(x * 2, y * 2, 0.0f));
				model = model * maths::Mat4::rotation((float)(glfwGetTime() + x + y) / 2.0f, maths::Vec3(1.0f, 0.0f, 0.0f));
				model = model * maths::Mat4::rotation((float)(glfwGetTime() + x + y) / 2.0f, maths::Vec3(0.0f, 1.0f, 0.0f));
				model = model * maths::Mat4::rotation((float)(glfwGetTime() + x + y) / 2.0f, maths::Vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(program, "u_model"), 1, GL_FALSE, model.transpose().getElements());
				glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, nullptr);
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}