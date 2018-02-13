#include "DebugShapes.h"
#include "glm\gtx\transform.hpp"
#include "ShapeGenerator.h"
#include "TextRenderer.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "GeometryManager.h"
#include "Renderable.h"

DebugShapes DebugShapes::instance;

#ifndef USING_DEBUG_SHAPES

DebugShapes::DebugShapes() {}

void DebugShapes::AddUpdatingBox(TransformInfo* xfm,
		float duration, 
		bool depthEnabled){}

void DebugShapes::AddLine( const glm::vec3& fromPosition,
	const glm::vec3& toPosition,
	float lineWidth,
	float duration,
	bool depthEnabled){}

void DebugShapes::AddArrow( const glm::vec3& fromPosition,
		const glm::vec3& toPosition,
		float lineWidth,
		float duration,
		bool depthEnabled){}

void DebugShapes::AddCross( const glm::vec3& position,
	float size,
	float duration,
	bool depthEnabled){}

void DebugShapes::AddSphere( const glm::vec3& centerPosition,
	float radius,
	float duration,
	bool depthEnabled){}

void DebugShapes::AddCircle( const glm::vec3& centerPosition,
	const glm::vec3& planeNormal,
	float radius,
	float duration,
	bool depthEnabled){}

void DebugShapes::AddAxes( const glm::mat4& xfm,
	float size,
	float duration,
	bool depthEnabled){}

void DebugShapes::AddAABB( const glm::vec3& minCoords,
	const glm::vec3& maxCoords,
	float lineWidth,
	float duration,
	bool depthEnabled){}

void DebugShapes::AddOBB( const glm::mat4& centerTransform,
	const glm::vec3& scaleXYZ,
	float lineWidth,
	float duration,
	bool depthEnabled){}

void DebugShapes::loadShapeData() {}
void DebugShapes::setUpTextures() {}
uint DebugShapes::findNextIndex() { return 0; }
void DebugShapes::update(float dt){}
void DebugShapes::drawText(){}
void DebugShapes::init(){}

#else

DebugShapes::DebugShapes()
	:nextDebugRenderable(0),nextUpdatingBox(0)
{

}

void DebugShapes::init()
{
	loadShapeData();
	setUpTextures();
}

void DebugShapes::setUpTextures()
{
	whiteTexture = Texture_Manager.addSolidColorTexture(glm::vec4(1.0f));
	redTexture = Texture_Manager.addSolidColorTexture(glm::vec4(1.0f,0.0f,0.0f,1.0f));
	blueTexture = Texture_Manager.addSolidColorTexture(glm::vec4(0.0f,1.0f,0.0f,1.0f));
	greenTexture = Texture_Manager.addSolidColorTexture(glm::vec4(0.0f,0.0f,1.0f,1.0f));
}

void DebugShapes::loadShapeData()
{
	//load all the shapes into geometries
	ShapeData data;
	data = Neumont::ShapeGenerator::makeVector(8);
	arrowGeometryID = Geometry_Manager.addGeometry(data);
	data.cleanUp();
	data = Neumont::ShapeGenerator::makeLine();
	lineGeometryID = Geometry_Manager.addGeometry(data,drawLine);
	data.cleanUp();
	data =  Neumont::ShapeGenerator::makeSphere(10);
	sphereGeometryID = Geometry_Manager.addGeometry(data);
	data.cleanUp();
	data = Neumont::ShapeGenerator::makeCircle(10);
	circleGeometryID = Geometry_Manager.addGeometry(data,drawLine);
	data.cleanUp();
	data = Neumont::ShapeGenerator::makeCircle(1);
	triangleGeometryID = Geometry_Manager.addGeometry(data);
	data.cleanUp();
	data = Neumont::ShapeGenerator::makeWireFrameCube();
	wfCubeGeometryID = Geometry_Manager.addGeometry(data,drawLine);
	data.cleanUp();
	data = Neumont::ShapeGenerator::makeCube();
	cubeID = Geometry_Manager.addGeometry(data);
	data.cleanUp();
}

void DebugShapes::AddUpdatingBox(TransformInfo* xfm, float duration, bool depthEnabled)
{
	uint index = findNextIndex();
	DebugUpdatingBox& where = updatingBoxes[nextUpdatingBox++];
	where.debugShapeID = index;
	where.xfm = xfm;

	shapes[index].renderableID = Render_Manager.addRenderable(cubeID,xfm,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(whiteTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddArrow( const glm::vec3& fromPosition, const glm::vec3& toPosition,
	float lineWidth,float duration,	bool depthEnabled)
{
	uint index = findNextIndex();
	//dot cross product rotation to point from one point to another
	glm::vec3 rot = toPosition - fromPosition;
	//get length to scale by
	float length = glm::length(rot);
	float rotationAngle= acosf(glm::dot(glm::normalize(glm::vec3(0.0f,0.0f,1.0f)),glm::normalize(rot)));
	//convert to radians
	float convertedRotationAngle = rotationAngle * 180/PI;
	glm::vec3 cross = glm::cross(glm::vec3(0.0f,0.0f,0.5f),rot);
	if(cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f)
	{
		cross.x = 1.0f;
		convertedRotationAngle = 0;
	}
	glm::mat4 transform = glm::translate(fromPosition) * glm::rotate(convertedRotationAngle,cross) * glm::scale(glm::vec3(lineWidth,lineWidth,length)) * glm::translate(0.0f,0.0f,0.5f) * glm::scale(0.5f,0.5f,-0.5f);
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(arrowGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(whiteTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddLine( const glm::vec3& fromPosition, const glm::vec3& toPosition,
	float lineWidth,float duration,	bool depthEnabled)
{
	uint index = findNextIndex();
	//dot cross product rotation to point from one point to another
	glm::vec3 rot = toPosition - fromPosition;
	//get length to scale by
	float length = glm::length(rot);
	float rotationAngle= acosf(glm::dot(glm::normalize(glm::vec3(0.0f,0.0f,1.0f)),glm::normalize(rot)));
	//convert to radians
	float convertedRotationAngle = rotationAngle * 180/PI;
	glm::vec3 cross = glm::cross(glm::vec3(0.0f,0.0f,0.5f),rot);
	if(cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f)
	{
		cross.x = 1.0f;
		convertedRotationAngle = 0;
	}
	glm::mat4 transform = glm::translate(fromPosition) * glm::rotate(convertedRotationAngle,cross) * glm::scale(glm::vec3(lineWidth,lineWidth,length)) * glm::translate(0.0f,0.0f,0.5f) * glm::scale(0.5f,0.5f,-0.5f);
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(whiteTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddCross( const glm::vec3& position, float size, float duration, bool depthEnabled)
{
	//z axis
	glm::mat4 transform = glm::translate(position) * glm::scale(size,size,size) * glm:: translate(-0.5f,0.0f,0.0f);
	//y axis
	glm::mat4 transform1 = glm::translate(position) * glm::rotate(90.0f,0.0f,0.0f,1.0f) * glm::scale(size,size,size) * glm:: translate(-0.5f,0.0f,0.0f);
	//x axis
	glm::mat4 transform2 = glm::translate(position) * glm::rotate(90.0f,0.0f,1.0f,0.0f) * glm::scale(size,size,size) * glm:: translate(-0.5f,0.0f,0.0f);
	
	uint index = findNextIndex();
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(redTexture);
	shapes[index].duration = duration;

	index = findNextIndex();
	shapes[index].transform.setMatrix(transform1);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(blueTexture);
	shapes[index].duration = duration;

	index = findNextIndex();
	shapes[index].transform.setMatrix(transform2);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(greenTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddSphere( const glm::vec3& centerPosition,float radius, float duration,	bool depthEnabled)
{
	uint index = findNextIndex();
	glm::vec4 baseVec = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glm::mat4 transform = glm::translate(centerPosition) * glm::scale(radius,radius,radius);
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(sphereGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(whiteTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddCircle( const glm::vec3& centerPosition,const glm::vec3& planeNormal,
	float radius,float duration,bool depthEnabled)
{
	//get dot product for rotation angle
	float dotProd = glm::dot(glm::normalize(glm::vec3(0.0f,0.0f,1.0f)),glm::normalize(planeNormal));
	float rotationAngle= acosf(dotProd);
	//convert radian degrees
	float convertedRotationAngle = rotationAngle * 180/PI;
	glm::vec3 cross = glm::cross(planeNormal,glm::vec3(0.0f,0.0f,1.0f));
	//check to make sure cross product returns a valid axis for rotation
	if(cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f)
	{
		//if axis is invalid remove rotation
		cross.x = 1.0f;
		convertedRotationAngle = 0;
	}
	uint index = findNextIndex();
	glm::mat4 transform = glm::translate(centerPosition) * glm::rotate(convertedRotationAngle,cross) * glm::scale(radius,radius,1.0f) * glm::translate(0.0f,-0.5f,0.0f);
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(circleGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(whiteTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddAxes( const glm::mat4& xfm, float size,float duration,bool depthEnabled)
{
	//z axis
	glm::mat4 transform = xfm * glm::scale(size,size,size) * glm:: translate(-0.5f,0.0f,0.0f);
	//y axis
	glm::mat4 transform1 = xfm * glm::rotate(90.0f,0.0f,0.0f,1.0f) * glm::scale(size,size,size) * glm:: translate(-0.5f,0.0f,0.0f);
	//x axis
	glm::mat4 transform2 = xfm * glm::rotate(90.0f,0.0f,1.0f,0.0f) * glm::scale(size,size,size) * glm:: translate(-0.5f,0.0f,0.0f);

	uint index = findNextIndex();
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(redTexture);
	shapes[index].duration = duration;
	
	index = findNextIndex();
	shapes[index].transform.setMatrix(transform1);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(blueTexture);
	shapes[index].duration = duration;
	
	index = findNextIndex();
	shapes[index].transform.setMatrix(transform2);
	shapes[index].renderableID = Render_Manager.addRenderable(lineGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(greenTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddAABB( const glm::vec3& minCoords,const glm::vec3& maxCoords,
	float lineWidth,float duration,bool depthEnabled)
{
	uint index = findNextIndex();
	//get the proper scale
	float scalex = (maxCoords.x - minCoords.x) /2;
	float scaley = (maxCoords.y - minCoords.y) /2;
	float scalez = (maxCoords.z - minCoords.z) /2;
	//move the scaled box to the proper position
	glm::mat4 transform = glm::translate(minCoords) * glm::scale(scalex,scaley,scalez);
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(wfCubeGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(greenTexture);
	shapes[index].duration = duration;
}

void DebugShapes::AddOBB( const glm::mat4& centerTransform,	const glm::vec3& scaleXYZ,
	float lineWidth, float duration, bool depthEnabled)
{
	uint index = findNextIndex();
	glm::mat4 transform = centerTransform * glm::scale(scaleXYZ) * glm::translate(-0.5f,-0.5f,-0.5f);
	shapes[index].transform.setMatrix(transform);
	shapes[index].renderableID = Render_Manager.addRenderable(wfCubeGeometryID,&shapes[index].transform,false,true,depthEnabled);
	Render_Manager.getRenderable(shapes[index].renderableID)->addDiffuseMap(greenTexture);
	shapes[index].duration = duration;
}

void DebugShapes::update(float dt)
{	
	for(uint i=0;i<nextUpdatingBox;i++)
	{
		DebugUpdatingBox& where = updatingBoxes[i];
		if(shapes[where.debugShapeID].duration <= 0)
		{
			Render_Manager.removeRenderable(shapes[i].renderableID);
		}
	}
	for(uint i=0;i<nextDebugRenderable;i++)
	{
		if(shapes[i].duration <= 0)
		{
			Render_Manager.removeRenderable(shapes[i].renderableID);
		}
		else
		{
			shapes[i].duration -= dt;
		}
	}
}

uint DebugShapes::findNextIndex()
{
	//find the next open index
	uint index=0;
	for(uint i=0;i<nextDebugRenderable;i++)
	{
		if(shapes[i].duration <= 0)
		{
			index = i;
			break;
		}
		else
		{
			index = nextDebugRenderable;
		}
	}
	if(index == nextDebugRenderable)
	{
		nextDebugRenderable++;
	}
	assert(index < MAX_NUM_DEBUGRENDERABLE);
	return index;
}
#endif