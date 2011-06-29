#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "settings.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
		~MainWindow();

	private:
		Settings _sSettings;
		Ui::UiMainWindow _umwMainWindow;

	private slots:
		const void on_ActionSettings_triggered(bool checked = false);
}; // MainWindow

#endif // MAINWINDOW_H