#pragma once
#include "Object.h"

#include "glm/vec3.hpp"

constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEAR_PLANE = 1.0e-01f;
constexpr float DEFAULT_FAR_PLANE = 1.0e+03f;

class Camera : public Object
{
public:
	Camera(const std::string& name);

	void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
	float GetNearPlane() const { return m_nearPlane; }

	void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
	float GetFarPlane() const { return m_farPlane; }

	void SetFOV(float fov) { m_fov = fov; }
	float GetFOV() const { return m_fov; }

	void SetClearColor(glm::vec3 color) { m_clearColor = color; }
	glm::vec3 GetClearColor() const { return m_clearColor; }

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjMatrix(float width, float height) const;

private:
	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	glm::vec3 m_clearColor;

};