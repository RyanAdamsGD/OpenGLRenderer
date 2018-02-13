#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include <vector>
#include "glm\gtx\transform.hpp"
class Renderable;
class TransformInfo;
class ShaderUniformManager;
class Camera;

class ENGINE_SHARED RenderManager
{
	struct DrawPhase
	{
		uint frameBuffer;
		const Camera* camera;
		std::vector<uint> renderables;
		DrawPhase(const Camera* camera, uint frameBuffer)
			:camera(camera),frameBuffer(frameBuffer){}

		bool removeRenderable(uint index);
		void draw();
	};

	std::vector<Renderable*> renderables;

	std::vector<DrawPhase> drawPhases;
	static RenderManager instance;

	DrawPhase* findDrawPhase(const Camera* camera);

	//allow renderable to add the necessary uniforms
	glm::vec3 cameraPosition;
	glm::vec4 clearColor;
	//TODO: Move ambient light some where more appropriate
	//and update number of lights when appropriate
	glm::vec3 ambientLight;
	int numberOfLights;
	const char* shadowsSubroutine;
	static const char* noShadows;
	static const char* hasShadows;
	

	RenderManager();
	RenderManager(const RenderManager&);
	RenderManager& operator =(const RenderManager&);
public:
	static RenderManager& getInstance() { return instance; }
	~RenderManager();

	void draw();
	uint addCamera(const Camera* camera, uint frameBuffer, uint* renderableIDs, uint numOfRenderables);
	uint addCamera(const Camera* camera, uint frameBuffer = 0);
	void removeRenderable(uint index, const Camera* camera = 0);
	
	void setClearColor(const glm::vec4& color);

	uint addRenderable(uint geometryManagerIndex,TransformInfo* transformInfo, ShaderUniformManager uniformManager,bool castsShadows = true, bool visible = true, bool depthTesting = true);
	uint addRenderable(uint geometryManagerIndex,TransformInfo* transformInfo,bool castsShadows = true, bool visible = true, bool depthTesting = true);
	Renderable* getRenderable(uint index) { return renderables[index]; }

	const glm::vec3& getCameraPosition() { return cameraPosition; }
	const glm::vec3& getAmbientLight() { return ambientLight; }
	const int& getNumLights() { return numberOfLights; }
};

#define Render_Manager RenderManager::getInstance()

#endif