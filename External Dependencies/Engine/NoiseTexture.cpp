#include "NoiseTexture.h"
#include <GL\glew.h>
#include "Helper Functions.h"

NoiseTexture::NoiseTexture(uint width, uint height, float gain, float lacunarity, float numOctaves)
	:Texture(0,width,height),linearNoise(gain,lacunarity,numOctaves)
{
	float *data = new float[width * height * 4];
	float xRange = 10.0;
	float yRange = 10.0;
	float xFactor = xRange / width;
	float yFactor = yRange / height;

	for( int i = 0; i < width; i++ ) {
		for( int j = 0 ; j < height; j++ ) {
			float x = xFactor * i;
			float y = yFactor * j;
			float z = 0.0;
			float val = linearNoise.valueFBM(glm::vec3(x,y,z)) * 0.5f;
			clamp(val,0.0f,1.0f);

			data[(i * height + j)*4] = val;
			data[(i * height + j)*4+1] = val;
			data[(i * height + j)*4+2] = val;
			data[(i * height + j)*4+3] = 1.0;
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
		GL_FLOAT,data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	delete [] data;
}

NoiseTexture::~NoiseTexture(void)
{
}


void NoiseTexture::bindTexture()const
{
	glBindTexture(GL_TEXTURE_2D,textureID);
}
