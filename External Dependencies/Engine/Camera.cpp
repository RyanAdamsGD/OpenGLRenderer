#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include "Ray.h"
#include "LuaState.h"

#define MOVE_AROUND_FOCUS

#ifdef ENGINE_EXPORTS
float Camera::zoomSpeed = 0.0f;
float Camera::cameraRotation = 0.0f;
float Camera::movementSpeed = 0.0f;
#endif

Camera::Camera()
	:position(0.0f,0.0f,5.0f),focus(0.0f,0.0f,-15.0f),up(0.0f,1.0f,0.0f),viewPortStart(0),aspectRatio(0.5f),
	viewAngle(45.0f),nearPlane(0.1f),farPlane(1000.0f)
{
	LuaState luaState("../../External Dependencies/Engine/config.lua");
	zoomSpeed = luaState.get<float>("ZoomSpeed");
	cameraRotation = luaState.get<float>("RotationSpeed");
	movementSpeed = luaState.get<float>("MovementSpeed");
	viewPortSize =  glm::vec2(luaState.get<float>("ScreenWidth"),luaState.get<float>("ScreenHeight"));

	computeUVW();
	computeProjection();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& lookat, const glm::vec2& viewPortSize)
	:position(position),focus(lookat),viewPortSize(viewPortSize),up(0.0f,1.0f,0.0f),viewPortStart(0),aspectRatio(0.5f),
	viewAngle(45.0f),nearPlane(0.1f),farPlane(1000.0f)
{
	LuaState luaState("../../External Dependencies/Engine/config.lua");
	zoomSpeed = luaState.get<float>("ZoomSpeed");
	cameraRotation = luaState.get<float>("RotationSpeed");
	movementSpeed = luaState.get<float>("MovementSpeed");

	computeUVW();
	computeProjection();
}

void Camera::computeUVW()
{
	w = position - focus;
	w = glm::normalize(w);
	u = glm::cross(up,w);
	u = glm::normalize(u);
	v = glm::cross(w,u);
	glm::vec3 s(-glm::dot(position,u),-glm::dot(position,v),-glm::dot(position,w));
	
	view = glm::mat4(u.x,v.x,w.x,0,u.y,v.y,w.y,0,u.z,v.z,w.z,0,s.x,s.y,s.z,1);
}

void Camera::yaw(Direction dir)
{
	switch (dir)
	{
	case RIGHT:
		focus = glm::normalize(glm::vec3(glm::vec4(focus,0) * glm::rotate(cameraRotation,v)));
		break;
	case LEFT:
		focus = glm::normalize(glm::vec3(glm::vec4(focus,0) * glm::rotate(-cameraRotation,v)));
		break;
	}
}

void Camera::pitch(Direction dir)
{
	glm::mat4 rotation;
	switch (dir)
	{
	case UP:
		rotation = glm::rotate(-cameraRotation,u);		
		break;
	case DOWN:
		rotation = glm::rotate(cameraRotation,u);		
		break;
	}
	focus = glm::normalize(glm::vec3(glm::vec4(focus,0) * rotation));
	up = glm::normalize(glm::vec3(glm::vec4(up,0) * rotation));
}

void Camera::spin(Direction dir)
{
	glm::quat rotationQuat;

	switch(dir)
	{
	case LEFT:
		rotationQuat = glm::angleAxis(-cameraRotation,glm::vec3(0,1,0));
		position = (position - focus) * rotationQuat;
		position += focus;
		up = up * rotationQuat;
		break;
	case RIGHT:
		rotationQuat = glm::angleAxis(cameraRotation,glm::vec3(0,1,0));
		position = (position - focus) * rotationQuat;
		position += focus;
		up = up * rotationQuat;
		break;
	case UP:
		rotationQuat = glm::angleAxis(-cameraRotation,u);
		position = (position - focus) * rotationQuat;
		position += focus;
		up = up * rotationQuat;
		break;
	case DOWN:
		rotationQuat = glm::angleAxis(cameraRotation,u);
		position = (position - focus) * rotationQuat;
		position += focus;
		up = up * rotationQuat;
		break;
	}
}

void Camera::MovePosition(Direction dir)
{
	glm::vec3 movement;
	switch(dir)
	{
	case LEFT:
		movement= -u * movementSpeed;
		break;
	case RIGHT:
		movement= u * movementSpeed;
		break;
	case UP:
#ifndef MOVE_AROUND_FOCUS
		movement = v * movementSpeed;
#else
		movement = up * -movementSpeed;
#endif
		break;
	case DOWN:
#ifndef MOVE_AROUND_FOCUS
		movement = -up * movementSpeed;
#else
		movement = up * movementSpeed;
#endif
		break;
	case BACKWARD:
#ifndef MOVE_AROUND_FOCUS
		movement = -w * zoomSpeed;
#else
		movement = w * zoomSpeed;
#endif
		break;
	case FORWARD:
#ifndef MOVE_AROUND_FOCUS
		movement = w * zoomSpeed;
#else
		movement = w * -zoomSpeed;
#endif
		break;
	}
	focus += movement;
	position += movement;	
	computeUVW();
}

void Camera::RotateCamera(Direction move)
{
	switch(move)
	{
#ifndef MOVE_AROUND_FOCUS
	case LEFT:
		yaw(LEFT);
		break;
	case RIGHT:
		yaw(RIGHT);
		break;
	case UP:
		pitch(UP);
		break;
	case DOWN:
		pitch(DOWN);
		break;
#else
	case LEFT:
		spin(LEFT);
		break;
	case RIGHT:
		spin(RIGHT);
		break;
	case UP:
		spin(UP);
		break;
	case DOWN:
		spin(DOWN);
		break;
#endif
	}
	computeUVW();
}

Ray Camera::rayCast(const glm::vec2& mousePosition) const
{
	glm::vec3 v;
	float screenWidth = viewPortSize.x - viewPortStart.x;
	float screenHeight = viewPortSize.y - viewPortStart.y;
	v.x = (((2.0f * mousePosition.x) / screenWidth) - 1) / projection[0][0];
	v.y = (((2.0f * mousePosition.y) / screenHeight) - 1) / projection[1][1];
	v.z = -1.0f;

	glm::mat4 m = glm::inverse(view);
	glm::vec3 rayDir, rayOrigin;
	rayDir.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	rayDir.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	rayDir.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
	rayOrigin.x = m[3][0];
	rayOrigin.y = m[3][1];
	rayOrigin.z = m[3][2];

	rayDir = glm::normalize(rayDir);
	return Ray(rayOrigin,rayDir);
}

void Camera::computeProjection()
{
	projection = glm::perspective(viewAngle,aspectRatio,nearPlane,farPlane);
}

void Camera::setAspectRatio(uint screenWidth, uint screenHeight)
{
	aspectRatio = (float)(screenWidth)/screenHeight;
	computeProjection();
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	computeProjection();
}

void Camera::setNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
	computeProjection();
}

void Camera::setFarPlane(float farPlane)
{
	this->farPlane = farPlane;
	computeProjection();
}

void Camera::setViewAngle(float viewAngle)
{
	this->viewAngle = viewAngle;
	computeProjection();
}

void Camera::setPosition(const glm::vec3& position)
{
	glm::vec3 translation = position - this->position;
	this->position = position;
	focus += translation;
	computeUVW();
}
