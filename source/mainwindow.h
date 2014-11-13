#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <ui_mainwindow.h>

#include <QtWidgets/QProgressBar>
#ifndef EDITION_FREE
# include <QtWidgets/QBoxLayout>
# include "plugins.h"
# include <QtWidgets/QSystemTrayIcon>
#endif
#include <QtCore/QTranslator>
#ifndef EDITION_FREE
# include "license.h"
#endif
#include "updatechecker.h"
#include <QtCore/QTimer>
#include "vocabularyorganizer.h"
#include <stdint.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		         MainWindow(QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~MainWindow();

	private:
		enum Template
		{
			TemplateLearning
#ifndef EDITION_FREE
			, TemplateTray
#endif
		}; // Template

#ifndef EDITION_FREE
		static const quint8  FLASH_COUNT            = 3;
		static const quint8  FLASH_WAIT             = 100;
		static const quint16 MAX_NEXTRECORD_TRIES   = 9999;
#endif
		static const quint16 MILISECONDS_PER_SECOND = 1000;
        static const quint32 RECORD_NONE            = UINT32_MAX;
#ifndef EDITION_FREE
        static const quint16 SAY_BEEP_WAIT          = 500;
#endif
        static const quint8  TIME_NONE              = 0;
        static const quint8  TIME_NOW               = 1;

		      bool                            _directionSwitched;
		      bool                            _learning;
              qint8                           _timeAnswer;
		      qint8                           _timeQuestion;
#ifndef EDITION_FREE
# ifndef EDITION_TRY
              License                        *_license;
# endif
		      Plugins                         _plugins;
		const QAction                        *_trayExit;
        const QAction                        *_traySettings;
		      QAction                        *_trayVocabularies;
              QHBoxLayout                    *_hboxLayoutInner;
#endif
              QLabel                          _vocabularyStatus;
#ifndef EDITION_FREE
		      QMenu                           _menuTray;
		      QMenu                           _menuTrayVocabularies;
#endif
              QProgressBar                    _progressBarTimer;
#ifndef EDITION_FREE
              QSystemTrayIcon                 _trayIcon;
#endif
		      QTimer                          _learningTimer;
		      QTranslator                     _translator;
		      VocabularyOrganizer::RecordInfo _currentRecord;
		      Settings                        _settings;
		      Ui::MainWindow                  _ui;
		      UpdateChecker                   _updateChecker;
		      VocabularyOrganizer             _vocabularyOrganizer;

                void    applySettings         (bool startup);
#ifndef EDITION_FREE
		        void    createTrayMenu        ();
#endif
		        void    createVocabulariesMenu();
                void    enableControls        ();
		virtual bool    event                 (QEvent *event);
                QString languageText          (bool directionSwitched, bool answer)                        const;
		        bool    learningDirection     ()                                                           const;
                QString learningText          (Template templateType, bool directionSwitched, bool answer) const;
                void    openVocabulary        (Vocabulary *vocabulary
#ifndef EDITION_FREE
            , bool currentRecord
#endif
            );
#ifndef EDITION_FREE
				quint8  recordPriority        ()                                                           const;
#endif
                void    refreshStatusBar      ();
#ifndef EDITION_FREE
# ifdef Q_OS_WIN
                void    registerHotkeys       ()                                                           const;
# endif
		        void    say                   (bool directionSwitched, bool answer)                        const;
#endif
                void    setLayout             ();
#ifndef EDITION_FREE
		        void    setRecordEnabled      (bool enabled);
		        void    setRecordPriority     (quint8 priority);
		        void    setupRecordControls   ()                                                           const;
#endif
                void    showAnswer            ();
#ifndef EDITION_FREE
		        void    showTrayBalloon       (bool directionSwitched, bool answer);
#endif
#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
		virtual bool    winEvent              (MSG *message, long *result);
#endif

	private slots:
        void on_actionAbout_triggered              (bool checked = false);
#ifndef EDITION_FREE
        void on_actionAnswer_triggered             (bool checked = false);
        void on_actionFindInVocabulary_triggered   (bool checked = false);
# ifndef EDITION_TRY
        void on_actionLicense_triggered            (bool checked = false);
# endif
        void on_actionMute_toggled                 (bool checked);
#endif
		void on_actionNext_triggered               (bool checked = false);
        void on_actionOrganizer_triggered          (bool checked = false);
		void on_actionSettings_triggered           (bool checked = false);
		void on_actionStart_triggered              (bool checked = false);
		void on_actionStop_triggered               (bool checked = false);
		void on_learningTimer_timeout              ();
#ifndef EDITION_FREE
		void on_menuTray_triggered                 (QAction *action);
#endif
		void on_menuVocabularies_triggered         (QAction *action);
#ifndef EDITION_FREE
		void on_priority1_clicked                  (bool checked = false);
		void on_priority2_clicked                  (bool checked = false);
		void on_priority3_clicked                  (bool checked = false);
		void on_priority4_clicked                  (bool checked = false);
		void on_priority5_clicked                  (bool checked = false);
		void on_priority6_clicked                  (bool checked = false);
		void on_priority7_clicked                  (bool checked = false);
		void on_priority8_clicked                  (bool checked = false);
		void on_priority9_clicked                  (bool checked = false);
		void on_recordEnabled_clicked              (bool checked = false);
		void on_trayIcon_activated                 (QSystemTrayIcon::ActivationReason reason);
#endif
		void on_updateChecker_finished             ();
		void on_vocabularyOrganizer_vocabularyClose(const Vocabulary *pVocabulary);
}; // MainWindow

#endif // MAINWINDOW_H