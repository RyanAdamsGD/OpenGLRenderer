#include "RenderManager.h"
#include "GL\glew.h"
#include "glm\gtx\transform.hpp"
#include "TransformInfo.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Texture.h"
#include "ShaderUniformManager.h"
#include "GeometryInfo.h"
#include "Renderable.h"
#include "Logger.h"

RenderManager RenderManager::instance;

RenderManager::RenderManager()
	:cameraPosition(0),ambientLight(0.3f),numberOfLights(0)
{

}

RenderManager::~RenderManager()
{
	uint size = renderables.size();
	for(uint i=0;i<size;i++)
	{
		if(renderables[i] != NULL)
		{
			delete renderables[i];
			renderables[i] = NULL;
		}
	}

	renderables.clear();
}

void RenderManager::setClearColor(const glm::vec4& color)
{
	clearColor = glm::clamp(color,glm::vec4(0.0f),glm::vec4(1.0f));
	glClearColor(clearColor.r,clearColor.g,clearColor.b,clearColor.a);
}

void RenderManager::draw()
{
	uint size = drawPhases.size();
	glm::vec2 viewPortStart;
	glm::vec2 viewPortEnd;
	for(uint i=0;i<size;i++)
	{
		DrawPhase& phase = drawPhases[i];
		glBindFramebuffer(GL_FRAMEBUFFER,phase.frameBuffer);
		Render_Manager.cameraPosition = phase.camera->getPosition();
		viewPortStart = phase.camera->getViewPortStart();
		viewPortEnd = viewPortStart + phase.camera->getViewPortSize();
		glViewport(viewPortStart.x,viewPortStart.y,viewPortEnd.x,viewPortEnd.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		phase.draw();
	}
}

void RenderManager::DrawPhase::draw()
{
	uint size = renderables.size();
	for(uint i =0;i<size;i++)
	{
		Renderable* renderable = Render_Manager.getRenderable(renderables[i]);
		const ShaderUniformManager* shaderUniformManager = renderable->getUniformManager();

		glUseProgram(shaderUniformManager->getProgramID());
		//set up uniform variables
		renderable->setMVP(camera->getProjection() * camera->getCameraView() * renderable->getTransformInfo()->getTransform());

		shaderUniformManager->activateUniforms();

		glBindVertexArray(renderable->getVAO());

		GeometryInfo* geometry = renderable->getGeometryInfo();

		if(geometry->getDrawType() == drawTriangle)
		{
			glDrawElements(GL_TRIANGLES,geometry->getNumIndicies(),GL_UNSIGNED_SHORT,BUFFER_OFFSET(geometry->getIndexBufferOffset()));
		}
		else
		{
			glDrawElements(GL_LINES,geometry->getNumIndicies(),GL_UNSIGNED_SHORT,BUFFER_OFFSET(geometry->getIndexBufferOffset()));
		}
	}
}

bool RenderManager::DrawPhase::removeRenderable(uint index)
{
	uint size = renderables.size();
	for(uint i=0;i<size;i++)
	{
		if(renderables[i] == index)
		{
			renderables.erase(renderables.begin() + i);
			return true;
		}
	}

	return false;
}

uint RenderManager::addCamera(const Camera* camera, uint frameBuffer, uint* renderableIDs, uint numOfRenderables)
{
	uint index = drawPhases.size();
	drawPhases.push_back(DrawPhase(camera,frameBuffer));

	DrawPhase& drawPhase = drawPhases[index];
	for(uint i=0;i<numOfRenderables;i++)
	{
		drawPhase.renderables.push_back(renderableIDs[i]);
	}

	return index;
}

uint RenderManager::addCamera(const Camera* camera, uint frameBuffer)
{
	uint index = drawPhases.size();
	drawPhases.push_back(DrawPhase(camera,frameBuffer));

	DrawPhase& drawPhase = drawPhases[index];
	uint size = renderables.size();
	for(uint i=0;i<size;i++)
	{
		drawPhase.renderables.push_back(i);
	}

	return index;
}

void RenderManager::removeRenderable(uint index, const Camera* camera)
{
#ifdef USE_LOGGER
	if(index > renderables.size())
		LOGW("The renderable index requested to be removed is out of range.");
#endif

	if(camera != NULL)
	{
		findDrawPhase(camera)->removeRenderable(index);
	}
	else
	{
		uint size = drawPhases.size();
		for(uint i=0;i<size;i++)
		{
			drawPhases[i].removeRenderable(index);
		}
		delete renderables[index];
		renderables[index] = NULL;
	}
}

RenderManager::DrawPhase* RenderManager::findDrawPhase(const Camera* camera)
{
	uint size = drawPhases.size();
	for(uint i=0;i<size;i++)
	{
		if(drawPhases[i].camera == camera)
			return &drawPhases[i];
	}

#ifdef USE_LOGGER
	LOGW("Camera not found in draw phases.");
#endif

	return NULL;
}

uint RenderManager::addRenderable(uint geometryManagerIndex, TransformInfo* transformInfo, ShaderUniformManager uniformManager,bool castsShadows, bool visible, bool depthTesting)
{
	uint index = renderables.size();
	renderables.push_back(new Renderable(geometryManagerIndex,transformInfo,uniformManager,castsShadows,visible,depthTesting));
	return index;
}

uint RenderManager::addRenderable(uint geometryManagerIndex, TransformInfo* transformInfo,bool castsShadows, bool visible, bool depthTesting)
{
	uint index = renderables.size();
	renderables.push_back(new Renderable(geometryManagerIndex,transformInfo,castsShadows,visible,depthTesting));
	return index;
}