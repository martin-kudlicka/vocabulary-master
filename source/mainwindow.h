#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <ui_mainwindow.h>

#include <QtGui/QProgressBar>
#ifndef FREE
# include <QtGui/QBoxLayout>
# include "plugins.h"
# include <QtGui/QSystemTrayIcon>
#endif
#include <QtCore/QTranslator>
#ifndef FREE
# include "license.h"
#endif
#include "updatechecker.h"
#include <QtCore/QTimer>
#include "vocabularyorganizer.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
        ~MainWindow();
		MainWindow(QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		enum eTemplate {
			TemplateLearning
#ifndef FREE
			, TemplateTray
#endif
		}; // eTemplate

#ifndef FREE
		static const int FLASH_COUNT = 3;
		static const int FLASH_WAIT = 100;
		static const int MAX_NEXTRECORD_TRIES = 9999;
#endif
		static const int MILISECONDS_PER_SECOND = 1000;
        static const int RECORD_NONE = -1;
#ifndef FREE
        static const int SAY_BEEP_WAIT = 500;
#endif
        static const int TIME_NONE = -1;
        static const int TIME_NOW = 1;

		bool _bDirectionSwitched;
		bool _bLearning;
        int _iTimeAnswer;
		int _iTimeQuestion;
#ifndef FREE
# ifndef TRY
        License *_lLicense;
# endif
		Plugins _pPlugins;
		QAction *_qaTrayExit;
        QAction *_qaTraySettings;
		QAction *_qaTrayVocabularies;
        QHBoxLayout *_qhblInner;
#endif
        QLabel _qlVocabularyStatus;
#ifndef FREE
		QMenu _qmTray;
		QMenu _qmTrayVocabularies;
#endif
        QProgressBar _qpbTimer;
#ifndef FREE
        QSystemTrayIcon _qstiTrayIcon;
#endif
		QTimer _qtLearning;
		QTranslator _qtTranslator;
		VocabularyOrganizer::sRecordInfo _sriCurrentRecord;
		Settings _sSettings;
		Ui::qmwUiMain _umwMainWindow;
		UpdateChecker _ucUpdateChecker;
		VocabularyOrganizer _voOrganizer;

        const void ApplySettings(const bool &pStartup);
#ifndef FREE
		const void CreateTrayMenu();
#endif
		const void CreateVocabulariesMenu();
        const void EnableControls();
		virtual bool event(QEvent *event);
        const QString GetLanguageText(const bool &pDirectionSwitched, const bool &pAnswer) const;
		const bool GetLearningDirection() const;
        const QString GetLearningText(const eTemplate &pTemplate, const bool &pDirectionSwitched, const bool &pAnswer) const;
#ifndef FREE
        const int GetRecordPriority() const;
#endif
        const void OpenVocabulary(Vocabulary *pVocabulary
#ifndef FREE
            , const bool &pCurrentRecord
#endif
            );
        const void RefreshStatusBar();
#ifndef FREE
# ifdef Q_WS_WIN
        const void RegisterHotkeys() const;
# endif
		const void Say(const bool &pDirectionSwitched, const bool &pAnswer) const;
#endif
        const void SetLayout();
#ifndef FREE
		const void SetRecordEnabled(const bool &pEnabled);
		const void SetRecordPriority(const int &pPriority);
		const void SetupRecordControls() const;
#endif
        const void ShowAnswer();
#ifndef FREE
		const void ShowTrayBalloon(const bool &pDirectionSwitched, const bool &pAnswer);
#endif
#if !defined(FREE) && defined(Q_WS_WIN)
		virtual bool winEvent(MSG *message, long *result);
#endif

	private slots:
        const void on_qaAbout_triggered(bool checked = false);
#ifndef FREE
        const void on_qaAnswer_triggered(bool checked = false);
        const void on_qaFindInVocabulary_triggered(bool checked = false);
# ifndef TRY
        const void on_qaLicense_triggered(bool checked = false);
# endif
        const void on_qaMute_toggled(bool checked);
#endif
		const void on_qaNext_triggered(bool checked = false);
        const void on_qaOrganizer_triggered(bool checked = false);
		const void on_qaSettings_triggered(bool checked = false);
		const void on_qaStart_triggered(bool checked = false);
		const void on_qaStop_triggered(bool checked = false);
#ifndef FREE
		const void on_qcbRecordEnabled_clicked(bool checked = false);
		const void on_qmTray_triggered(QAction *action);
#endif
		const void on_qmVocabularies_triggered(QAction *action);
#ifndef FREE
		const void on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason reason);
		const void on_qtbPriority1_clicked(bool checked = false);
		const void on_qtbPriority2_clicked(bool checked = false);
		const void on_qtbPriority3_clicked(bool checked = false);
		const void on_qtbPriority4_clicked(bool checked = false);
		const void on_qtbPriority5_clicked(bool checked = false);
		const void on_qtbPriority6_clicked(bool checked = false);
		const void on_qtbPriority7_clicked(bool checked = false);
		const void on_qtbPriority8_clicked(bool checked = false);
		const void on_qtbPriority9_clicked(bool checked = false);
#endif
		const void on_qtLearning_timeout();
		const void on_ucUpdateChecker_Finished();
		const void on_voOrganizer_VocabularyClose(const Vocabulary *pVocabulary);
}; // MainWindow

#endif // MAINWINDOW_H