#pragma once
#include <GLFW/glfw3.h>

#include "maths/Vec3.hpp"
#include "maths/Mat4.hpp"
#include "maths/Utils.hpp"

class OrbitCamera
{
	public:
		Vec3 position;
		Vec3 target;
		float distance;
		float pitch, yaw;
		Vec3 worldUp;

		OrbitCamera(
			Vec3 target = Vec3(0.0f, 0.0f, 0.0f),
			float distance = 10.0f,
			float pitch = 0.0f,
			float yaw = 0.0f,
			Vec3 worldUp = Vec3(0.0f, 1.0f, 0.0f)
		);

		Mat4 getViewMatrix();
		void processMouseInput(float xOffset, float yOffset);
		void processKeyboardInput(GLFWwindow *window, float deltaTime);
		void processMouseScroll(float yOffset);
		void updateCamera();
};