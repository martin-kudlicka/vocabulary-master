#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);
  application.setOrganizationName("MartinK");
  application.setApplicationName("Vocabulary Master");

  const MainWindow mainWindow;

  return application.exec();
}