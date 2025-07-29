#include "Camera.h"

Camera::Camera(Vector3f pos, float fov, float aspect, float zNear, float zFar)
{
	this->pos = pos;
	this->forward = Vector3f(0.0f, 0.0f, 1.0f);
	this->up = Vector3f(0.0f, 1.0f, 0.0f);
    this->projection = glm::perspective(fov, aspect, zNear, zFar);

    this->fov = fov;
    this->aspect = aspect;
    this->zNear = zNear;
    this->zFar = zFar;

}

Camera::~Camera()
{
    int k = 0;
}

void Camera::UpdateAspectRatio(float ar)
{
    aspect = ar;
    this->projection = glm::perspective(fov, aspect, zNear, zFar);
}


glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(pos, pos + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return projection;
}

void Camera::MoveForward(float amt)
{
	pos += forward * amt;
}

void Camera::MoveRight(float amt)
{
	pos += glm::cross(up, forward) * amt;
}

void Camera::RotateX(float angle)
{
	Vector3f right = glm::normalize(glm::cross(up, forward));

	forward = Vector3f(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
	up = glm::normalize(glm::cross(forward, right));
}

void Camera::RotateY(float angle)
{
	static const Vector3f UP(0.0f, 1.0f, 0.0f);

	glm::mat4 rotation = glm::rotate(angle, UP);

	forward = Vector3f(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	up = Vector3f(glm::normalize(rotation * glm::vec4(up, 0.0)));
}
