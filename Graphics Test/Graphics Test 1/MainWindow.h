#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui\qmainwindow.h>
#include <Qt\qtimer.h>
#include "IUpdatable.h"
namespace Neumont{class Slider;}
class MainGLWindow;
class QCheckBox;
class QtPaintWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT
		MainGLWindow *GLWindow;
		QWidget* DebugMenu;
		QtPaintWindow* QtWindow;
		QToolBar* toolBar;
		QMenu* fileMenu;
		QMenu* createMenu;
		QStatusBar* statusBar;

		//widgets

		

		IUpdatable* program;
		float dt;
		float fps;
		bool createDelayedProgramForThreeD;
		bool autoStep;

private:
	void createMenus();
	void createToolBar();
	void createDockWidgets();

protected:
		void timerEvent(QTimerEvent* e);
	private slots:
		void guiValueChanged();
		void openScene();
		void newScene();
		void saveScene();
		void createLight();
public:
	bool use3d;
	void step();
	MainWindow(IUpdatable* program, bool threeDee = false);
	void status(char* message);
};

#endif