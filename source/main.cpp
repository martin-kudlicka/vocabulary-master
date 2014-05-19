#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	const MainWindow mainWindow;

	return application.exec();
} // main