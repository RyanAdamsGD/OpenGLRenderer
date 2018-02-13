#ifndef MAINWOOD_H
#define MAINWOOD_H
#include <GL\glew.h>
#include <QtOpenGL\qglwidget>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "Camera.h"
#include <QtGui\QKeyEvent>
#include <QtGui\QMouseEvent>
#include <Qt\qevent.h>
#include "ParticleSystem.h"
#include "ShapeData.h"
#include "IParticleDrawer.h"
#include "TransformInfo.h"
#include "ShaderUniformManager.h"
#include <vector>
class ParticleSystemFire;

class MainGLWindow: public QGLWidget, public ParticleDrawer
{
	uint fboID;
	uint renderTex;
	glm::vec4 colorR, colorP;
	glm::vec3* currentParticle;
	float rayDistance;
	QPointF prevMousePos;
	uint geomteryInfoID;
	uint particleGeometryID, particleShaderID, particleRenderInfoID;
	uint lineGeometryID, lineShaderID, lineRenderInfoID;
	uint lightRenderable;
	float timer;
	TransformInfo transform;
	uint render;


	bool useShapeTexture;
	bool useDiffuseLighting;
	bool useSpecularLighting;

	uint passThroughShaderID;


	struct colorInfo{
		glm::vec3 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
	};

	std::vector<colorInfo> colors;

	
	struct light{
		colorInfo colors;
		glm::vec3 position;	
		glm::vec3 direction;
		float specularPower;
	};
	light lamp;

	struct particle
	{		
		const glm::vec3* position;
		const float* scale;
		const bool* showBoundingBox;
		uint renderableID;
		particle(const glm::vec3* position,const float* scale, const bool* showBoundingBox, uint id)
			:position(position),scale(scale),showBoundingBox(showBoundingBox),renderableID(id){}
	};
	std::vector<particle*> particles;

	struct line
	{
		const glm::vec3* start;
		const glm::vec3* end;
		uint renderableID;
		line(const glm::vec3* start,const glm::vec3* end, uint id)
			:start(start),end(end),renderableID(id){}
	};
	static const uint MAX_NUM_LINES = 500000;
	line* lines[MAX_NUM_LINES];
	uint nextLine;

	std::vector<glm::mat3> modelMat3;
	std::vector<glm::mat4> modelMat4;
	float currentRotation;
	uint noseID;


	Camera MainCamera;
	Camera DebugCamera;
	void writeOutModel()const;
	void setupLighting();
	void moveCamera(Camera& where);
	void zoomCamera(QWheelEvent*,Camera& where);
	void updateParticles();

	bool showDebugCamera;
	float buttonPressTimer;
	GLuint programID, defaultProgramID, ADSProgramID;
	void shaderLayoutSetup();
	void setupDebugShader(const glm::vec4* color);
	uint modelSetup(const ShapeData& data, glm::mat4 modelTransform, const char* vertexFileName, const char* fragmentFileName, DrawType type, const glm::vec4* color);
	uint modelSetup(const ShapeData& data, glm::mat4 modelTransform, const char* vertexFileName, const char* fragmentFileName, const char* textureFileName=NULL, DrawType type = DrawType::drawTriangle);
protected:
	void initializeGL();
	void paintGL();
	void resizeEvent(QResizeEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void wheelEvent(QWheelEvent*);
public:
	virtual void addParticle(const glm::vec3* position, const float* scale = &DEFAULT_SCALE, const glm::vec4* color = &DEFAULT_COLOR,const bool* showBoundBox = &DEFAULT_SHOW);
	void addLine(const glm::vec3* start, const glm::vec3* end, const glm::vec4* color = &DEFAULT_COLOR);
	virtual void clearParticleList();
	virtual void removeParticle(const glm::vec3* position);
	virtual void clearLineList();
	static uint ScreenWidth;
	static uint ScreenHeight;
	void update(float dt);
	MainGLWindow();
	~MainGLWindow();

};


#endif