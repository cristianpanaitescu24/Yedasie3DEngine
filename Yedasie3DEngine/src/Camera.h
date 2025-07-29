#pragma once

#include "Utils.h"

class Camera
{
public:
    Camera(Vector3f pos, float fov, float aspect, float zNear, float zFar);
    ~Camera();
	
    void            UpdateAspectRatio(float ar);

	glm::mat4		GetViewMatrix() const;
	glm::mat4		GetProjectionMatrix() const;

	Vector3f		GetForward() { return forward; }

	Vector3f		GetPosition() { return pos; };
	void			SetPosition(Vector3f pos) { this->pos = pos; }

	void			MoveForward(float amt);
	void			MoveRight(float amt);

	void			RotateX(float angle);
	void			RotateY(float angle);
protected:
private:
	glm::mat4 projection;
	Vector3f pos;
	Vector3f forward;
	Vector3f up;

    float fov;
    float aspect;
    float zNear;
    float zFar;

};

