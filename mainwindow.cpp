#include "mainwindow.h"

#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent /* 0 */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags)
{
	_umwMainWindow.setupUi(this);
} // MainWindow

MainWindow::~MainWindow()
{

} // ~MainWindow

const void MainWindow::on_ActionSettings_triggered(bool checked /* false */)
{
	SettingsDialog sdDialog(&_sSettings);
	sdDialog.exec();
} // on_ActionSettings_triggered