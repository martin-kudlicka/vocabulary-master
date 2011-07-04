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
		static const int MILISECONDS_PER_SECOND = 1000;

		int _iCurrentWord;
		int _iTimerLearing;
		Settings _sSettings;
		Ui::qmwUiMain _umwMainWindow;
        Vocabulary _vVocabulary;

        const void ApplySettings(const bool &pStartup);
        const void EnableControls();
		const QString GetLangColumn(const bool &pAnswer) const;
        const QString GetLearningText(const bool &pAnswer) const;
        const QString GetNoteColumn(const bool &pAnswer) const;
        const void SetLayout();
		void timerEvent(QTimerEvent *event);

	private slots:
        const void on_qaManage_triggered(bool checked = false);
        const void on_qaNew_triggered(bool checked = false);
		const void on_qaNext_triggered(bool checked = false);
        const void on_qaOpen_triggered(bool checked = false);
		const void on_qaSettings_triggered(bool checked = false);
		const void on_qaStart_triggered(bool checked = false);
		const void on_qaStop_triggered(bool checked = false);
		const void OnShowTranslation();
}; // MainWindow

#endif // MAINWINDOW_H