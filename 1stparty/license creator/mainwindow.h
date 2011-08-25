#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include <QtCore/QXmlStreamWriter>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		QXmlStreamWriter _qxswXmlWriter;
		Ui::qmwUiMain _umwMainWindow;

		const void WriteLicense();
		const void WriteLicenseGeneral();
		const void WriteLicensePersonal();

	private slots:
		const void on_qpbCreate_clicked(bool checked = false);
		const void on_qpbGenerateUid_clicked(bool checked = false);
}; // MainWindow

#endif // MAINWINDOW_H
