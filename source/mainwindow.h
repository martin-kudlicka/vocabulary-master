#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>

#include <QtWidgets/QProgressBar>
#include "plugins.h"
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QTranslator>
#include "updatechecker.h"
#include <QtCore/QTimer>
#include "vocabularyorganizer.h"
#include <stdint.h>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
             MainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~MainWindow() Q_DECL_OVERRIDE;

  private:
    enum class Template
    {
      Learning,
      Tray
    };

          bool                            _directionSwitched;
          bool                            _learning;
          qintptr                         _timeAnswer;
          qintptr                         _timeQuestion;
          Plugins                         _plugins;
    const QAction                        *_trayExit;
    const QAction                        *_traySettings;
          QAction                        *_trayVocabularies;
          QHBoxLayout                    *_hboxLayoutInner;
          QLabel                          _vocabularyStatus;
          QMenu                           _menuTray;
          QMenu                           _menuTrayVocabularies;
          QProgressBar                    _progressBarTimer;
          QSystemTrayIcon                 _trayIcon;
          QTimer                          _learningTimer;
          QTranslator                     _translator;
          VocabularyOrganizer::RecordInfo _currentRecord;
          Settings                        _settings;
          Ui::MainWindow                  _ui;
          UpdateChecker                   _updateChecker;
          VocabularyOrganizer             _vocabularyOrganizer;

    void     applySettings         (bool startup);
    void     createTrayMenu        ();
    void     createVocabulariesMenu();
    void     enableControls        ();
    QString  languageText          (bool directionSwitched, bool answer)                        const;
    bool     learningDirection     ()                                                           const;
    QString  learningText          (Template templateType, bool directionSwitched, bool answer) const;
    void     openVocabulary        (Vocabulary *vocabulary, bool currentRecord);
    quintptr recordPriority        ()                                                           const;
    void     refreshStatusBar      ();
#ifdef Q_OS_WIN
    void     registerHotkeys       ()                                                           const;
#endif
    void     say                   (bool directionSwitched, bool answer)                        const;
    void     setLayout             ();
    void     setRecordEnabled      (bool enabled);
    void     setRecordPriority     (quintptr priority);
    void     setupRecordControls   ()                                                           const;
    void     showAnswer            ();
    void     showTrayBalloon       (bool directionSwitched, bool answer);

    virtual bool event      (QEvent *event)                                            Q_DECL_OVERRIDE;
#ifdef Q_OS_WIN
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;
#endif

  private Q_SLOTS:
    void on_actionAbout_triggered              (bool checked = false);
    void on_actionAnswer_triggered             (bool checked = false);
    void on_actionFindInVocabulary_triggered   (bool checked = false);
    void on_actionLicense_triggered            (bool checked = false);
    void on_actionMute_toggled                 (bool checked);
    void on_actionNext_triggered               (bool checked = false);
    void on_actionOrganizer_triggered          (bool checked = false);
    void on_actionSettings_triggered           (bool checked = false);
    void on_actionStart_triggered              (bool checked = false);
    void on_actionStop_triggered               (bool checked = false);
    void on_learningTimer_timeout              ();
    void on_menuTray_triggered                 (QAction *action);
    void on_menuVocabularies_triggered         (QAction *action);
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
    void on_updateChecker_finished             ();
    void on_vocabularyOrganizer_vocabularyClose(const Vocabulary *pVocabulary);
};

#endif