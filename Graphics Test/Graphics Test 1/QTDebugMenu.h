#ifndef QTDEBUGMENU
#define QTDEBUGMENU
#include <QtGui\qwidget.h>
#include <MyTypeDefs.h>
#include <FastDelegate.h>

//define out the class
#define DEBUGMENU

typedef fastdelegate::FastDelegate0<> basicEvent;

class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QCheckBox;
class QSlider;
class QPushButton;
namespace Neumont {class Slider;}

class QTDebugMenu: public QWidget
{
	Q_OBJECT	

	QVBoxLayout* mainLayout;
public:

	struct Slider{
		QSlider* slide;
		QLabel* label;
		float* value;
	};
	static const uint MAX_NUM_SLIDER_COUNT = 20;
	uint nextSlider;
	Slider sliders[MAX_NUM_SLIDER_COUNT];
	
	struct CheckBox{
		bool* value;
		QCheckBox* checkbox;
	};
	static const uint MAX_NUM_CHECKBOX_COUNT = 20;
	uint nextCheckBox;
	CheckBox checkBoxes[MAX_NUM_CHECKBOX_COUNT];

	struct WatchFloat{
		const float* value;
		QLabel* text;
	};
	static const uint MAX_NUM_WFLOAT_COUNT = 20;
	uint nextWatchFloat;
	WatchFloat WFloats[MAX_NUM_WFLOAT_COUNT];

	struct Button{
		basicEvent func;
		QPushButton* button;
		bool wasDown;
	};
	static const uint MAX_NUM_BUTTON_COUNT = 20;
	uint nextButton;
	Button buttons[MAX_NUM_BUTTON_COUNT];	

	QHBoxLayout* sliderLayout, *buttonLayout,*checkBoxLayout,*watchFloatLayout;
	static QTDebugMenu* instance;

	//constructors
	QTDebugMenu();
	QTDebugMenu(const QTDebugMenu&){}
public:	
	~QTDebugMenu(){}
	static QTDebugMenu& getInstance(){ return *instance; }

	void Init();
	void visible();
	void addSlider(const char* name, float* value, float min =-10.0f, float max = 10.0f, bool newLine = false);
	void addCheckBox(const char* name, bool* value, bool newLine = false);
	void addWatchFloat(const char* name, const float* value, bool newLine = false);
	void addButton(const char* text, basicEvent func, bool newLine = false);
	void update();
};

#define QtDebugMenu QTDebugMenu::getInstance()

#endif