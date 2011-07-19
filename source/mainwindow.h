#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <ui_mainwindow.h>

#include "settings.h"
#ifndef FREE
# include <QtGui/QBoxLayout>
#endif
#include "vocabulary.h"
#ifndef FREE
# include "plugins.h"
# include <QtGui/QSystemTrayIcon>
#endif

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
        ~MainWindow();
		MainWindow(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
#ifndef FREE
		static const int FLASH_COUNT = 3;
		static const int FLASH_WAIT = 100;
#endif
		static const int MILISECONDS_PER_SECOND = 1000;
#ifndef FREE
        static const int SAY_BEEP_WAIT = 500;
#endif

		struct sAnswer {
			int iWord;
			bool bDirectionSwitched;
		}; // sAnswer

        typedef QHash<int, sAnswer> tAnswer;

		int _iCurrentWord;
        int _iTimerAnswer;
		int _iTimerQuestion;
#ifndef FREE
		Plugins _pPlugins;
		QAction *_qaTrayExit;
		QMenu _qmTray;
        QSystemTrayIcon _qstiTrayIcon;
#endif
		Settings _sSettings;
        tAnswer _taHash;
		Ui::qmwUiMain _umwMainWindow;
        Vocabulary _vVocabulary;

        const void ApplySettings(const bool &pStartup);
#ifndef FREE
		const void CreateTrayMenu();
#endif
        const void EnableControls();
#ifndef FREE
		bool event(QEvent *event);
#endif
		const QString GetLangColumn(const bool &pDirectionSwitched, const bool &pAnswer) const;
        const QString GetLanguageText(const bool &pDirectionSwitched, const bool &pAnswer) const;
		const bool GetLearningDirection() const;
        const QString GetLearningText(const bool &pDirectionSwitched, const bool &pAnswer) const;
#ifndef FREE
        const QString GetNoteColumn(const bool &pDirectionSwitched, const bool &pAnswer) const;
		const void Say(const bool &pDirectionSwitched, const bool &pAnswer) const;
        const void SetLayout();
		const void ShowTrayBalloon(const bool &pDirectionSwitched, const bool &pAnswer);
#endif
		void timerEvent(QTimerEvent *event);

	private slots:
#ifndef FREE
        const void on_qaAnswer_triggered(bool checked = false);
#endif
        const void on_qaManage_triggered(bool checked = false);
#ifndef FREE
        const void on_qaMute_toggled(bool checked);
#endif
        const void on_qaNew_triggered(bool checked = false);
		const void on_qaNext_triggered(bool checked = false);
        const void on_qaOpen_triggered(bool checked = false);
		const void on_qaSettings_triggered(bool checked = false);
		const void on_qaStart_triggered(bool checked = false);
		const void on_qaStop_triggered(bool checked = false);
#ifndef FREE
		const void on_qmTray_triggered(QAction *action);
		const void on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason reason);
#endif
}; // MainWindow

#endif // MAINWINDOW_H