#include "RayTraceDemo.h"
#include "MyTypeDefs.h"
#include "IParticleDrawer.h"

RayTraceDemo::RayTraceDemo(void)
	:gridSize(10),sphereRadius(60.0f),spherePostion(0.0f,0.0f,-15.0f)
{
}


RayTraceDemo::~RayTraceDemo(void)
{
}

void RayTraceDemo::update(float dt)
{

}

void RayTraceDemo::init(ParticleDrawer* drawer)
{
	this->drawer = drawer;
	buildGrid();
	addGeometry();
}

void RayTraceDemo::buildGrid()
{
	glm::vec3 startingPoint(gridSize*0.5f,gridSize*0.5f,0.0f);
	glm::vec3 endPoint(-gridSize*0.5f,gridSize*0.5f,0.0f);
	glm::vec3 movement(0.0f,1.0f,0.0);
	for(uint i=0;i<gridSize+1;i++)
	{
		gridPoints.push_back(startingPoint + (movement * (float)i));
		gridPoints.push_back(endPoint + (movement * (float)i));
		for(uint j=0;j<gridSize;j++)
		{
			if(i>0)
			{
				glm::vec3 origin(startingPoint + (movement * (float)i) + glm::vec3(0.5f,-0.5f,0.0f) + glm::vec3(1.0f,0.0f,0.0f) * (float)j);
				rayPoints.push_back(origin);
				rayPoints.push_back(origin + glm::vec3(0.0f,0.0f,-sphereRadius * .30f));
			}
		}
	}

	startingPoint = glm::vec3(gridSize*0.5f,gridSize*0.5f,0.0f);
	endPoint = glm::vec3(gridSize*0.5f,gridSize*1.5f,0.0f);
	movement = glm::vec3(1.0f,0.0f,0.0);
	for(uint i=0;i<gridSize+1;i++)
	{
		gridPoints.push_back(startingPoint + (movement * (float)i));
		gridPoints.push_back(endPoint + (movement * (float)i));
	}


}

void RayTraceDemo::addGeometry()
{
	uint size = gridPoints.size();
	for(uint i=0;i<size;i+=2)
		drawer->addLine(&gridPoints[i],&gridPoints[i+1]);

	size = rayPoints.size();
	for(uint i=0;i<size;i+=2)
		drawer->addLine(&rayPoints[i],&rayPoints[i+1]);

	drawer->addParticle(&spherePostion,&sphereRadius);
}