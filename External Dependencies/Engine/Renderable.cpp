#include "Renderable.h"
#include "RenderManager.h"
#include "Logger.h"
#include "TextureManager.h"
#include "ShaderUniformManager.h"
#include "ShaderManager.h"
#include "TransformInfo.h"


const char* Renderable::hasDiffuseMap = "hasDiffuseMap";
const char* Renderable::noDiffuseMap = "noDiffuseMap";
const char* Renderable::hasNormalMap = "hasNormalMap";
const char* Renderable::noNormalMap = "noNormalMap";
const char* Renderable::hasBumpMap = "hasBumpMap";
const char* Renderable::noBumpMap = "noBumpMap";
const char* Renderable::hasOcclusionMap = "hasOcclusionMap";
const char* Renderable::noOcclusionMap = "noOcclusionMap";

Renderable::Renderable(uint geometryManagerIndex, TransformInfo* transformInfo, ShaderUniformManager uniformManager, bool castsShadows, bool visible, bool depthTesting)
	:geometryManagerIndex(geometryManagerIndex),transformInfo(transformInfo),uniformManager(uniformManager),diffuseMap(UNBOUND_TEXTURE),normalMap(UNBOUND_TEXTURE),
	bumpMap(UNBOUND_TEXTURE),occlusionMap(UNBOUND_TEXTURE),castsShadows(castsShadows),visible(visible),depthTesting(depthTesting),
	diffuseMapSubroutine(noDiffuseMap),normalMapSubroutine(noNormalMap),bumpMapSubroutine(noBumpMap),occlusionMapSubroutine(noOcclusionMap)
{	
	addUniforms();
}

Renderable::Renderable(uint geometryManagerIndex, TransformInfo* transformInfo, bool castsShadows, bool visible, bool depthTesting)
	:geometryManagerIndex(geometryManagerIndex),transformInfo(transformInfo),uniformManager(Shader_Manager.getDefault()),diffuseMap(UNBOUND_TEXTURE),normalMap(UNBOUND_TEXTURE),
	bumpMap(UNBOUND_TEXTURE),occlusionMap(UNBOUND_TEXTURE),castsShadows(castsShadows),visible(visible),depthTesting(depthTesting),
	diffuseMapSubroutine(noDiffuseMap),normalMapSubroutine(noNormalMap),bumpMapSubroutine(noBumpMap),occlusionMapSubroutine(noOcclusionMap)
{	
	addUniforms();
}

Renderable::Renderable(const Renderable& rhs)
	:geometryManagerIndex(rhs.geometryManagerIndex),transformInfo(rhs.transformInfo),uniformManager(rhs.uniformManager),diffuseMap(rhs.diffuseMap),normalMap(rhs.normalMap),
	bumpMap(rhs.bumpMap),occlusionMap(rhs.occlusionMap),castsShadows(rhs.castsShadows),visible(rhs.visible),depthTesting(rhs.depthTesting),
	diffuseMapSubroutine(rhs.diffuseMapSubroutine),normalMapSubroutine(rhs.normalMapSubroutine),bumpMapSubroutine(rhs.bumpMapSubroutine),occlusionMapSubroutine(rhs.occlusionMapSubroutine)
{
	updateUniforms();
}

Renderable& Renderable::operator =(const Renderable& rhs)
{
	Renderable renderable(rhs);
	return renderable;
}

Renderable::~Renderable(void)
{
}

void Renderable::updateUniforms()
{
	//update the uniforms so that the pointers now point to the correct data
	uniformManager.updateUniform(&mvp,"mvp",MAT4);
	uniformManager.updateUniform(&transformInfo->getTransform(),"modelTransform",MAT4);
	uniformManager.updateUniform(&diffuseMap,"diffuseMap",TEXTURE);
	uniformManager.updateUniform(&normalMap,"normalMap",TEXTURE);
	uniformManager.updateUniform(&bumpMap,"bumpMap",TEXTURE);
	uniformManager.updateUniform(&occlusionMap,"occlusionMap",TEXTURE);
	uniformManager.updateUniform(&diffuseMapSubroutine,"GetDiffuse",SUBROUTINE);
	uniformManager.updateUniform(&normalMapSubroutine,"GetNormal",SUBROUTINE);
	uniformManager.updateUniform(&occlusionMapSubroutine,"GetOcclusion",SUBROUTINE);
}

void Renderable::addUniforms()
{
	//add the uniforms that are stored inside the renderable
	uniformManager.addUniform(&mvp,"mvp",MAT4);
	uniformManager.addUniform(&transformInfo->getTransform(),"modelTransform",MAT4);
	uniformManager.addUniform(&diffuseMap,"diffuseMap",TEXTURE);
	uniformManager.addUniform(&normalMap,"normalMap",TEXTURE);
	uniformManager.addUniform(&bumpMap,"bumpMap",TEXTURE);
	uniformManager.addUniform(&occlusionMap,"occlusionMap",TEXTURE);
	uniformManager.addUniform(&diffuseMapSubroutine,"GetDiffuse",SUBROUTINE);
	uniformManager.addUniform(&normalMapSubroutine,"GetNormal",SUBROUTINE);
	uniformManager.addUniform(&occlusionMapSubroutine,"GetOcclusion",SUBROUTINE);
	uniformManager.addUniform(&Render_Manager.getCameraPosition(),"cameraPosition",FLOAT3);
	uniformManager.addUniform(&Render_Manager.getAmbientLight(),"ambientLight",FLOAT3);
	uniformManager.addUniform(&Render_Manager.getNumLights(),"numberOfLights",INT1);
}

void Renderable::addDiffuseMap(uint textureManagerIndex)
{
	bool exists = Texture_Manager.getTotalNumberOfTextures() > textureManagerIndex;
	if(!exists)
	{
#ifdef USE_LOGGER
		LOGW("Texture does not exist.");
#endif
		textureManagerIndex = UNBOUND_TEXTURE;
		diffuseMapSubroutine = noDiffuseMap;
	}
	else
		diffuseMapSubroutine = hasDiffuseMap;

	diffuseMap = textureManagerIndex;
}

void Renderable::addNormalMap(uint textureManagerIndex)
{
	bool exists = Texture_Manager.getTotalNumberOfTextures() > textureManagerIndex;
	if(!exists)
	{
#ifdef USE_LOGGER
		LOGW("Texture does not exist.");
#endif
		textureManagerIndex = UNBOUND_TEXTURE;
		normalMapSubroutine = noNormalMap;
	}
	else
		normalMapSubroutine = hasNormalMap;

	normalMap = textureManagerIndex;
}

void Renderable::addBumpMap(uint textureManagerIndex)
{
	bool exists = Texture_Manager.getTotalNumberOfTextures() > textureManagerIndex;
	if(!exists)
	{
#ifdef USE_LOGGER
		LOGW("Texture does not exist.");
#endif
		textureManagerIndex = UNBOUND_TEXTURE;
		bumpMapSubroutine = noBumpMap;
	}
	else
		bumpMapSubroutine = hasBumpMap;

	bumpMap = textureManagerIndex;
}

void Renderable::addOcclusionMap(uint textureManagerIndex)
{
	bool exists = Texture_Manager.getTotalNumberOfTextures() > textureManagerIndex;
	if(!exists)
	{
#ifdef USE_LOGGER
		LOGW("Texture does not exist.");
#endif
		textureManagerIndex = UNBOUND_TEXTURE;
		occlusionMapSubroutine = noOcclusionMap;
	}
	else
		occlusionMapSubroutine = hasOcclusionMap;

	occlusionMap = textureManagerIndex;
}

void Renderable::removeDiffuseMap()
{
	diffuseMap = UNBOUND_TEXTURE;
	diffuseMapSubroutine = noDiffuseMap;
}

void Renderable::removeNormalMap()
{
	normalMap = UNBOUND_TEXTURE;
	normalMapSubroutine = noNormalMap;
}

void Renderable::removeBumpMap()
{
	bumpMap = UNBOUND_TEXTURE;
	bumpMapSubroutine = noBumpMap;
}

void Renderable::removeOcclusionMap()
{
	occlusionMap = UNBOUND_TEXTURE;
	occlusionMapSubroutine = noOcclusionMap;
}