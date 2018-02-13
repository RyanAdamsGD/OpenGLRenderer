#include "QTDebugMenu.h"
#include <QtGui\qvboxlayout>
#include <QtGui\qboxlayout.h>
#include <QtGui\qcheckbox>
#include <Qt\qstring.h>
#include <QtGui\qlabel>
#include <QtGui\qslider.h>
#include <QtGui\qlabel.h>
#include <QtCore\qcoreapplication.h>
#include <QtGui\qmainwindow.h>
#include <Qt\qpushbutton.h>

QTDebugMenu* QTDebugMenu::instance = NULL;

static const float SLIDER_GRANULARITY = 50.0;

#ifndef DEBUGMENU
void QTDebugMenu::Init(){instance = new QTDebugMenu();}
void QTDebugMenu::visible(){}
void QTDebugMenu::addSlider(const char* name, float* value, float min, float max, bool newLine){}
void QTDebugMenu::addCheckBox(const char* name, bool* value, bool newLine){}
void QTDebugMenu::addWatchFloat(const char* name, const float* value, bool newLine){}
void QTDebugMenu::addButton(const char* text, basicEvent func, bool newLine){}
void QTDebugMenu::update(){}
#else

QTDebugMenu::QTDebugMenu()
	:nextCheckBox(0),nextButton(0),nextSlider(0),nextWatchFloat(0)
{	
	setLayout(mainLayout = new QVBoxLayout);
	setMaximumHeight(250);
}

void QTDebugMenu::Init()
{
	if(instance == NULL)
		instance = new QTDebugMenu();
}

void QTDebugMenu::visible()
{
	if(!isHidden())
	{
		hide();
	}
	else
	{
		show();
	}
}

void QTDebugMenu::addSlider(const char* name, float* value, float min, float max, bool newLine)
{
	if(nextSlider == 0 || newLine)
		mainLayout->addLayout(sliderLayout = new QHBoxLayout);

	QVBoxLayout* layout;
	sliderLayout->addLayout(layout = new QVBoxLayout);
	QLabel* label;
	layout->addWidget(label = new QLabel(name));
	label->setAlignment(Qt::AlignCenter);
	Slider& where = sliders[nextSlider++];
	layout->addWidget(where.slide = new QSlider(this));
	layout->addWidget(where.label = new QLabel);
	where.label->setMinimumWidth(35);
	where.label->setAlignment(Qt::AlignCenter);
	where.slide->setOrientation(Qt::Horizontal);
	where.slide->setMinimum(min * SLIDER_GRANULARITY);
	where.slide->setMaximum(max * SLIDER_GRANULARITY);
	where.slide->setValue(*value * SLIDER_GRANULARITY);
	where.value = value;
}

void QTDebugMenu::addCheckBox(const char* name, bool* value, bool newLine)
{
	if(nextCheckBox == 0 || newLine)
		mainLayout->addLayout(checkBoxLayout = new QHBoxLayout);

	QVBoxLayout* layout;
	checkBoxLayout->addLayout(layout = new QVBoxLayout);
	QLabel* label;
	layout->addWidget(label = new QLabel(name));
	CheckBox& where = checkBoxes[nextCheckBox++];
	layout->addWidget(where.checkbox = new QCheckBox);
	where.value = value;
	where.checkbox->setChecked(*value);
}

void QTDebugMenu::addWatchFloat(const char* name, const float* value, bool newLine)
{
	if(nextWatchFloat == 0 || newLine)
		mainLayout->addLayout(watchFloatLayout = new QHBoxLayout);

	QHBoxLayout* layout;
	watchFloatLayout->addLayout(layout = new QHBoxLayout);
	QLabel* label;
	layout->addWidget(label = new QLabel(name));
	label->setAlignment(Qt::AlignRight);
	WatchFloat& where = WFloats[nextWatchFloat++];
	layout->addWidget(where.text = new QLabel);
	where.text->setAlignment(Qt::AlignLeft);
	where.value = value;
}

void QTDebugMenu::addButton(const char* text, basicEvent func, bool newLine)
{
	if(nextButton == 0 || newLine)
		mainLayout->addLayout(buttonLayout = new QHBoxLayout);

	QVBoxLayout* layout;
	buttonLayout->addLayout(layout = new QVBoxLayout);
	Button& where = buttons[nextButton++];
	layout->addWidget(where.button = new QPushButton);
	where.button->setText(text);
	where.func = func;
	where.wasDown = false;
}


void QTDebugMenu::update()
{
	for(uint i=0;i<nextSlider;i++)
	{
		Slider& where = sliders[i];
		*where.value = where.slide->value() / SLIDER_GRANULARITY;
		QString temp;
		temp.setNum(*where.value);
		where.label->setText(temp);
	}

	for(uint i=0;i<nextCheckBox;i++)
	{
		CheckBox& where = checkBoxes[i];
		*where.value = where.checkbox->isChecked();
	}

	for(uint i=0;i<nextWatchFloat;i++)
	{
		WatchFloat& where = WFloats[i];
		QString temp;
		temp.setNum(*where.value);
		where.text->setText(temp);
	}

	for(uint i=0;i<nextButton;i++)
	{
		Button& where = buttons[i];
		if(where.button->isDown())
		{
			where.wasDown = true;
		}
		else if(where.wasDown)
		{
			where.wasDown = false;
			where.func();
		}
	}
}

#endif