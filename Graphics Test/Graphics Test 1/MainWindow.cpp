#include "MainWindow.h"
#include <QtGui\qvboxlayout>
#include <QtGui\qhboxlayout>
#include <QtGui\qcheckbox>
#include <QtGui\qlabel>
#include <QtGui\qmenubar.h>
#include <QtGui\qdockwidget.h>
#include "MainGLWindow.h"
#include "Slider.h"
#include <Qt\qdebug.h>
#include "QTDebugMenu.h"
#include "QtPaintWindow.h"
#include "WhatADrag.h"
#include "AutoTimer.h"
#include "MyTypeDefs.h"
#include "LetsBounce.h"


MainWindow::MainWindow(IUpdatable* program, bool threeDee)
	:use3d(threeDee),dt(TARGETFRAMERATE),fps(0),createDelayedProgramForThreeD(false),program(program),autoStep(true)
{
	setMinimumSize(1000,800);

	if(use3d)
	{	
		setCentralWidget(GLWindow = new MainGLWindow());
		createDelayedProgramForThreeD = true;
	}
	else
	{
		setCentralWidget(QtWindow = new QtPaintWindow(this));
		program->init(QtWindow);
	}

	createDockWidgets();
	createToolBar();
	createMenus();
	

	this->startTimer(1);
	

	QtDebugMenu.addCheckBox("AutoStep", &autoStep);
	QtDebugMenu.addButton("Step",fastdelegate::MakeDelegate(this,&MainWindow::step));
	QtDebugMenu.addWatchFloat("DT: ", &dt);
	autoStep = false;
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction("New Scene",this,SLOT(newScene()));
	fileMenu->addAction("Open Scene",this,SLOT(openScene()));
	fileMenu->addAction("Save Scene",this,SLOT(saveScene()));
	
	createMenu = menuBar()->addMenu("&Create");
	createMenu->addAction("Create Light",this,SLOT(createLight()));
}

void MainWindow::createDockWidgets()
{
	QtDebugMenu.Init();
	QDockWidget* debugWidget = new QDockWidget("Debug Menu",this);
	debugWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea |Qt::RightDockWidgetArea);
	debugWidget->setWidget(&QtDebugMenu);
	addDockWidget(Qt::TopDockWidgetArea,debugWidget);
}

void MainWindow::createToolBar()
{

}

void MainWindow::guiValueChanged()
{
	
}

void MainWindow::openScene()
{

}

void MainWindow::newScene()
{

}

void MainWindow::saveScene()
{

}

void MainWindow::createLight()
{

}

void MainWindow::step()
{
	program->update(0.016f);
}

void MainWindow::timerEvent(QTimerEvent* e)
{
	QtDebugMenu.update();
	if(use3d)
	{
		GLWindow->update(dt);

		if(createDelayedProgramForThreeD)
		{
			program->init(GLWindow);
			createDelayedProgramForThreeD = false;
		}
	}
	else
	{
		QtWindow->update();
		QtWindow->repaint();
	}
	if(autoStep)
	{
		program->update(dt);
		dt = AutoTimer::end();
		if(dt <= 0.0f)
			dt = TARGETFRAMERATE;
		fps = 1/dt;
	}
	AutoTimer::start();
}