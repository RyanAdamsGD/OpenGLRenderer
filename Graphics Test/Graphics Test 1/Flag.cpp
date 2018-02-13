#include "Flag.h"
#include "VerletParticle.h"
#include "ParticleLink.h"
#include "IParticleDrawer.h"
#include "QTDebugMenu.h"
#include "FastDelegate.h"
#include "Helper Functions.h"

#define P_WIDTH 15
#define P_HEIGHT 15

Flag::Flag(void)
	:particles(NULL),links(NULL),registry(),particlesMass(1.8f),restingLength(2.45f),stiffness(1.0f),
	width(P_WIDTH),height(P_HEIGHT),numOfSolves(15),newWidth(P_WIDTH),newHeight(P_HEIGHT),pinTop(true),drawer(NULL),
	pinCorners(false),fanStrength(0.0f),fanX(0),goLeft(true)
{
	particles = (VerletParticle**)malloc (sizeof(VerletParticle) * width * height);		
	links = (ParticleLink*)malloc (sizeof(ParticleLink) * (width-1) * (height-1));
	fanParticle = new VerletParticle(glm::vec3(-24,25.0f,-66.0f),2.1f);
	directionParticle = new VerletParticle(glm::vec3(-18.5f,25.0f,-60.0f),2.1f);
	createFlag();
}

void Flag::update(float dt)
{
	//////////Print Code//////////
	/*drawer->frameCount++;
	if(drawer->frameCount > 200)
		fanStrength = 1.25f;
	float dt = 1.0f/30.0f;
	numOfSolves = 100;*/
	/////////////////////////////

	glm::vec3 startPos = glm::vec3(-9.0f,36.0f,-41.0f);

	if(goLeft && fanX < -restingLength)
		goLeft = !goLeft;
	else if (fanX > restingLength)
		goLeft = !goLeft;

	if(goLeft)
	{
		fanX -= dt*2.0f;
		directionParticle->position.x -= dt*2.0f;
	}
	else
	{
		fanX += dt*2.0f;
		directionParticle->position.x += dt*2.0f;
	}

	for(uint i=0, w=0;w<width;w++)
	{
		for(uint h=0;h<height;h++,i++)
		{
			if(h> 5 && h < 16 || randFloat() > 0.5f)
			{
				glm::vec3 pos = particles[i]->position + startPos;

				glm::vec3 offset = pos-fanParticle->position;
				float len = glm::length(offset);
				float power = (fanStrength * (randFloat()-0.5f))/powf(len,2);
				glm::vec3 direction = pos - directionParticle->position - fanParticle->position;
				particles[i]->velocity +=  fanStrength * direction * powf(randFloat() - 0.5f,2.0f) * particles[i]->inverseMass * dt;
			}
			particles[i]->update(dt);
			if(particles[i]->position.z < startPos.z - 1.5f)
				particles[i]->position.z = startPos.z - 1.5f;
		}
	}
	for(uint j=0; j<numOfSolves;j++)
	{
		registry.solveLinks();
		if(pinCorners)
		{
			particles[0]->position = glm::vec3(0.0f,4.0f,0.0f);
			particles[(width-1) * height]->position = glm::vec3((width-1) * 0.5f * restingLength,4.0f,0.0f);
			particles[height-1]->position = glm::vec3(0.0f,4.0f,(height-1) * 0.5f * restingLength);
			particles[(width * height) -1]->position = glm::vec3((width-1) * 0.5f * restingLength,4.0f,(height-1) * 0.5f * restingLength);
		}
		else if(pinTop)
		{
			for(uint i=1;i<=width;i++)
			{
				particles[(width-i) * height]->position =startPos + glm::vec3((width-i) * 0.25f * restingLength,4.0f,0.0);
			}
		}
		else
		{
			particles[0]->position = glm::vec3(0.0f,4.0f,0.0f);
			particles[(width-1) * height]->position = glm::vec3((width-1) * 0.5f * restingLength,4.0f,0.0f);
		}
	}

}

void Flag::init(ParticleDrawer* drawer)
{
	this->drawer = drawer;
	drawer->useAdditiveLines = false;
	addParticles();
	addSliders();
}

void Flag::addParticles()
{
	for(uint i=0;i<width*height;i++)
	{
		drawer->addParticle(&particles[i]->position,&particles[i]->inverseMass);
	}
	for(uint i=0;i<(((width-1) * height) + ((height-1) * width));i++)
	{
		//drawer->addLine(&links[i].particle[0]->position,&links[i].particle[1]->position);
	}
	drawer->addParticle(&fanParticle->position,&fanParticle->inverseMass);
	drawer->addParticle(&directionParticle->position,&directionParticle->inverseMass);
}

void Flag::addSliders()
{
	QtDebugMenu.addWatchFloat("Frame Count", &drawer->frameCount);
	QtDebugMenu.addWatchFloat("FanX", &fanX);
	QtDebugMenu.addCheckBox("Pin Top",&pinTop);
	QtDebugMenu.addCheckBox("Pin Corners",&pinCorners);
	QtDebugMenu.addSlider("Stiffness",&stiffness,0.001f,1.0f);
	QtDebugMenu.addSlider("Fan Strength",&fanStrength,0.0f,10.0f);
	QtDebugMenu.addSlider("Number of Resolutions",&numOfSolves,1.0f,50.0f);
	QtDebugMenu.addSlider("New Width",&newWidth,1.0f,20.0f,true);
	QtDebugMenu.addSlider("New Height",&newHeight,1.0f,20.0f);
	QtDebugMenu.addSlider("Resting Length",&restingLength,0.0f,5.0f);
	QtDebugMenu.addButton("Rebuild Cloth",fastdelegate::MakeDelegate(this,&Flag::createFlag));
}

void Flag::createFlag()
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
	if(links != NULL)
	{
		if(drawer != NULL)
		{
			drawer->clearLineList();
		}
		free(links);
		links = (ParticleLink*)malloc (sizeof(ParticleLink) * (((width-1) * height) + ((height-1) * width)));
	}
	registry.clear();
	uint linkIndex =0;

	for(uint x=0;x<width;x++)
	{
		for(uint y=0;y<height;y++)
		{
			particles[x * height + y] = new VerletParticle(glm::vec3((x - (width * 0.5f)) * restingLength,4.0f,0.0f),particlesMass,glm::vec3(0,-9.8,0.0f));
			if(y>0)
			{
				links[linkIndex++] = ParticleLink(particles[(x * height + y)-1],particles[(x * height) + y],restingLength,stiffness);
				registry.add(particles[(x * height + y)-1],particles[(x * height + y)],&links[linkIndex-1]);
			}
			if(x>0)
			{
				links[linkIndex++] = ParticleLink(particles[(x * height + y)-height],particles[(x * height) + y],restingLength,stiffness);
				registry.add(particles[(x * height + y)-height],particles[(x * height + y)],&links[linkIndex-1]);
			}
		}
	}
	//readd the removed objects
	if(drawer != NULL)
	{
		addParticles();
	}
}