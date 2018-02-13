#ifndef CAMERA_H
#define CAMERA_H
#include "glm\gtx\transform.hpp"
#include "MyTypeDefs.h"
#include "ExportHeader.h"

class Ray;

class ENGINE_SHARED Camera
{
	glm::vec3 focus, up;
	glm::vec3 position;
	glm::vec3 u,v,w;
	glm::mat4 view;
	glm::vec2 viewPortStart;
	glm::vec2 viewPortSize;

	void yaw(Direction dir);
	void pitch(Direction dir);
	void spin(Direction dir);

	static float zoomSpeed;
	static float cameraRotation;
	static float movementSpeed;
	void computeUVW();

	float aspectRatio;
	float viewAngle;
	float nearPlane;
	float farPlane;
	glm::mat4 projection;
	void computeProjection();

public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& lookat, const glm::vec2& viewPortSize);
	inline const glm::mat4& getCameraView() const { return view; }
	inline const glm::vec3& getPosition() const { return position; }
	inline const glm::vec3& getFocus() const { return focus; }
	inline const glm::mat4& getProjection() const { return projection; }
	inline const float& getViewAngle() const { return viewAngle; }
	inline const float& getNearPlane() const { return nearPlane; }
	inline const float& getFarPlane() const { return farPlane; }
	inline const glm::vec2& getViewPortStart() const { return viewPortStart; }
	inline const glm::vec2& getViewPortSize() const { return viewPortSize; }

	Ray rayCast(const glm::vec2& mousePosition) const;
	void MovePosition(Direction);
	void RotateCamera(Direction);
	void Zoom(float ratio);

	void setAspectRatio(uint screenWidth, uint screenHeight);
	void setAspectRatio(float aspectRatio);
	void setNearPlane(float nearPlane);
	void setFarPlane(float farPlane);
	void setViewAngle(float viewAngle);
	void setPosition(const glm::vec3& position);
	void setViewPortStart(const glm::vec2& value) { viewPortStart = value; }
	void setViewPortSize(const glm::vec2& value) { viewPortSize = value; }
};

#endif