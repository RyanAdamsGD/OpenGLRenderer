#include "ParticleSystemFountain.h"
#include "GL\glew.h"
#include <QtOpenGL\qglwidget>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <Qt\qimage.h>
#include "Math Defines.h"
#include "Helper Functions.h"
#include "LuaState.h"

ParticleSystemFountain::ParticleSystemFountain()
{
	LuaState luaState("../../External Dependencies/Engine/config.lua");
	POINTSIZE = luaState.get<float>("FountainPointSize");
	LIFETIME = luaState.get<float>("FountainLifeTime");
	SPEED = luaState.get<float>("FountainSpeed");
}


void ParticleSystemFountain::createShader()
{

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	compileShader(vertexShaderID, "vertexParticleShader.txt");
	compileShader(fragShaderID,"FragParticleShader.txt");
	glAttachShader(programID,vertexShaderID);
	glAttachShader(programID,fragShaderID);

	glBindAttribLocation(programID,0, "VertexInitVel");
	glBindAttribLocation(programID,1, "VertexStartTime");

	const char* outputNames[] = {"StartTime"};
	glTransformFeedbackVaryings(programID,1,outputNames,GL_INTERLEAVED_ATTRIBS);

	uint i = glGetError();
	glLinkProgram(programID);
}

void ParticleSystemFountain::fillBuffer()
{
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[particleCount * 3];
	for( GLuint i = 0; i<particleCount; i++ ) 
	{
		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)PI / 10.0f, randFloat());
		phi = glm::mix(0.0f, (float)(2 * PI), randFloat());
		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);
		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(.5f,1.0f,randFloat());
		v = v * velocity;
		data[3*i] = v.x;
		data[3*i+1] = v.y;
		data[3*i+2] = v.z;
	}
	glGenBuffers(1,&initVel);
	glBindBuffer(GL_ARRAY_BUFFER,initVel);
	glBufferData(GL_ARRAY_BUFFER,particleCount * 3 * sizeof(float),data,GL_STATIC_DRAW);

	free(data);
	data = (float*) malloc(particleCount * sizeof(float));
	float time = 0.0f, rate = 0.00075f;
	for( unsigned int i = 0; i<particleCount; i++ ) 
	{
		data[i] = time;
		time += rate;
	}
	glGenBuffers(2,startTime);
	glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
	glBufferData(GL_ARRAY_BUFFER,particleCount * sizeof(float),data,GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,startTime[1]);
	glBufferData(GL_ARRAY_BUFFER,particleCount * sizeof(float),data,GL_STREAM_DRAW);
	free(data);
}

void ParticleSystemFountain::genVertexArrays()
{
	////////Original Code///////

	//glUseProgram(programID);
	////gen VAO
	//glGenVertexArrays(2,particleArray);

	////bind VAO
	//glBindVertexArray(particleArray[0]);

	////Enable Attrib
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	////Bind & Attrib Buffers
	//glBindBuffer(GL_ARRAY_BUFFER,initVel);
	//glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
	//glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
	//glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,4,0);
	//

	////bind VAO
	//glBindVertexArray(particleArray[1]);

	////Enable Attrib
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	////Bind & Attrib Buffers
	//glBindBuffer(GL_ARRAY_BUFFER,initVel);
	//glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
	//glBindBuffer(GL_ARRAY_BUFFER,startTime[1]);
	//glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,4,0);
	//

	////close off the VAO and VBO
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER,0);













	////////Work Around////////
	if(!drawBuf)
	{
		//Enable Attrib
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//Bind & Attrib Buffers
		glBindBuffer(GL_ARRAY_BUFFER,initVel);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
		glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
		glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,4,0);
	}
	else
	{

		//Enable Attrib
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//Bind & Attrib Buffers
		glBindBuffer(GL_ARRAY_BUFFER,initVel);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0);
		glBindBuffer(GL_ARRAY_BUFFER,startTime[1]);
		glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,4,0);
	}
}

void ParticleSystemFountain::createFeedback()
{
	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);
	// Bind Buffers
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,startTime[0]);

	// Bind Buffers
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,startTime[1]);
}

void ParticleSystemFountain::draw(const glm::mat4& projection,const glm::mat4& view,float dt)
{
	//Setup uniforms
	glPointSize(POINTSIZE);

	uint uniformLocation = glGetUniformLocation(programID,"Time");
	glUniform1fv(uniformLocation,1,&runTime);

	uniformLocation = glGetUniformLocation(programID,"ParticleLifetime");
	float l = 4.0f;
	glUniform1fv(uniformLocation,1,&l);

	glm::mat4 xfm = projection * view * glm::translate(position);
	
	uniformLocation = glGetUniformLocation(programID,"MVP");
	glUniformMatrix4fv(uniformLocation,1,GL_FALSE,&xfm[0][0]);

	glm::vec3 grav(0.0f,-0.12f,0.0f);
	uniformLocation = glGetUniformLocation(programID,"Gravity");
	glUniform3fv(uniformLocation,1,&grav[0]);

	//Query Feedback updates
	GLuint query;
	glGenQueries(1, &query);
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

	/////////// Update pass ////////////////
	uint updateSub = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,"update");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);

	// Disable rendering
	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,feedback[1-drawBuf]);
	glBeginTransformFeedback(GL_POINTS);

	//Want to get this line of code to work
	//glBindVertexArray(particleArray[drawBuf]);
	//current work around is this line
	genVertexArrays();

	glDrawArrays(GL_POINTS, 0, particleCount);
	uint i = glGetError();
	glEndTransformFeedback();
	// Enable Rendering
	glDisable(GL_RASTERIZER_DISCARD);

	
	//Print out Query Results
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	GLuint primWritten;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primWritten);
	printf("Primitives written: %d\n", primWritten);
	
	//////////// Render pass ///////////////
	uint renderSub = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,"render");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);

	//unbind transform feedback
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,0);

	glDrawArrays(GL_POINTS, 0, particleCount);
	
	//swap VAO's
	drawBuf=1-drawBuf;
}
