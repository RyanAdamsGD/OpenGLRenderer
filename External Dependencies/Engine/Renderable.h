#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "ExportHeader.h"
#include "MyTypeDefs.h"
#include <vector>
#include "TextureManager.h"
#include "GeometryManager.h"
#include "glm\gtx\transform.hpp"
#include "ShaderUniformManager.h"
class TransformInfo;
class Texture;

class ENGINE_SHARED Renderable
{
	glm::mat4 mvp;
	uint geometryManagerIndex;
	TransformInfo *transformInfo;
	ShaderUniformManager uniformManager;
	bool depthTesting;
	bool visible;
	bool castsShadows;
	uint diffuseMap;
	uint normalMap;
	uint bumpMap;
	uint occlusionMap;
	const char* diffuseMapSubroutine;
	const char* normalMapSubroutine;
	const char* occlusionMapSubroutine;
	const char* bumpMapSubroutine;

	static const char* hasDiffuseMap;
	static const char* noDiffuseMap;
	static const char* hasNormalMap;
	static const char* noNormalMap;
	static const char* hasBumpMap;
	static const char* noBumpMap;
	static const char* hasOcclusionMap;
	static const char* noOcclusionMap;

	void addUniforms();
	void updateUniforms();
public:
	Renderable(uint geometryManagerIndex, TransformInfo* transformInfo, ShaderUniformManager uniformManager,bool castsShadows, bool visible, bool depthTesting);
	Renderable(uint geometryManagerIndex, TransformInfo* transformInfo,bool castsShadows, bool visible, bool depthTesting);
	~Renderable(void);
	Renderable(const Renderable& rhs);
	Renderable& operator =(const Renderable& rhs);

	void draw();

	void addDiffuseMap(uint textureManagerIndex);
	void addNormalMap(uint textureManagerIndex);
	void addBumpMap(uint textureManagerIndex);
	void addOcclusionMap(uint textureManagerIndex);

	void removeDiffuseMap();
	void removeNormalMap();
	void removeBumpMap();
	void removeOcclusionMap();

	TransformInfo* getTransformInfo()const { return transformInfo; }
	const ShaderUniformManager* getUniformManager()const { return &uniformManager; }
	GeometryInfo* getGeometryInfo()const { return Geometry_Manager.getGeometry(geometryManagerIndex); }
	uint getVAO()const { return Geometry_Manager.getVAO(geometryManagerIndex); }

	Texture* getDiffuseMap()const { return Texture_Manager.getTexture(diffuseMap); }
	Texture* getNormalMap()const { return Texture_Manager.getTexture(normalMap); }
	Texture* getBumpMap()const { return Texture_Manager.getTexture(bumpMap); }
	Texture* getOcclusionMap()const { return Texture_Manager.getTexture(occlusionMap); }

	bool getDepthTesting()const { return depthTesting; }
	bool getIsVisible()const { return visible; }
	bool getCastsShadows()const { return castsShadows; }

	void setDepthTesting(bool value) { depthTesting = value; }
	void setIsVisible(bool value) { visible = value; }
	void setCastsShadows(bool value) { castsShadows = value; }
	void setMVP(const glm::mat4 value) { mvp = value; }
};

#endif