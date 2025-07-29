#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include "Utils.h"
#include "camera.h"

struct Transform
{
public:
	Transform(const Vector3f& pos = Vector3f(), const Vector3f& rot = Vector3f(), const Vector3f& scale = Vector3f(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(pos);
		glm::mat4 scaleMat = glm::scale(scale);
		glm::mat4 rotX = glm::rotate(rot.x, Vector3f(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(rot.y, Vector3f(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(rot.z, Vector3f(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;

		return posMat * rotMat * scaleMat;
	}

	inline Vector3f* GetPos() { return &pos; }
	inline Vector3f* GetRot() { return &rot; }
	inline Vector3f* GetScale() { return &scale; }

	inline void SetPos(Vector3f& pos) { this->pos = pos; }
	inline void SetRot(Vector3f& rot) { this->rot = rot; }
	inline void SetScale(Vector3f& scale) { this->scale = scale; }
protected:
private:
	Vector3f pos;
	Vector3f rot;
	Vector3f scale;
};

#endif
