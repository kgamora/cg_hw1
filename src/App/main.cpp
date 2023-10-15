#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(640, 480);
    window.show();

//	window.setAnimated(true);

	return app.exec();
}
