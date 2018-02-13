#version 400 

layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
layout (location=2) in vec3 normal;
layout (location=3) in vec2 UV;
layout (location=4) in vec3 tangent;
layout (location=5) in vec3 bitangent;

out vec4 vertexColor;
out vec2 vertexUV;
out vec3 worldSpaceNormal;
out vec3 vertexWorldPosition;
//out vec3 shadowCoord;
out mat3 tangentToWorld;

uniform mat4 mvp;
uniform mat4 modelTransform;
uniform mat4 depthBiasMVP;

void main()
{
	vertexColor = color;
	vertexUV = UV;
	mat3 modelTransformMat3 = mat3(modelTransform);
	worldSpaceNormal = modelTransformMat3 * normal;
	tangentToWorld = modelTransformMat3 * mat3(tangent,bitangent,normal);
	vertexWorldPosition = vec3(modelTransform * vec4(position,1));

	gl_Position = mvp * vec4(position,1);
}