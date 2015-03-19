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
		virtual ~MainWindow() override;

	private:
		QXmlStreamWriter _xmlStreamWriter;
		Ui::MainWindow   _ui;

		void writeLicense        ();
		void writeLicenseGeneral ();
		void writeLicensePersonal();

	private slots:
		void on_create_clicked     (bool checked = false);
		void on_generateUid_clicked(bool checked = false);
}; // MainWindow

#endif // MAINWINDOW_H