#include <Qt\qapplication.h>
#include "MainWindow.h"
#include "LetsBounce.h"
#include "WhatADrag.h"
#include "CollisionResolution.h"
#include "TheRoofTheRoofIsOnFire.h"
#include "Flag.h"
#include "RestingParticles.h"
#include "Water.h"
#include "Hair.h"
#include "RayTraceDemo.h"

int main(int argc, char* argv[])
{	
	QApplication app(argc,argv);
	MainWindow widget(new RestingParticles(),true);
	widget.show();
	return app.exec();
}