#include "ParticleSystemFire.h"
#include "GL\glew.h"
#include <QtOpenGL\qglwidget>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <Qt\qimage.h>
#include "Helper Functions.h"
#include "Math Defines.h"
#include "LuaState.h"

ParticleSystemFire::ParticleSystemFire()
{
	LuaState luaState("../../External Dependencies/Engine/config.lua");
	POINTSIZE = luaState.get<float>("FirePointSize");
	LIFETIME = luaState.get<float>("FireLifeTime");
	SPEED = luaState.get<float>("FireSpeed");
}


void ParticleSystemFire::createShader()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	compileShader(vertexShaderID, "VertexShaderParticleSystem.txt");
	compileShader(fragShaderID,"FragShaderParticleSystem.txt");
	glAttachShader(programID,vertexShaderID);
	glAttachShader(programID,fragShaderID);

	glBindAttribLocation(programID,0, "VertexPosition");
	glBindAttribLocation(programID,1, "VertexVelocity");
	glBindAttribLocation(programID,2, "VertexStartTime");
	glBindAttribLocation(programID,3, "VertexInitialVelocity");

		const char * outputNames[] = { "Position", "Velocity",
	"StartTime" };
	glTransformFeedbackVaryings(programID, 3, outputNames,
	GL_SEPARATE_ATTRIBS);

	glLinkProgram(programID);
}


void ParticleSystemFire::fillBuffer()
{
	uint e =0;

	float *data = (float*) malloc(particleCount * 3 * sizeof(float));
	uint size = sizeof(float) * particleCount * 3;
	
	//Fill particle buffer
	for( int i = 0; i<particleCount * 3; i += 3 ) 
	{
		float r = randFloat();
		data[i] = r * 3.0f - 1.5f;
		data[i+1] = 0.0f;
		data[i+2] = 0.0f;
	}
	glGenBuffers(2, posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, size, data,GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, size, NULL,GL_STREAM_DRAW);

	free(data);
	data = (float*) malloc(particleCount * 3 * sizeof(float));
	// Fill the first velocity buffer with random velocities
	for( unsigned int i = 0; i<particleCount; i++ ) 
	{
		data[3*i] = 0.0f;
		data[3*i+1] = randFloat() /2;
		data[3*i+2] = 0.0f;
	}
	glGenBuffers(2,velBuf);
	glBindBuffer(GL_ARRAY_BUFFER,velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER,size,data,GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER,size,NULL,GL_STREAM_DRAW);
	glGenBuffers(1,&initVel);
	glBindBuffer(GL_ARRAY_BUFFER,initVel);
	glBufferData(GL_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);

	free(data);
	//Fill the start time buffer
	data = (float*) malloc(particleCount * sizeof(float));
	float time = 0.0f;
	for( uint i = 0; i<particleCount; i++ ) 
	{
		data[i] = time;
		time += SPEED;
	}
	glGenBuffers(2,startTime);
	glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
	glBufferData(GL_ARRAY_BUFFER,particleCount*sizeof(float),data,GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,startTime[1]);
	glBufferData(GL_ARRAY_BUFFER,particleCount*sizeof(float),NULL,GL_STREAM_DRAW);

	free(data);
}

void ParticleSystemFire::genVertexArrays()
{
	//glUseProgram(programID);

	//gen VAO
	//glGenVertexArrays(2,particleArray);

	//bind VAO
	//glBindVertexArray(particleArray[0]);
	if(!drawBuf)
	{
	//Enable Attrib
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//Bind & Attrib Buffers
	glBindBuffer(GL_ARRAY_BUFFER,posBuf[0]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
	glBindBuffer(GL_ARRAY_BUFFER,velBuf[0]);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,12,0);
	glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
	glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,4,0);
	glBindBuffer(GL_ARRAY_BUFFER,initVel);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,12,0);
	}
	else
	{
	//bind VAO
	//glBindVertexArray(particleArray[1]);

	//Enable Attrib
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	//Bind & Attrib Buffers
	glBindBuffer(GL_ARRAY_BUFFER,posBuf[1]);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
	glBindBuffer(GL_ARRAY_BUFFER,velBuf[1]);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,12,0);
	glBindBuffer(GL_ARRAY_BUFFER,startTime[1]);
	glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,4,0);
	glBindBuffer(GL_ARRAY_BUFFER,initVel);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,12,0);
	}
	//close off the VAO and VBO
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
}


void ParticleSystemFire::createFeedback()
{
	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);
	// Bind Buffers
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,1,velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,2,startTime[0]);


	// Bind Buffers
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,1,velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,2,startTime[1]);

}

void ParticleSystemFire::draw(const glm::mat4& projection,const glm::mat4& view,float dt)
{
	glPointSize(POINTSIZE);
	//uniforms
	uint uniformLocation = glGetUniformLocation(programID,"Time");
	glUniform1f(uniformLocation,runTime);

	uniformLocation = glGetUniformLocation(programID,"ParticleLifetime");
	glUniform1f(uniformLocation,LIFETIME);

	glm::mat4 xfm = projection * view * glm::translate(position);
	
	uniformLocation = glGetUniformLocation(programID,"MVP");
	glUniformMatrix4fv(uniformLocation,1,GL_FALSE,&xfm[0][0]);	
	
	uniformLocation = glGetUniformLocation(programID,"Accel");
	glm::vec3 acceleration(0.0f,0.1f,0.0f);
	glUniform3fv(uniformLocation,1,&acceleration[0]);
	
	uniformLocation = glGetUniformLocation(programID,"H");
	glUniform1f(uniformLocation,dt);

	uniformLocation = glGetUniformLocation(programID,"Tex");
	glUniform1i(uniformLocation,textureID);


	//Query Feedback updates
	GLuint query;
	glGenQueries(1, &query);
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

	/////////// Update pass ////////////////
	uint updateSub = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,"update");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);

	
	//// Disable rendering
	glEnable(GL_RASTERIZER_DISCARD);
	//// Bind the feedback object for the buffers to be drawn next
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1-drawBuf]);
	//// Draw points from input buffer with transform feedback
	glBeginTransformFeedback(GL_POINTS);
	
	//glBindVertexArray(particleArray[1-drawBuf]);
	genVertexArrays();
	
	glDrawArrays(GL_POINTS, 0, particleCount);
	glEndTransformFeedback();
	// Enable rendering
	glDisable(GL_RASTERIZER_DISCARD);

	//Print out Query Results
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	GLuint primWritten;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primWritten);
	printf("Primitives written: %d\n", primWritten);



	//////////// Render pass ///////////////
	uint renderSub = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,"render");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);

	//glClear( GL_COLOR_BUFFER_BIT );
	
	// Un-bind the feedback object.
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	//Draw the sprites from the feedback buffer
	//glBindVertexArray(particleArray[drawBuf]);
	genVertexArrays();
	glDrawArrays(GL_POINTS, 0, particleCount);
	// Swap buffers
	drawBuf = 1 - drawBuf;
}
