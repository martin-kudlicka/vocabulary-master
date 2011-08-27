#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		static const int KEY_SIZE = 3072;

		Ui::qmwMainWindow _qmwMainWindow;

	private slots:
		const void on_qpbBrowsePublic_clicked(bool checked = false);
		const void on_qpbBrowsePrivate_clicked(bool checked = false);
		const void on_qpbGenerate_clicked(bool checked = false) const;
}; // MainWindow

#endif // MAINWINDOW_H
