#include <MkWidgets/MApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  MApplication application(argc, argv);

  const MainWindow mainWindow;

  return application.exec();
}