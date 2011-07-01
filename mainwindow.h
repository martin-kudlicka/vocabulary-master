#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "settings.h"
#include <QtGui/QBoxLayout>
#include "vocabulary.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
        ~MainWindow();
		MainWindow(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Settings _sSettings;
		Ui::qmwUiMain _umwMainWindow;
        Vocabulary _vVocabulary;

        const void ApplySettings();
        const void EnableControls();
        const void SetLayout();

	private slots:
        const void on_qaManage_triggered(bool checked = false);
        const void on_qaNew_triggered(bool checked = false);
        const void on_qaOpen_triggered(bool checked = false);
		const void on_qaSettings_triggered(bool checked = false);
}; // MainWindow

#endif // MAINWINDOW_H