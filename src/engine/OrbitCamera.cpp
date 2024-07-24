#include "engine/OrbitCamera.hpp"

OrbitCamera::OrbitCamera(Vec3 target, float distance, float pitch, float yaw, Vec3 worldUp):
	target(target),
	distance(distance),
	pitch(pitch),
	yaw(yaw),
	worldUp(worldUp)
{
	updateCamera();
}

Mat4 OrbitCamera::getViewMatrix()
{
	return Mat4::lookAt(position, target, worldUp);
}

void OrbitCamera::processMouseInput(float xOffset, float yOffset)
{
	xOffset *= 0.1f;
	yOffset *= 0.1f;

	yaw += xOffset;
	pitch += yOffset;
	pitch = maths::clamp(pitch, -89.0f, 89.0f);

	updateCamera();
}

void OrbitCamera::processKeyboardInput(GLFWwindow *window, float deltaTime)
{
	float rotateSpeed = 45.0f * deltaTime;
	float moveSpeed = 5.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pitch += rotateSpeed;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pitch -= rotateSpeed;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		yaw += rotateSpeed;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		yaw -= rotateSpeed;

	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		distance -= moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		distance += moveSpeed;

	pitch = maths::clamp(pitch, -89.0f, 89.0f);
	distance = maths::clamp(distance, 1.0f, 45.0f);

	updateCamera();
}

void OrbitCamera::processMouseScroll(float yOffset)
{
	distance -= yOffset;
	distance = maths::clamp(distance, 1.0f, 45.0f);

	updateCamera();
}

void OrbitCamera::updateCamera()
{
	position.x = target.x + distance * cos(maths::radians(yaw)) * cos(maths::radians(pitch));
	position.y = target.y + distance * sin(maths::radians(pitch));
	position.z = target.z + distance * sin(maths::radians(yaw)) * cos(maths::radians(pitch));
}