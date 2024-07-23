#pragma once
#include <GLFW/glfw3.h>
#include <cmath>

#include "maths/Vec3.hpp"
#include "maths/Mat4.hpp"
#include "maths/Utils.hpp"

class Camera
{
	public:
		Vec3 position, direction, worldUp;
		float yaw, pitch;
		float fov, speed, sensitivity;

		Camera(
			Vec3 position = Vec3(0.0f, 0.0f, 0.0f),
			float yaw = -90.0f,
			float pitch = 0.0f,
			Vec3 worldUp = Vec3(0.0f, 1.0f, 0.0f)
		);

		Mat4 getViewMatrix();
		void updateCameraVectors();
		void processKeyboardInput(GLFWwindow* window, float deltaTime);
		void processMouseInput(float xOffset, float yOffset, bool constrainPitch = true);
};