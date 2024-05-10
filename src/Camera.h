#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	~Camera();
	void MoveForward(float distance);
	void MoveBackward(float distance);
	void StrafeLeft(float distance);
	void StrafeRight(float distance);
	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void SetEye(float x, float y, float z);
	void SetRotation(float yaw, float pitch, float roll);
	glm::mat4 GetViewTransform();
	glm::vec3 GetPosition();
private:
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f; //Rotation about Y axis. Angle measured in XZ plane, going clockwise from X+
	float pitch = 0.0f; //Rotation about the X axis. Angle measured in the YZ plane, going counter-clockwise from Z-
	float roll = 0.0f; //Rotation about the Z axis. Angle mesaured in XY plane, going clockwise from X+.

	void UpdateCenter();
};

