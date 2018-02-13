#version 400 

out vec4 FragColor;

in vec2 vertexUV;
in vec4 vertexColor;
in vec3 worldSpaceNormal;
in vec3 vertexWorldPosition;
in mat3 tangentToWorld;

uniform int numberOfLights;
uniform vec3 cameraPosition;
uniform vec3 ambientLight;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D bumpMap;
uniform sampler2D occlusionMap;

//for making an array
struct LightStruct
{
	vec3 position;
	vec3 diffuseColor;
	vec3 specularColor;
	float power;
};
//for querying the data layout
uniform LightUniform
{
	vec3 position;
	vec3 diffuseColor;
	vec3 specularColor;
	float power;
};

const int MAX_NUM_LIGHTS = 10;
layout(std140) uniform lightsUniformBlock
{
	LightStruct lights[MAX_NUM_LIGHTS];
};

struct TestColorStruct
{
	vec4 color;
};

layout (std140) uniform TestColorUniform
{
	vec4 color;
};

layout (std140) uniform TestColorsBlock
{
	TestColorStruct colors[2];
};

//Lighting Subroutine
subroutine vec4 LightingCalculations(vec3,vec4);
subroutine uniform LightingCalculations CalculateLighting;
//Normals Subroutine
subroutine vec3 CalculateNormal();
subroutine uniform CalculateNormal GetNormal;
//Diffuse Texture Subroutine
subroutine vec4 CalculateDiffuse();
subroutine uniform CalculateDiffuse GetDiffuse;
//Occlusion Subroutine
subroutine vec3 CalculateOcclusion();
subroutine uniform CalculateOcclusion GetOcclusion;


subroutine (CalculateOcclusion)
vec3 noOcclusionMap()
{
	return ambientLight;
}

subroutine (CalculateOcclusion)
vec3 hasOcclusionMap()
{
	vec4 occTex = texture(occlusionMap,vertexUV);
	return (ambientLight * vec3(occTex));
}

subroutine (CalculateDiffuse)
vec4 noDiffuseMap()
{
	return vertexColor;
}

subroutine (CalculateDiffuse)
vec4 hasDiffuseMap()
{
	return texture(diffuseMap,vertexUV);
}

subroutine (CalculateNormal)
vec3 noNormalMap()
{
	return worldSpaceNormal;
}

subroutine (CalculateNormal)
vec3 hasNormalMap()
{
	vec3 mapNormal = normalize(texture2D( normalMap, vertexUV ).rgb*2.0 - 1.0);
	return normalize(tangentToWorld * mapNormal);
}

vec4 calculateLighting(vec3 normal, vec4 materialColor)
{
	uint numLights = numberOfLights;
	if(numberOfLights > MAX_NUM_LIGHTS)
		numLights = MAX_NUM_LIGHTS;

	vec4 diffuseColor;
	vec3 specularColor;
	vec3 lightVector;
	float diffuseBrightness;
	vec4 diffuseLight;
	vec3 lightReflectedVector;
	vec3 cameraVector;
	float specularBrightness;

	for(int i=0;i<numLights;i++)
	{
		//Diffuse
		//dot of normal and light
		lightVector = normalize(lights[i].position - vertexWorldPosition);
		diffuseBrightness = dot(normal, lightVector);
		diffuseBrightness = clamp(diffuseBrightness,0,1);
		diffuseLight = vec4(diffuseBrightness * diffuseLight.x,diffuseBrightness * diffuseLight.y,diffuseBrightness * diffuseLight.z,0);
		diffuseColor += diffuseLight * vec4(lights[i].diffuseColor,0);

		//Specular
		//dot product of the reflection of the light around the normal and the eye position
		lightReflectedVector = reflect(-lightVector,normal);
		cameraVector = normalize(cameraPosition-vertexWorldPosition);
		specularBrightness = dot(cameraVector,lightReflectedVector);
		specularBrightness = clamp(specularBrightness,0,1);
		specularBrightness = pow(specularBrightness,lights[i].power);
		specularColor += (specularBrightness * lights[i].specularColor);
	}
	//dif * mat + spec + amb * mat
	//alpha added in via ambient
	return ((diffuseColor * materialColor) + vec4(specularColor,0) + (vec4(GetOcclusion(),1) * materialColor));
}

subroutine (LightingCalculations)
vec4 noShadows(vec3 normal, vec4 materialColor)
{
	return calculateLighting(normal,materialColor);
}

subroutine (LightingCalculations)
vec4 hasShadows(vec3 normal, vec4 materialColor)
{
	return calculateLighting(normal,materialColor);
}

void main()
{
	vec3 normal = GetNormal();
	vec4 color = GetDiffuse();
	if(numberOfLights > 0)
		color = calculateLighting(normal,color);

	FragColor = color;
}