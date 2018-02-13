#include "Hair.h"
#include "VerletParticle.h"
#include "ParticleLink.h"
#include "IParticleDrawer.h"
#include "QTDebugMenu.h"
#include "FastDelegate.h"
#include "Helper Functions.h"
#include "ParticleContact.h"
#include <fstream>

//20
#define NUM_OF_SOLVES 1
#define DRAW_KEY_HAIRS

//152 89

Hair::Hair(void)
	:restingLength(0.8f),keyHairCount(0),stiffness(0.9f),normHairCount(0),hairLength(7),centerOfHead(0.0f),drawer(NULL)
	,inverseMass(0.25),numColumns(12),numRows(19),headRadius(3.725f),browAngle(50.0f),keyHairPercent(1.0f),
	gridKeyHairs(),gridVelocities(),headXScale(0.76f)
{
	keyHairCount = uint(numColumns * keyHairPercent) * uint(numRows * keyHairPercent);
	normHairCount = numColumns * numRows;
	createHair(keyHairs,numColumns * keyHairPercent, numRows * keyHairPercent);
	//createHair(hairs,numColumns, numRows);
	gridKeyHairs = Grid<VerletParticle>(glm::vec3(-headRadius - (hairLength * restingLength * 2.0f)), glm::vec3(headRadius + (hairLength * restingLength * 2.0f)),
		numColumns * keyHairPercent,numRows * .75f * keyHairPercent,numRows * .75f * keyHairPercent);
	gridVelocities = Grid<glm::vec3>(glm::vec3(-headRadius - (hairLength * restingLength * 2.0f)), glm::vec3(headRadius + (hairLength * restingLength * 2.0f)),
		numColumns * keyHairPercent,numRows * .75f * keyHairPercent,numRows * .75f * keyHairPercent);

	for(uint i=0;i<gridVelocities.numOfCells;i++)
		gridVelocities.addItem(new glm::vec3(0.0f),i);
	for(uint i=0;i<keyHairCount;i++)
		for(uint j=0;j<keyHairs[i]->count;j++)
			gridKeyHairs.addItem(&keyHairs[i]->hairParticles[j],keyHairs[i]->hairParticles[j].position);
}

void Hair::update(float dt)
{
	////////Print Code//////////
	drawer->frameCount++;
	/*if(drawer->frameCount > 200)
		writeFile();
	float dt = 1.0f/30.0f;*/
	///////////////////////////

	//updateKeyHairs();
	//updateVelocityGrid();
	//updateHairsVelocity();

	uint size = hairs.size();
	for(uint i=0;i<size;i++)
	{
		//hairs[i]->update(dt);
	}

	size = keyHairs.size();
	for(uint i=0;i<size;i++)
	{
		keyHairs[i]->update(dt);
	}
	//apply force away from head
	float headRadiusSQ = headRadius * headRadius;
	float invHeadXScale = 1.0f / headXScale;
	uint strandCount;
	for(uint i=0;i<size;i++)
	{
		keyHairs[i]->moveAwayFromPoint(centerOfHead,headRadiusSQ, dt,invHeadXScale);
		strandCount = keyHairs[i]->count;
		if(drawer->frameCount > 50 && (int)(drawer->frameCount) % 70 < 30)
		for(uint j=0;j<strandCount;j+=2)
		{
			keyHairs[i]->hairParticles[j].velocity += glm::vec3((randFloat()- 0.5f) * 0.005f,-0.01f,-randFloat() * 0.06f);
		}
	}

}

void Hair::init(ParticleDrawer* drawer)
{
	this->drawer = drawer;
	drawer->useAdditiveLines = false;
	addParticles();
	addSliders();
}

void Hair::addParticles()
{
#ifndef DRAW_KEY_HAIRS
	uint size = hairs.size();
	for(uint i=0;i<size;i++)
	{
		for(uint j=0;j<hairs[i]->count;j++)
		{
			//drawer->addParticle(&hairs[i]->hairParticles[j].position,&inverseMass);
			if(j<hairs[i]->count-1)
				drawer->addLine(&hairs[i]->hairParticles[j].position,&hairs[i]->hairParticles[j+1].position);

		}
	}
#else
	uint size = keyHairs.size();
	for(uint i=0;i<size;i++)
	{
		for(uint j=0;j<keyHairs[i]->count;j++)
		{
			//drawer->addParticle(&keyHairs[i]->hairParticles[j].position,&inverseMass);
			if(j<keyHairs[i]->count-1)
				drawer->addLine(&keyHairs[i]->hairParticles[j].position,&keyHairs[i]->hairParticles[j+1].position);

		}
	}
#endif
}

void Hair::addSliders()
{
	QtDebugMenu.addWatchFloat("Frame Count", &drawer->frameCount);
	QtDebugMenu.addSlider("Resting Length",&restingLength,0.0f,5.0f);
}

void Hair::createHair(std::vector<HairStrand*>& hair, uint numColumn, uint numRow)
{
	uint hairCount = numColumn * numRow;
	for(uint i=0;i<hairCount;i++)
	{
		uint currentHairLength = hairLength;
		if(i/numColumn < numRow * 0.15f)
		{
			currentHairLength *= lerp<float>((float)i/(hairCount*0.15f),0.3f,1.0f);
			currentHairLength = clamp(currentHairLength,2,hairLength);
		}
		VerletParticle* particles = new VerletParticle[currentHairLength];
		ParticleLink* links = new ParticleLink[currentHairLength-1];
		
		for(uint j=0;j<currentHairLength;j++)
		{
			VerletParticle nextParticle(getPosition(i,numColumn,numRow) + glm::vec3(0.01f,0.0f,-0.0f),inverseMass,glm::vec3(0.0f,-0.28f,0.0f));
			//add acceleration to the non root particles
			if(j>0)
			{
				if(j == currentHairLength-1)
				{
					nextParticle.acceleration = glm::vec3(0.0f,-9.28f,0.0f);
					nextParticle.inverseMass = inverseMass;
				}
			}
			else
				nextParticle.inverseMass = 0.0f;

			particles[j] = nextParticle;
			if(j>0)
			{
				links[j-1] = ParticleLink(&particles[j],&particles[j-1],restingLength,stiffness);
			}
		}
		HairStrand* strand = new HairStrand(&particles[0],particles,links,currentHairLength);


		hair.push_back(strand);
	}
	//readd the removed objects
	if(drawer != NULL)
	{
		addParticles();
	}
}

glm::vec3 Hair::getPosition(uint index, uint numColumn, uint numRow)
{
	glm::vec4 position(0.0f,0.0f,0.0f,0.0f);
	float rotationAngleZ, rotationAngleX;
	float translationY = 0.0f;
	//rotate for curv of head
	if(numColumn == 1)
		rotationAngleZ = 90.0f;
	else
	{
		rotationAngleZ = 140.0f / (numColumn-1);
		float randAngle = (randFloat() - 0.5f) * rotationAngleZ;

		//move more towards the top of the head;
		int rowIndex = index%numColumn;
		rotationAngleZ *= rowIndex;
		if(rotationAngleZ > 70.0f)
			rotationAngleZ -= 140;

		if(rowIndex%3 == 0)
			rotationAngleZ *= 0.3f;

		rotationAngleZ += randAngle;

		if(index%numColumn > numColumn/2)
		{
			position.y += lerp<float>(float(index%numColumn)/(numColumn*0.5f),headRadius,headRadius * headXScale);
		}
		else
		{
			position.y += lerp<float>(float((index%numColumn) - numColumn*0.5f)/(numColumn*0.5f),headRadius * headXScale,headRadius);
		}
	}
	//rotate first 70% of hair for top of head
	//rotate and translate other 30% for neck
	if(numRow == 1)
		rotationAngleX = 90.0f + browAngle;
	else
	{
		int rowIndex = index/numColumn;
		if(rowIndex < numRow * 0.9f)
		{
			rotationAngleX = lerp(float(rowIndex)/(numRow * 0.7f), -browAngle,90.0f);
		}
		else
		{
			rotationAngleX = 90.0f;
			translationY = lerp(float(rowIndex-numRow * 0.7f)/(numRow * 0.3f),0.0f,headRadius * -0.5f);
		}

		float randAngle = (randFloat() - 0.5f) * rotationAngleX;
		if(rowIndex%3 == 0)
			rotationAngleX *= 0.1f;
		rotationAngleX += randAngle;
	}

	position = position * glm::rotate(rotationAngleZ,0.0f,0.0f,1.0f);
	position = position * glm::rotate(rotationAngleX,1.0f,0.0f,0.0f);
	position.y += translationY;

	return glm::vec3(position);
}

Hair::HairStrand::HairStrand(VerletParticle* root, VerletParticle* hairParticles, ParticleLink* links, int count)
	:root(root),hairParticles(hairParticles),count(count),links(links)
{
}

void Hair::HairStrand::update(float dt)
{
	for(uint j=0;j<NUM_OF_SOLVES;j++)
		for(uint i=0;i<count-1;i++)
		{
			links[i].solve();
		}

	for(uint i=1;i<count;i++)
	{
		hairParticles[i].update(dt);
	}
}

Hair::HairStrand::~HairStrand()
{
	if(hairParticles != NULL)
	{
		delete [] hairParticles;
		hairParticles = NULL;
	}

	if(links != NULL)
	{
		delete [] links;
		links = NULL;
	}
}

void Hair::HairStrand::moveAwayFromPoint(glm::vec3 point, float minDistanceSq, float dt, float headXScale)
{
	for(uint i=1;i<count;i++)
	{
		glm::vec3 vecBetween = hairParticles[i].position - point;
		vecBetween.x *= headXScale;
		float distSq = glm::dot(vecBetween,vecBetween);
		if(distSq < minDistanceSq)
		{
			if(distSq * 0.0f == 0.0f)
				hairParticles[i].velocity = vecBetween / sqrt(distSq) * (minDistanceSq - distSq) * dt * 20.0f;
			else
			{
				hairParticles[i].position = glm::vec3();
				hairParticles[i].velocity = glm::vec3();
			}
		}
	}
}


void Hair::writeFile()
{
	std::ofstream File(getFileName(),std::ios::out | std::ios::binary);
	File.clear();
	uint totalFileSize = 0;
	uint count = keyHairs.size();
	uint strandSize = (sizeof(glm::vec3) * 2);
	uint strandCount = 0;
	for(uint i=0;i<count;i++)
	{
		assert(keyHairs[i]->count > 1);
		strandCount += keyHairs[i]->count-1;
	}
	totalFileSize += strandSize * strandCount;
	//total fileSize and count of particles
	uint headerSize = sizeof(uint) + sizeof(uint);
	totalFileSize += headerSize;
	char* buffer = new char[totalFileSize];
	memcpy(buffer, &totalFileSize,sizeof(uint));
	uint totalCount = 0;
	for(uint i=0;i<count;i++)
	{
		for(uint j=0;j<keyHairs[i]->count-1;j++)
		{
			memcpy(&buffer[headerSize + totalCount * strandSize],&keyHairs[i]->links[j].particle[0]->position,sizeof(glm::vec3));
			memcpy(&buffer[headerSize + totalCount * strandSize + sizeof(glm::vec3)],&keyHairs[i]->links[j].particle[1]->position,sizeof(glm::vec3));
			totalCount++;
		}
	}
	memcpy(buffer+sizeof(uint),&totalCount,sizeof(uint));
	File.write(buffer,totalFileSize);
	File.close();
	delete [] buffer;
}

char* Hair::getFileName() const
{
	char* fileName = new char[14];
	int existingFileCount = 0;
	while(true)
	{
		sprintf(fileName,"hair%04d.rbn",existingFileCount);
		std::ifstream file(fileName,std::ios::binary);
		//look until we find an open file name
		if(!file || existingFileCount > 9999)
			break;
		existingFileCount++;
	}
	return fileName;
}

void Hair::updateVelocityGrid()
{
	uint count, throwAway;
	glm::vec3 velocity(0.0f);
	for(uint i=0;i<gridKeyHairs.numOfCells;i++)
	{
		VerletParticle** particles = gridKeyHairs.getCellData(i,count);
		if(count <= 0)
			continue;
		for(uint j=0;j<count;j++)
		{
			velocity += particles[j]->velocity;
		}
		glm::vec3* cellVelocity = *gridVelocities.getCellData(i,throwAway);
		*cellVelocity = velocity / (float)count;
		velocity = glm::vec3(0.0f);
	}
}

void Hair::updateKeyHairs()
{
	gridKeyHairs.clearCellData();
	uint size = keyHairs.size();
	for(uint i=0;i<size;i++)
	{
		HairStrand& strand = *keyHairs[i];
		for(uint j=0;j<strand.count;j++)
			gridKeyHairs.addItem(&strand.hairParticles[j],strand.hairParticles[j].position);
	}
}

void Hair::updateHairsVelocity()
{
	uint size = hairs.size();
	uint throwAway;
	for(uint i=0;i<size;i++)
	{
		HairStrand& strand = *hairs[i];
		for(uint j=0;j<strand.count;j++)
		{
			glm::vec3 **velocity = gridVelocities.getCellData(strand.hairParticles[j].position,throwAway);
			if(velocity != NULL)
				strand.hairParticles[j].velocity = **velocity;
		}
	}
}