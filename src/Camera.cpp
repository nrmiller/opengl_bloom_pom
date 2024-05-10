#include "Camera.h"
#include "Includes.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::MoveForward(float distance)
{
	eye += center * distance;
}

void Camera::MoveBackward(float distance)
{
	eye -= center * distance;
}

void Camera::StrafeLeft(float distance)
{
	eye -= glm::normalize(glm::cross(center, up)) * distance;
}

void Camera::StrafeRight(float distance)
{
	eye += glm::normalize(glm::cross(center, up)) * distance;
}

void Camera::RotateYaw(float angle)
{
	yaw += angle;

	UpdateCenter();
}

void Camera::RotatePitch(float angle)
{
	pitch += angle;
	
	if (pitch > 89.99f)
		pitch = 89.99f;
	if (pitch < -89.99f)
		pitch = -89.99f;

	UpdateCenter();
}

void Camera::SetEye(float x, float y, float z)
{
	eye = glm::vec3(x, y, z);
}

void Camera::SetRotation(float yaw, float pitch, float roll)
{
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;

	UpdateCenter();
}


// This function returns the inverse camera matrix (view) transform.
glm::mat4 Camera::GetViewTransform()
{
	/*TCHAR format[256];
	StringCchPrintf(format, 256, _T("(%0.2f, %0.2f, %0.2f) -> (%0.2f, %0.2f, %0.2f) | UP(%0.2f, %0.2f, %0.2f)\n"), eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	OutputDebugString(format);*/

	return glm::lookAt(eye, eye + center, up);
}

glm::vec3 Camera::GetPosition()
{
	//Return a copy of the eye vector.
	return glm::vec3(eye.x, eye.y, eye.z);
}

void Camera::UpdateCenter()
{
	center.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	center.y = sin(glm::radians(pitch));
	center.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}