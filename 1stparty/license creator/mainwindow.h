#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include <QtCore/QXmlStreamWriter>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		         MainWindow(QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~MainWindow();

	private:
		QXmlStreamWriter _xmlStreamWriter;
		Ui::MainWindow   _ui;

		const void writeLicense        ();
		const void writeLicenseGeneral ();
		const void writeLicensePersonal();

	private slots:
		const void on_create_clicked     (bool checked = false);
		const void on_generateUid_clicked(bool checked = false);
}; // MainWindow

#endif // MAINWINDOW_H
