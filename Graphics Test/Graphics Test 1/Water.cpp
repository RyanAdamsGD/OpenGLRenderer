#include "Water.h"
#include "VerletParticle.h"
#include "IParticleDrawer.h"
#include "QTDebugMenu.h"
#include "FastDelegate.h"
#include "Helper Functions.h"
#include "MyTypeDefs.h"
#include <iostream>
#include <fstream>

const float Water::waterEffect::travelSpeed = 1.5f;
const float Water::waterEffect::maxDuration = 10.0f;

Water::Water(void)
	:restingLength(0.25f),newWidth(50),newHeight(50),height(0),width(0),drawer(NULL),
	particles(NULL)
{
	particles = (VerletParticle**)malloc (sizeof(VerletParticle) * width * height);		
	createWater();
}

void Water::addRainDrop()
{
	float x = (randFloat() - 0.5f) * restingLength * width * 1.5;
	float z = (randFloat() - 0.5f) * restingLength * height * 1.5;
	float y = randFloat() * 5.0f + 30.0f;

	rain.push_back(new VerletParticle(glm::vec3(x,y,z),randFloat(),glm::vec3(0.0f,-0.98f,0.0f)));
	drawer->addParticle(&rain[rain.size()-1]->position,&rain[rain.size()-1]->inverseMass);
}

void Water::update(float dt)
{
	/*drawer->frameCount++;
	float dt = 1.0f/30.0f;
	if(drawer->frameCount > 200)
	{
		writeParticlePosition();
	}*/


	if(randFloat() < dt * 5.0f)
			addRainDrop();

	uint size = width * height;
	for(uint i=0;i<size;i++)
	{
		particles[i]->position.y = 0;
	}

	size = effects.size();
	for(uint i=0;i<size;i++)
	{
		if(effects[i].duration > waterEffect::maxDuration)
		{
			effects.erase(effects.begin() + i--);
			size--;
		}
		else
			updateWaterEffect(effects[i],dt);
	}

	size = rain.size();
	//adjusts when rain drops actually create waves
	float dropOff = 0.4f;
	for(uint i=0;i<size;i++)
	{
		rain[i]->update(dt);
		glm::vec3 position = rain[i]->position;
		if(position.y <= 0.0f)
		{
			if(fabs(position.x) < (width * restingLength * dropOff) && fabs(position.z) < (height * restingLength * dropOff))
				effects.push_back(waterEffect(rain[i]->position,rain[i]->inverseMass));

			drawer->removeParticle(&rain[i]->position);
			rain.erase(rain.begin() + i--);
			size--;
		}
	}
}

void Water::updateWaterEffect(waterEffect& effect, float dt)
{
	float maxDistance = effect.duration * waterEffect::travelSpeed;
	float minDistance = maxDistance - _2_PI * 2.0;
	uint size = width*height;
	bool rowHit = false;
	bool hittingRow = false;
	for(uint i=0;i<size;i++)
	{
		float distance = glm::length(particles[i]->position - effect.position);
		if(distance < maxDistance && distance > minDistance)
		{
			float currentDuration = maxDistance - distance;
			if(maxDistance > 0.0)
				particles[i]->position.y += 7.0f * (sin(currentDuration * 4.0f) / (16.0f + (currentDuration*currentDuration*currentDuration)))/(effect.duration * effect.duration + 0.25f);
		}
	}

	effect.duration += dt;
}
void Water::init(ParticleDrawer* drawer)
{
	this->drawer = drawer;
	drawer->frameCount = 0;
	drawer->useAdditiveLines = false;
	addParticles();
	addSliders();
}

void Water::addParticles()
{
	for(uint i=0;i<width*height;i++)
	{
		drawer->addParticle(&particles[i]->position,&particles[i]->inverseMass);
	}
}

void Water::addSliders()
{
	QtDebugMenu.addWatchFloat("Frame Count", &drawer->frameCount);
}

void Water::createWater()
{
	//clean up the particles and lines and reallocate memory
	if(particles != NULL)
	{
		width = newWidth;
		height = newHeight;
		if(drawer != NULL)
		{
			drawer->clearParticleList();
		}
		free(particles);
		particles = (VerletParticle**)malloc (sizeof(VerletParticle) * width * height);		
	}

	for(uint x=0;x<width;x++)
	{
		for(uint z=0;z<height;z++)
		{
			particles[x * height + z] = new VerletParticle(glm::vec3((x - (width * 0.5f)) * restingLength,0.0f,(z - (height * 0.5)) * restingLength),1.0,glm::vec3(0,-9.8,0.0f));
		}
	}
	//readd the removed objects
	if(drawer != NULL)
	{
		addParticles();
	}
}

void Water::writeParticlePosition()const
{
	std::ofstream File(getFileName(),std::ios::out | std::ios::binary);
	File.clear();
	uint totalFileSize = 0;
	uint count = rain.size();
	uint particleSize = (sizeof(float) + sizeof(glm::vec3));
	totalFileSize += particleSize * count;
	//total fileSize and count of particles
	uint headerSize = sizeof(uint) + sizeof(uint);
	totalFileSize += headerSize;
	char* buffer = new char[totalFileSize];
	memcpy(buffer, &totalFileSize,sizeof(uint));
	memcpy(buffer+sizeof(uint),&count,sizeof(uint));
	for(uint i=0;i<count;i++)
	{
		memcpy(&buffer[headerSize + i * particleSize],&rain[i]->inverseMass,sizeof(float));
		memcpy(&buffer[headerSize + i * particleSize + sizeof(float)],&rain[i]->position,sizeof(glm::vec3));
	}
	File.write(buffer,totalFileSize);
	File.close();
}

char* Water::getFileName()const 
{
	char* fileName = new char[14];
	int existingFileCount = 0;
	while(true)
	{
		sprintf(fileName,"rain%04d.rbn",existingFileCount);
		std::ifstream file(fileName,std::ios::binary);
		//look until we find an open file name
		if(!file || existingFileCount > 9999)
			break;
		existingFileCount++;
	}
	return fileName;
}