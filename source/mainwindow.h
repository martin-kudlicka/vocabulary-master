#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

#include "settings.h"
#include <QtGui/QBoxLayout>
#include "vocabulary.h"
#include "plugins.h"
#include <QtGui/QSystemTrayIcon>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
        ~MainWindow();
		MainWindow(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		static const int FLASH_COUNT = 3;
		static const int FLASH_WAIT = 100;
		static const int MILISECONDS_PER_SECOND = 1000;
        static const int SAY_BEEP_WAIT = 500;

		struct sAnswer {
			int iWord;
			bool bDirectionSwitched;
		}; // sAnswer

        typedef QHash<int, sAnswer> tAnswer;

		int _iCurrentWord;
        int _iTimerAnswer;
		int _iTimerQuestion;
		Plugins _pPlugins;
        QSystemTrayIcon _qstiTrayIcon;
		Settings _sSettings;
        tAnswer _taHash;
		Ui::qmwUiMain _umwMainWindow;
        Vocabulary _vVocabulary;

        const void ApplySettings(const bool &pStartup);
        const void EnableControls();
		const QString GetLangColumn(const bool &pDirectionSwitched, const bool &pAnswer) const;
        const QString GetLanguageText(const bool &pDirectionSwitched, const bool &pAnswer) const;
		const bool GetLearningDirection() const;
        const QString GetLearningText(const bool &pDirectionSwitched, const bool &pAnswer) const;
        const QString GetNoteColumn(const bool &pDirectionSwitched, const bool &pAnswer) const;
		const void Say(const bool &pDirectionSwitched, const bool &pAnswer) const;
        const void SetLayout();
		void timerEvent(QTimerEvent *event);

	private slots:
        const void on_qaAnswer_triggered(bool checked = false);
        const void on_qaManage_triggered(bool checked = false);
        const void on_qaMute_toggled(bool checked);
        const void on_qaNew_triggered(bool checked = false);
		const void on_qaNext_triggered(bool checked = false);
        const void on_qaOpen_triggered(bool checked = false);
		const void on_qaSettings_triggered(bool checked = false);
		const void on_qaStart_triggered(bool checked = false);
		const void on_qaStop_triggered(bool checked = false);
}; // MainWindow

#endif // MAINWINDOW_H