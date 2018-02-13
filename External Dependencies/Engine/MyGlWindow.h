#ifndef MLGLWINDOW_H
#define MLGLWINDOW_H
#include <GL\glew.h>
#include <QtOpenGL\qglwidget>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Camera.h"
#include <QtGui\QKeyEvent>
#include "GraphicObject.h"

class MyGlWindow: public QGLWidget
{
	GLuint vertexShaderID;
	GLuint fragShaderID;
	GLuint numindicies;
	float aspect,viewAngle,zn,zf;
	void compileShader(GLuint shaderID, const char* fileName);
	Camera MainCamera;
	void moveCamera(QKeyEvent*);
	void rotateCamera(QMouseEvent*);
	bool lockMouse;
	GraphicObject arrow;
	QPointF prevMousePos;
protected:
	void initializeGL();
	void shutdown();
	void paintGL();
	void resizeEvent(QResizeEvent*);
	void keyPressEvent(QKeyEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void createShader();
public:
	static glm::mat4 projection;
	static float ScreenWidth;
	static float ScreenHeight;
	GLuint programID;
	void update();
};


#endif