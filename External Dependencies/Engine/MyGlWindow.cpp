#include "MyGlWindow.h"
#include <iostream>
#include <Qt\qfile.h>
#include <Qt\qtextstream.h>
#include <Qt\qdebug.h>
#include <string>
#include "ShapeGenerator.h"

float MyGlWindow::ScreenWidth=600;
float MyGlWindow::ScreenHeight=550;
glm::mat4 MyGlWindow::projection;

void MyGlWindow::initializeGL()
{
	glewInit();
	glClearColor(0,0,0,1);
	resize(ScreenWidth,ScreenHeight);
	
	glEnable(GL_DEPTH_TEST);
	createShader();
	lockMouse = true;
	MainCamera.CameraInit();
	arrow.Init(Neumont::ShapeGenerator::makeArrow());
}

void MyGlWindow::keyPressEvent(QKeyEvent* key)
{
	moveCamera(key);
	if(key->key() == Qt::Key_Alt)
	{
		lockMouse = false;
	}
	else
	{
		lockMouse = true;
	}
}

void MyGlWindow::mouseMoveEvent(QMouseEvent* mouse)
{
	if(lockMouse)
	{
		rotateCamera(mouse);
	}
}

void MyGlWindow::update()
{
	
	updateGL();
}

void MyGlWindow::createShader()
{
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	compileShader(vertexShaderID, "MyVertexShader.txt");
	compileShader(fragShaderID,"MyFragmentShader.txt");

	glAttachShader(programID,vertexShaderID);
	glAttachShader(programID,fragShaderID);
	glLinkProgram(programID);
	glUseProgram(programID);
}

void MyGlWindow::compileShader(GLuint shaderID, const char* fileName)
{
	QFile input(fileName);
	if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug()<< "File failed to open: "<< fileName;
		//exit(1);
	}
	QTextStream stream(&input);
	QString shaderCodeText = stream.readAll();
	input.close();
	//char * shaderSource = new char[shaderCodeText.length()];
	//memcpy(shaderSource,&shaderCodeText,sizeof(shaderSource));
	std::string temp = shaderCodeText.toStdString();
	const char *shaderSource = temp.c_str();

	const char* buf[1];
	buf[0] = shaderSource;
	glShaderSource(shaderID,1,buf,NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID,GL_COMPILE_STATUS,&status);
	if(status != GL_TRUE)
	{
		//TODO:
		//Log this
		GLint infoLogLength;
		glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
		char* buffer = new char[infoLogLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID,infoLogLength,&bitBucket,buffer);
		std::cout<<buffer;
		delete[] buffer;
		//exit(1);
	}
}

void MyGlWindow::shutdown()
{
}

void MyGlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	arrow.Draw(projection,MainCamera.cameraView,programID);

}

void MyGlWindow::resizeEvent(QResizeEvent*)
{
	ScreenWidth=float(width());
	ScreenHeight=float(height());

			//field of view matrix
	aspect = ScreenWidth/ScreenHeight;
	viewAngle = 60; //45 degrees
	zn = 1.0f; //forward view plane
	zf = 1000.0f; //rear view plane
	projection = glm::perspective(viewAngle,aspect,zn,zf);

	glViewport(0,0,width(),height());
}


void MyGlWindow::moveCamera(QKeyEvent* key)
{
	
	if(key->key() == Qt::Key_A)
	{
		MainCamera.MovePosition(LEFT);
	}
	if(key->key() == Qt::Key_D)
	{
		MainCamera.MovePosition(RIGHT);
	}
	if(key->key() == Qt::Key_S)
	{
		MainCamera.MovePosition(BACKWARD);
	}
	if(key->key() == Qt::Key_W)
	{
		MainCamera.MovePosition(FORWARD);
	}
	if(key->key() == Qt::Key_R)
	{
		MainCamera.MovePosition(UP);
	}
	if(key->key() == Qt::Key_F)
	{
		MainCamera.MovePosition(DOWN);
	}
	update();
}

void MyGlWindow::rotateCamera(QMouseEvent* mouse)
{
	QPointF cursorPos = mouse->posF();
	if(cursorPos.x() < prevMousePos.x())
	{		
		MainCamera.RotateCamera(LEFT);
	}
	if(cursorPos.x() > prevMousePos.x())
	{		
		MainCamera.RotateCamera(RIGHT);
	}
	if(cursorPos.y() > prevMousePos.y())
	{		
		MainCamera.RotateCamera(DOWN);
	}
	if(cursorPos.y() < prevMousePos.y())
	{		
		MainCamera.RotateCamera(UP);
	}
	//TODO:
	//Get this Working
	//QCursor::setPos(ScreenWidth/2,ScreenHeight/2);
	//code works, using those coords to rotate isn't

	prevMousePos = cursorPos;


	update();
}