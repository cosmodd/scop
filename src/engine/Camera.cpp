#include "engine/Camera.hpp"

Camera::Camera(
	Vec3 position,
	float yaw,
	float pitch,
	Vec3 worldUp
):
	position(position),
	worldUp(worldUp),
	yaw(yaw),
	pitch(pitch),
	fov(45.0f),
	speed(2.5f),
	sensitivity(0.01f)
{
	updateCameraVectors();
}

Mat4 Camera::getViewMatrix()
{
	return Mat4::lookAt(position, position + direction, worldUp);
}

void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime)
{
	float cameraSpeed = speed * deltaTime;
	Vec3 right = direction.cross(worldUp).normalize();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += direction * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= direction * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= right * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += right * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position += worldUp * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		position -= worldUp * cameraSpeed;
}

void Camera::processMouseInput(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	Vec3 forward;
	forward.x = std::cos(maths::radians(yaw)) * std::cos(maths::radians(pitch));
	forward.y = std::sin(maths::radians(pitch));
	forward.z = std::sin(maths::radians(yaw)) * std::cos(maths::radians(pitch));
	direction = forward.normalize();
}