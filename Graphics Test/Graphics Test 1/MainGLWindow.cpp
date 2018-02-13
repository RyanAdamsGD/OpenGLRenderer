#include "MainGLWindow.h"
#include <iostream>
#include "ShapeGenerator.h"
#include "AutoTimer.h"
#include "RenderManager.h"
#include "DebugShapes.h"
#include "TextRenderer.h"
#include "ShapeLoader.h"
#include "QTDebugMenu.h"
#include "Helper Functions.h"
#include "Ray.h"
#include "ShapeExporter.h"
#include "FileTexture.h"
#include "Renderable.h"
#include "FastDelegate.h"
#include "Profiler.h"
//#define MOVE_PARTICLES

uint MainGLWindow::ScreenWidth=1024;
uint MainGLWindow::ScreenHeight=1024;
#define P_WIDTH 0
#define P_HEIGHT 50
MainGLWindow::MainGLWindow()
	:currentParticle(NULL),buttonPressTimer(0),nextLine(0),colorP(105.0f/255.0f,1.0f,100.0f/255.0f,1.0f), colorR(0.3f,0.3f,0.4f,0.5f),
	programID(0),showDebugCamera(false),prevMousePos(0,0),geomteryInfoID(0),
	modelMat3(),modelMat4(),timer(10.0f),currentRotation(0),noseID(0)
{}

void MainGLWindow::initializeGL()
{
	frameCount = 0;
	this->startTimer(1);
	glewInit();
	//force a resize event to allow the shadows to work properly
	QResizeEvent resize(QSize(ScreenWidth,ScreenHeight),QSize(ScreenWidth,ScreenHeight));
	resizeEvent(&resize);
	glClearColor(.75f,0.75f,0.8f,1.0f);
	setMinimumSize(ScreenWidth,ScreenHeight);
	setFocusPolicy(Qt::StrongFocus);
	//const char* temp = reinterpret_cast<const char*> (glGetString(GL_VERSION));
	//CreateFrameBufferObject();
	//setupLighting();

	uint geoID = Geometry_Manager.addGeometry(Neumont::ShapeGenerator::makeSphere(100));
	for(uint i=0;i<1024;i++)
		render = Render_Manager.addRenderable(geoID,&transform);
	Render_Manager.addCamera(&MainCamera);

	/*if(P_WIDTH > 0 && P_HEIGHT > 0)
		geomteryInfoID = modelSetup(Neumont::ShapeGenerator::makePlane(P_WIDTH,P_HEIGHT),glm::mat4(),"VertexShaderSendToFragWithShadows.txt","FragShaderCalculateADSWithShadows.txt",0,DrawType::drawTriangle);*/




	//lightRenderable = modelSetup(Neumont::ShapeGenerator::makeCube(),glm::translate(lamp.position) * glm::scale(glm::vec3(0.5f,0.5f,0.5f)),"MyVertexShader.txt","MyFragmentShader.txt",0,DrawType::drawTriangle);


	//ShapeData data = Neumont::ShapeGenerator::makeSphere(10);
	//particleGeometryID = Renderer.createGeomtry(data.verts,data.indices,data.vertexBufferSize(),data.indexBufferSize(),data.numVerts,data.numIndices,Vertex::STRIDE,Vertex::POSITION_OFFSET,Vertex::COLOR_OFFSET,Vertex::NORMAL_OFFSET,Vertex::UV_OFFSET,drawTriangle);
	//particleShaderID = Renderer.createShader("DebugShapeVertexShader.txt","MyFragmentShader.txt");
	//particleRenderInfoID = Renderer.createRenderInfo(debugShaderP);

	//data = Neumont::ShapeGenerator::makeLine();
	//lineGeometryID = Renderer.createGeomtry(data.verts,data.indices,data.vertexBufferSize(),data.indexBufferSize(),data.numVerts,data.numIndices,Vertex::STRIDE,Vertex::POSITION_OFFSET,Vertex::COLOR_OFFSET,Vertex::NORMAL_OFFSET,Vertex::UV_OFFSET,drawLine);
	//lineShaderID = Renderer.createShader("DebugShapeVertexShader.txt","MyFragmentShader.txt");
	//lineRenderInfoID = Renderer.createRenderInfo(debugShaderR);

	Debug_Shapes.init();
}

void MainGLWindow::mouseMoveEvent(QMouseEvent* mouse)
{
	if(currentParticle != NULL)
	{
		prevMousePos = mouse->posF();
	}
}

void MainGLWindow::setupLighting()
{
	lamp.colors.ambientColor = glm::vec3(1.0f);
	lamp.colors.specularColor = glm::vec4(1.0f);
	lamp.colors.diffuseColor = glm::vec4(1.0f);
	lamp.position = glm::vec3(0.0,15.0,-5.0);
	lamp.direction = glm::vec3(0,-1,1);
	lamp.specularPower = 10.0f;
	//Renderer.addLight(lamp.position,lamp.direction);
}

void MainGLWindow::mousePressEvent(QMouseEvent* e)
{
	prevMousePos = e->posF();	

#ifdef MOVE_PARTICLES
	Ray ray = MainCamera.rayCast(glm::vec2(prevMousePos.x(), ScreenHeight - prevMousePos.y()),ScreenWidth,ScreenHeight);
	if(currentParticle == NULL)
	{
		uint size = particles.size();
		for(uint i=0;i<size;i++)
		{
			float rayDist = ray.intersetectsSphere(*particles[i]->position,*particles[i]->scale/10 + 0.15);
			std::cout<<rayDist<<std::endl;
			if( rayDist > 0)
			{
				rayDistance = rayDist;
				currentParticle = const_cast<glm::vec3*>(particles[i]->position);
				return;
			}
		}
	}
#endif
}

void MainGLWindow::mouseReleaseEvent(QMouseEvent* e)
{
	currentParticle = NULL;
}

void MainGLWindow::update(float dt)
{
	buttonPressTimer+=dt;

	if(showDebugCamera)
	{
		moveCamera(DebugCamera);
	}
	else
	{
		moveCamera(MainCamera);			
		DebugCamera = MainCamera;
	}

	if(currentParticle != NULL)
	{
		Ray ray = MainCamera.rayCast(glm::vec2(prevMousePos.x(),ScreenHeight - prevMousePos.y()));
		*currentParticle = rayDistance * ray.getDirection() + ray.getOrigin();
	}

	Debug_Shapes.update(dt);
	//Renderer.cameraPosition = MainCamera.getPosition();


	if(GetAsyncKeyState(VK_OEM_3) && buttonPressTimer >0.1f)
	{
		QtDebugMenu.visible();
		buttonPressTimer=0;
	}

	/*if(frameCount>200)
		writeOutModel()*/;

	timer-=dt;
	/*if(noseID != 0)
		Renderer.updateRenderablePosition(noseID, glm::rotate(currentRotation,0.0f,0.0f,1.0f) * glm::scale(0.1f,0.1f,0.1f));*/
	currentRotation += dt * 10000.0f;

	updateGL();	
}

MainGLWindow::~MainGLWindow()
{
}

void MainGLWindow::paintGL()
{
	//this needs to be here
	//Renderer.updateLightPosition(0,lamp.position);
	//Renderer.updateRenderablePosition(lightRenderable,glm::translate(lamp.position) * glm::scale(glm::vec3(0.5f,0.5f,0.5f)));
	updateParticles();

	
	//Left viewport
	//if(showDebugCamera)
	//{
	//	//Right viewport
	//	glViewport(width()/2,0,width()/2,height());
	//	Renderer.draw(DebugCamera.getProjection(),DebugCamera.getCameraView());
	//	DebugShapes::drawText();
	//	glViewport(0,0,width()/2,height());
	//}
	Render_Manager.draw();
	//Debug_Shapes.drawText();
}

void MainGLWindow::resizeEvent(QResizeEvent*)
{
	ScreenWidth=width();
	ScreenHeight=height();

	//field of view matrix
	MainCamera.setViewPortSize(glm::vec2(ScreenWidth,ScreenHeight));
	MainCamera.setAspectRatio(ScreenWidth,ScreenHeight);
	//Renderer.resizeFrameBuffer(ScreenWidth,ScreenHeight);
}

void MainGLWindow::wheelEvent(QWheelEvent* e)
{
	if(showDebugCamera)
	{
		zoomCamera(e, DebugCamera);
	}
	else
	{
		zoomCamera(e, MainCamera);
		DebugCamera=MainCamera;
	}	
}

void MainGLWindow::zoomCamera(QWheelEvent* e,Camera& where)
{
	
	if(e->orientation() == Qt::Vertical)
	{
		if(e->delta() > 0)
			where.MovePosition(FORWARD);
		else
			where.MovePosition(BACKWARD);
	}
}

void MainGLWindow::moveCamera(Camera& where)
{	
	QPointF cursorPos = mapFromGlobal(QCursor::pos());
	if(GetAsyncKeyState(VK_MBUTTON))
	{
		if(fabs(cursorPos.x() - prevMousePos.x()) > fabs(cursorPos.y() - prevMousePos.y()))
		{
			if(cursorPos.x() < prevMousePos.x())
			{		
				where.MovePosition(RIGHT);
			}
			else
			{		
				where.MovePosition(LEFT);
			}
		}
		else if(fabs(cursorPos.x() - prevMousePos.x()) < fabs(cursorPos.y() - prevMousePos.y()))
		{
			if(cursorPos.y() > prevMousePos.y())
			{		
				where.MovePosition(DOWN);
			}
			else
			{		
				where.MovePosition(UP);
			}
		}
	}

	if(GetAsyncKeyState(VK_LBUTTON))
	{
		if(fabs(cursorPos.x() - prevMousePos.x()) > fabs(cursorPos.y() - prevMousePos.y()))
		{
			if(cursorPos.x() < prevMousePos.x())
			{		
				where.RotateCamera(LEFT);
			}
			else 
			{		
				where.RotateCamera(RIGHT);
			}
		}
		else if(fabs(cursorPos.x() - prevMousePos.x()) < fabs(cursorPos.y() - prevMousePos.y()))
		{
			if(cursorPos.y() > prevMousePos.y())
			{		
				where.RotateCamera(DOWN);
			}
			else
			{		
				where.RotateCamera(UP);
			}
		}
	}

	prevMousePos = cursorPos;

	//if(GetAsyncKeyState('P'))
		//Renderer.writeBuffers();

	if(GetAsyncKeyState('A'))
	{
		Render_Manager.getRenderable(render)->addDiffuseMap(Texture_Manager.addSolidColorTexture(glm::vec4(1.0f)));

		lamp.position.x -= 0.05f;
		//where.MovePosition(LEFT);
	}
	if(GetAsyncKeyState('D'))
	{
		lamp.position.x += 0.05f;
		//where.MovePosition(RIGHT);
	}
	if(GetAsyncKeyState('S'))
	{
		lamp.position.z += 0.05f;
		//where.MovePosition(BACKWARD);
	}
	if(GetAsyncKeyState('W'))
	{
		lamp.position.z -= 0.05f;
		//where.MovePosition(FORWARD);
	}
	if(GetAsyncKeyState('R'))
	{
		lamp.position.y += 0.05f;
		//where.MovePosition(UP);
	}
	if(GetAsyncKeyState('F'))
	{
		lamp.position.y -= 0.05f;
		//where.MovePosition(DOWN);
	}
}

uint MainGLWindow::modelSetup(const ShapeData& data, glm::mat4 modelTransform, const char* vertexFileName, const char* fragmentFileName, const char* textureFileName, DrawType type)
{
	/*uint what = Renderer.createGeomtry(data.verts,data.indices,data.vertexBufferSize(),data.indexBufferSize(),data.numVerts,data.numIndices,Vertex::STRIDE,Vertex::POSITION_OFFSET,Vertex::COLOR_OFFSET,Vertex::NORMAL_OFFSET,Vertex::UV_OFFSET,type);
	uint pID = Renderer.createShader(vertexFileName,fragmentFileName);
	uint how = Renderer.createRenderInfo(shaderInfo[shaderInfo.size()-1],textureFileName);
	return Renderer.addRenderable(what,pID,how,modelTransform,true);*/

	return 0;
}

uint MainGLWindow::modelSetup(const ShapeData& data, glm::mat4 modelTransform, const char* vertexFileName, const char* fragmentFileName, DrawType type, const glm::vec4* color)
{
	//uint what = Renderer.createGeomtry(data.verts,data.indices,data.vertexBufferSize(),data.indexBufferSize(),data.numVerts,data.numIndices,Vertex::STRIDE,Vertex::POSITION_OFFSET,Vertex::COLOR_OFFSET,Vertex::NORMAL_OFFSET,Vertex::UV_OFFSET,type);
	//uint pID = Renderer.createShader(vertexFileName,fragmentFileName);
	//uint how = Renderer.createRenderInfo(debugShaderP);
	//return Renderer.addRenderable(what,pID,how,modelTransform,true);

	return 0;
}

void MainGLWindow::setupDebugShader(const glm::vec4* color)
{
	/*debugShaderR.Init("mvp");
	debugShaderR.addLayout(0,VectorDataType::position);
	debugShaderR.addLayout(1,VectorDataType::color);
	debugShaderR.addLayout(2,VectorDataType::normal);
	debugShaderR.addLayout(3,VectorDataType::uv);
	debugShaderR.addUniform(&colorR,"ambientColor",DataType::FLOAT4);

	debugShaderP.Init("mvp");
	debugShaderP.addLayout(0,VectorDataType::position);
	debugShaderP.addLayout(1,VectorDataType::color);
	debugShaderP.addLayout(2,VectorDataType::normal);
	debugShaderP.addLayout(3,VectorDataType::uv);
	debugShaderP.addUniform(&colorP,"ambientColor",DataType::FLOAT4);*/
}

void MainGLWindow::shaderLayoutSetup()
{
	//uint size = shaderInfo.size();
	//shaderInfo.push_back(ShaderLayoutBuilder());
	//modelMat3.push_back(glm::mat3());
	//modelMat4.push_back(glm::mat4());

	//colors.push_back(colorInfo());
	//colors[size].ambientColor = glm::vec3(0.2f,0.2f,0.2f);
	//colors[size].specularColor = glm::vec4(0.3f,0.3f,0.3f,1.0f);
	//colors[size].diffuseColor = glm::vec4(0.8f,0.8f,0.8f,1.0f);

	//shaderInfo[size].Init("mvp");
	//shaderInfo[size].addLayout(0,VectorDataType::position);
	//shaderInfo[size].addLayout(1,VectorDataType::color);
	//shaderInfo[size].addLayout(2,VectorDataType::normal);
	//shaderInfo[size].addLayout(3,VectorDataType::uv);
	//shaderInfo[size].addLayout(4,VectorDataType::tangent);
	//shaderInfo[size].addLayout(5,VectorDataType::bitangent);
	//shaderInfo[size].addUniform(&renderTex,"baseTex",DataType::INT1);
	//shaderInfo[size].addUniform(&colors[size].ambientColor,"ambientColor",DataType::FLOAT3);
	//shaderInfo[size].addUniform(&colors[size].diffuseColor,"diffuseColor",DataType::FLOAT4);
	//shaderInfo[size].addUniform(&colors[size].specularColor,"specularColor",DataType::FLOAT4);
	//shaderInfo[size].addUniform(&lamp.specularPower,"power",DataType::FLOAT1);
	//shaderInfo[size].addUniform(&lamp.position,"lightPosition",DataType::FLOAT3);
	//shaderInfo[size].addUniform(&lamp.colors.diffuseColor,"diffuseLight",DataType::FLOAT4);
	//shaderInfo[size].addUniform(&lamp.colors.ambientColor,"ambientLight",DataType::FLOAT3);
	//shaderInfo[size].addUniform(&MainCamera.getPosition(),"cameraPosition",DataType::FLOAT3);
	//shaderInfo[size].addUniform(&modelMat3[size],"modelTransformMat3",DataType::MAT3);
	//shaderInfo[size].addUniform(&modelMat4[size],"modelTransformMat4",DataType::MAT4);
	////shaderInfo[size].addUniform(&timer,"timer",DataType::FLOAT1);
}

void MainGLWindow::addParticle(const glm::vec3* position, const float* scale, const glm::vec4* color,const bool* showBoundBox)
{
	/*if((int)particles.size() > (P_WIDTH * P_HEIGHT) -1)
	{
		uint id = Renderer.addRenderable(particleGeometryID,particleShaderID,particleRenderInfoID,glm::mat4());
		particles.push_back(new particle(position,scale,showBoundBox,id));
	}
	else
		particles.push_back(new particle(position,scale,showBoundBox,1));*/
}

void MainGLWindow::updateParticles()
{
	//uint numVerts =0;
	//if(P_WIDTH > 0 && P_HEIGHT > 0)
	//{
	//	//get plane verts
	//	void* vertexData = NULL;
	//	unsigned short* indexData = NULL;
	//	numVerts = Renderer.getVertexBufferData(vertexData,geomteryInfoID);
	//	uint numIndicies = Renderer.getIndexBufferData(indexData,geomteryInfoID);
	//	glm::vec3* normals = new glm::vec3[numVerts];
	//	float* averageCount = new float[numVerts];
	//	//start the two buffers with fresh data
	//	for(uint i=0;i<numVerts;i++)
	//	{
	//		normals[i] = glm::vec3(0);
	//		averageCount[i] = 0;
	//	}
	//	//if we can use the plane otherwise just draw the lines
	//	if(particles.size() >= numVerts)
	//	{
	//		for(uint i=0;i<numVerts;i++)
	//		{
	//			reinterpret_cast<Vertex*>(vertexData)[i].position = *particles[i]->position;
	//		}

	//		for(uint i=0;i<numIndicies;i+=3)
	//		{
	//			glm::vec3 index0, index1, index2;
	//			index0 = reinterpret_cast<Vertex*>(vertexData)[indexData[i]].position;
	//			index1 = reinterpret_cast<Vertex*>(vertexData)[indexData[i+1]].position;
	//			index2 = reinterpret_cast<Vertex*>(vertexData)[indexData[i+2]].position;
	//			glm::vec3 normal = glm::normalize(glm::cross(index1 - index0,index2 - index0));
	//			normals[indexData[i]] += normal;
	//			normals[indexData[i+1]] += normal;
	//			normals[indexData[i+2]] += normal;
	//			averageCount[indexData[i]]++;
	//			averageCount[indexData[i+1]]++;
	//			averageCount[indexData[i+2]]++;
	//		}

	//		for(uint i=0;i<numVerts;i++)
	//		{
	//			reinterpret_cast<Vertex*>(vertexData)[i].normal = normals[i] / averageCount[i];
	//		}
	//		Renderer.updateVertexBufferData(vertexData,geomteryInfoID);
	//		delete [] normals;
	//		delete [] averageCount;
	//		delete[] vertexData;
	//	}
	//}	
	//uint size = particles.size();
	//for(int i=numVerts;i<size;i++)
	//{
	//	particle& p = *particles[i];
	//	Renderer.updateRenderablePosition(p.renderableID,glm::translate(*p.position) * glm::scale(glm::vec3(*p.scale * 0.25f)));
	//}

	//for(int i=0;i<nextLine;i++)
	//{
	//	line& l = *lines[i];
	//	//dot cross product rotation to point from one point to another
	//	glm::vec3 rot = *l.end - *l.start;
	//	//get length to scale by
	//	float length = glm::length(rot);
	//	float rotationAngle= acosf(glm::dot(glm::vec3(1.0f,0.0f,0.0f),rot / length));
	//	//convert to radians
	//	float convertedRotationAngle = rotationAngle * _180_PI;
	//	glm::vec3 cross = glm::cross(glm::vec3(1.0f,0.0f,0.0f),rot);
	//	if(cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f)
	//	{
	//		cross.x = 1.0f;
	//		convertedRotationAngle = 0;
	//	}
	//	glm::mat4 transform = glm::translate(*l.start) * glm::rotate(convertedRotationAngle,cross) * glm::scale(glm::vec3(length));

	//	Renderer.updateRenderablePosition(l.renderableID,transform);
	//}
}

void MainGLWindow::addLine(const glm::vec3* start, const glm::vec3* end, const glm::vec4* color)
{
	/*uint id = Renderer.addRenderable(lineGeometryID,lineShaderID,lineRenderInfoID,glm::mat4());
	lines[nextLine++] = new line(start,end,id);*/
}

void MainGLWindow::clearParticleList()
{
	/*uint size = particles.size();
	for(uint i=0;i<size;i++)
	{
		Renderer.removeRenderable(particles[i]->renderableID);
	}
	particles.clear();*/
}

void MainGLWindow::clearLineList()
{
	/*for(uint i=0;i<nextLine;i++)
	{
		Renderer.removeRenderable(lines[i]->renderableID);
		lines[i]->start = NULL;
		lines[i]->end = NULL;
	}
	nextLine = 0;*/
}

void MainGLWindow::writeOutModel()const
{
	/*ShapeData data = Renderer.getShapeData(geomteryInfoID);
	ShapeExporter exporter;
	exporter.writeShapeData(&data,1);*/
}

void MainGLWindow::removeParticle(const glm::vec3* position)
{
	/*uint size = particles.size();
	for(uint i=0;i<size;i++)
	{
		if(particles[i]->position == position)
		{
			Renderer.removeRenderable(particles[i]->renderableID);
			particles.erase(particles.begin() + i);
			return;
		}
	}*/
}
