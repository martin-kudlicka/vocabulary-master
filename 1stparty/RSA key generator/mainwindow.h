#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = NULL, Qt::WindowFlags flags = 0);

	private:
		static const quint16 KEY_SIZE = 3072;

		Ui::MainWindow _ui;

	private slots:
		const void on_browsePrivate_clicked(bool checked = false);
		const void on_browsePublic_clicked (bool checked = false);
		const void on_generate_clicked     (bool checked = false) const;
}; // MainWindow

#endif // MAINWINDOW_H
