#include "mainwindow.h"

#include "settingsdialog.h"
#include "vocabularymanagerdialog.h"
#include <QtCore/QTime>
#include <QtTest/QTest>
#include <QtWidgets/QMessageBox>
#ifdef Q_OS_WIN
# include <QtCore/qt_windows.h>
#endif
#include "common/licensetextdialog.h"
#include <QtMultimedia/QSound>
#include "vocabularymanagerdialog/prioritydelegate.h"
#include "vocabularyorganizerdialog.h"
#include <MkWidgets/MAboutBox>
#include <MkSupport/MFeedback>

const auto FLASH_COUNT            = 3;
const auto FLASH_WAIT             = 100;
const auto MAX_NEXTRECORD_TRIES   = 9999;
const auto MILISECONDS_PER_SECOND = 1000;
const auto RECORD_NONE            = UINTPTR_MAX;
const auto SAY_BEEP_WAIT          = 500;
const auto TIME_NONE              = 0;
const auto TIME_NOW               = 1;
const auto VOCABULARY_MASTER      = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary Master");

MainWindow::MainWindow() : _learning(false), _hboxLayoutInner(Q_NULLPTR), _updateChecker(&_settings), _vocabularyOrganizer(&_settings)
{
  _learningTimer.setSingleShot(true);

  qsrand(QTime::currentTime().msec());

  // gui
  _ui.setupUi(this);
  _trayIcon.setToolTip("Vocabulary Master");
  _ui.statusBar->addWidget(&_vocabularyStatus);
  _progressBarTimer.setTextVisible(false);
  _ui.statusBar->addWidget(&_progressBarTimer, 1);

  // plugins
  _plugins.load();
  _plugins.initialize();

  // system tray icon
  const QIcon trayIcon(":/resources/images/mainwindow.png");
  _trayIcon.setIcon(trayIcon);

  // translator
  QCoreApplication::installTranslator(&_translator);

  // settings
  applySettings(true);

  _vocabularyOrganizer.openAll(this);
  refreshStatusBar();

  // menus
  createTrayMenu();
  createVocabulariesMenu();

  // controls
  enableControls();
  _ui.actionMute->setChecked(_settings.mute());

  // connections
  connect(&_menuTrayVocabularies, &QMenu::triggered,                     this, &MainWindow::on_menuVocabularies_triggered);
  connect(&_trayIcon,             &QSystemTrayIcon::activated,           this, &MainWindow::on_trayIcon_activated);
  connect(&_learningTimer,        &QTimer::timeout,                      this, &MainWindow::on_learningTimer_timeout);
  connect(&_updateChecker,        &UpdateChecker::finished,              this, &MainWindow::on_updateChecker_finished);
  connect(&_vocabularyOrganizer,  &VocabularyOrganizer::vocabularyClose, this, &MainWindow::on_vocabularyOrganizer_vocabularyClose);

  // learning
  if (_settings.startLearningOnStartup() && _vocabularyOrganizer.isOpen())
  {
    _ui.actionStart->trigger();
  }

  // update check
  if (_settings.updateCheck())
  {
    _updateChecker.checkForUpdate();
  }
}

MainWindow::~MainWindow()
{
  _settings.setWindowX(geometry().x());
  _settings.setWindowY(geometry().y());
  _settings.setWindowHeight(geometry().height());
  _settings.setWindowWidth(geometry().width());

  _plugins.uninitialize();
}

void MainWindow::applySettings(bool startup)
{
  setLayout();

  // change translation
  if (!_translator.load(_settings.translation(), DIR_LANG))
  {
    if (_settings.translation().isEmpty())
    {
      if (_translator.load(QLocale::system().name(), DIR_LANG))
      {
        _settings.setTranslation(QLocale::system().name() + '.' + LANG_SUFFIX);
      }
    }
  }
  _plugins.setLanguage(_settings.translation());

  _ui.toolBar->setVisible(_settings.showToolBar());
  _ui.language1->setVisible(_learning && _settings.showLanguageNames());
  _ui.language2->setVisible(_learning && _settings.showLanguageNames());
  _ui.category->setVisible(_learning && _settings.showCategoryName());
  _ui.recordControls->parentWidget()->setVisible(_learning && _settings.showRecordControls());
  _ui.statusBar->setVisible(_settings.showStatusBar());

  if (_settings.alwaysOnTop())
  {
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  }
  else
  {
    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
  }
  if (startup && _settings.windowX() != Settings::DEFAULT_DIMENSION)
  {
    setGeometry(_settings.windowX(), _settings.windowY(), _settings.windowWidth(), _settings.windowHeight());
  }
  show();

  _trayIcon.setVisible(_settings.systemTrayIcon());

#ifdef Q_OS_WIN
  registerHotkeys();
#endif
}

void MainWindow::createTrayMenu()
{
  _trayVocabularies = _menuTray.addAction(QIcon(":/resources/images/menubar/manage.png"), tr("&Vocabularies"));
  _trayVocabularies->setMenu(&_menuTrayVocabularies);
  _traySettings = _menuTray.addAction(QIcon(":/resources/images/menubar/settings.png"), tr("&Settings"));
  _menuTray.addSeparator();
  _trayExit = _menuTray.addAction(QIcon(":/resources/images/menubar/exit.png"), tr("&Exit"));

  connect(&_menuTray, &QMenu::triggered, this, &MainWindow::on_menuTray_triggered);
  _trayIcon.setContextMenu(&_menuTray);
}

void MainWindow::createVocabulariesMenu()
{
  _ui.menuVocabularies->clear();
  _menuTrayVocabularies.clear();

  for (auto vocabularyIndex = 0; vocabularyIndex < _vocabularyOrganizer.vocabularyCount(); ++vocabularyIndex)
  {
    const auto vocabularyInfo = _vocabularyOrganizer.vocabularyInfo(vocabularyIndex);
    const auto name           = vocabularyInfo.vocabulary->name();

    // main menu
    auto action = _ui.menuVocabularies->addAction(QIcon(":/resources/images/menubar/vocabulary.png"), name);
    action->setData(vocabularyIndex);

    // tray menu
    action = _menuTrayVocabularies.addAction(QIcon(":/resources/images/menubar/vocabulary.png"), name);
    action->setData(vocabularyIndex);
  }
}

void MainWindow::enableControls()
{
  // menu
  _ui.menuVocabularies->setEnabled(_vocabularyOrganizer.isOpen());

  // tool bar
  _ui.actionStart->setEnabled(_vocabularyOrganizer.isOpen() && !_learning && _vocabularyOrganizer.recordCount(true) > 0);
  _ui.actionStop->setEnabled(_learning);
  _ui.actionNext->setEnabled(_learning);
  _ui.actionFindInVocabulary->setEnabled(_learning);
  _ui.actionAnswer->setEnabled(_learning && _timeAnswer >= TIME_NOW);

  // tray
  _trayVocabularies->setEnabled(_vocabularyOrganizer.isOpen());
}

QString MainWindow::languageText(bool directionSwitched, bool answer) const
{
  if ((!directionSwitched && !answer) || (directionSwitched && answer))
  {
    return _currentRecord.vocabulary->languageName(VocabularyDatabase::FieldLanguage::Left);
  }
  else
  {
    return _currentRecord.vocabulary->languageName(VocabularyDatabase::FieldLanguage::Right);
  }
}

bool MainWindow::learningDirection() const
{
  if (_settings.switchLearningDirection() != Qt::PartiallyChecked)
  {
    return _settings.switchLearningDirection();
  }
  else
  {
    return qrand() % 2;
  }
}

QString MainWindow::learningText(Template templateType, bool directionSwitched, bool answer) const
{
  QString templateText;
  VocabularyDatabase::FieldLanguage fieldLanguage;
  if ((!directionSwitched && !answer) || (directionSwitched && answer))
  {
    if (templateType == Template::Learning)
    {
      templateText = _currentRecord.vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguage::Left);
    }
    else
    {
      templateText = _currentRecord.vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguage::Left);
    }
    fieldLanguage = VocabularyDatabase::FieldLanguage::Left;
  }
  else
  {
    if (templateType == Template::Learning)
    {
      templateText = _currentRecord.vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguage::Right);
    }
    else
    {
      templateText = _currentRecord.vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguage::Right);
    }
    fieldLanguage = VocabularyDatabase::FieldLanguage::Right;
  }

  // substitute variables in template
  for (auto fieldId : _currentRecord.vocabulary->fieldIds())
  {
    if (_currentRecord.vocabulary->fieldLanguage(fieldId) == fieldLanguage)
    {
      const auto data = _currentRecord.vocabulary->dataText(_currentRecord.id, fieldId);

      auto field = _currentRecord.vocabulary->fieldTemplateName(fieldId);
      templateText.replace(VARIABLE_MARK + field, data);
    }
  }

  return templateText;
}

void MainWindow::openVocabulary(const QSharedPointer<Vocabulary> &vocabulary, bool currentRecord)
{
  VocabularyManagerDialog vocabularyManagerDialog(vocabulary, &_settings, &_plugins, this);
  if (currentRecord)
  {
    vocabularyManagerDialog.execOnRecord(_currentRecord.id);
  }
  else
  {
    vocabularyManagerDialog.exec();
  }

  _ui.actionStart->setEnabled(_vocabularyOrganizer.isOpen() && !_learning && _vocabularyOrganizer.recordCount() > 0);
  refreshStatusBar();
}

quintptr MainWindow::recordPriority() const
{
  for (auto fieldId : _currentRecord.vocabulary->fieldIds())
  {
    if (_currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      const auto builtInType = _currentRecord.vocabulary->fieldBuiltIn(fieldId);
      switch (builtInType)
      {
        case VocabularyDatabase::FieldBuiltIn::Priority:
          return _currentRecord.vocabulary->dataText(_currentRecord.id, fieldId).toUInt();
      }
    }
  }

  return PriorityDelegate::RECORD_PRIORITY_MIN;
}

void MainWindow::refreshStatusBar()
{
  if (!_vocabularyOrganizer.isOpen())
  {
    _vocabularyStatus.setText(tr("no vocabulary"));
  }
  else
  {
    const auto count = QString("%1/%2").arg(_vocabularyOrganizer.recordCount(true)).arg(_vocabularyOrganizer.recordCount());
    _vocabularyStatus.setText(tr("%1 voc., %2 records").arg(_vocabularyOrganizer.vocabularyCount()).arg(count));
  }
}

#ifdef Q_OS_WIN
void MainWindow::registerHotkeys() const
{
  for (auto hotkeyIndex = 0; hotkeyIndex < static_cast<quintptr>(Settings::Hotkey::Count) - 1; ++hotkeyIndex)
  {
    const auto hotkeyInfo = _settings.hotkey(static_cast<Settings::Hotkey>(hotkeyIndex));

    if (hotkeyInfo.virtualKey == SettingsDialog::VIRTUALKEY_NONE)
    {
      UnregisterHotKey(reinterpret_cast<HWND>(winId()), hotkeyIndex);
    }
    else
    {
      UINT modifiers;
      if (hotkeyInfo.text.contains(MODIFIER_ALT))
      {
        modifiers = MOD_ALT;
      }
      else
      {
        modifiers = 0;
      }
      if (hotkeyInfo.text.contains(MODIFIER_CTRL))
      {
        modifiers |= MOD_CONTROL;
      }
      if (hotkeyInfo.text.contains(MODIFIER_SHIFT))
      {
        modifiers |= MOD_SHIFT;
      }

      RegisterHotKey(reinterpret_cast<HWND>(winId()), hotkeyIndex, modifiers, hotkeyInfo.virtualKey);
    }
  }
}
#endif

void MainWindow::say(bool directionSwitched, bool answer) const
{
  if (!_settings.mute())
  {
    VocabularyDatabase::FieldLanguage fieldLanguage;
    if ((!directionSwitched && !answer) || (directionSwitched && answer))
    {
      fieldLanguage = VocabularyDatabase::FieldLanguage::Left;
    }
    else
    {
      fieldLanguage = VocabularyDatabase::FieldLanguage::Right;
    }

    // get text to speech
    QString text;
    for (auto fieldId : _currentRecord.vocabulary->fieldIds())
    {
      if (_currentRecord.vocabulary->fieldLanguage(fieldId) == fieldLanguage && _currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::Speech))
      {
        if (!text.isEmpty())
        {
          text += ' ';
        }
        text += _currentRecord.vocabulary->dataText(_currentRecord.id, fieldId);
      }
    }

    if (!text.isEmpty())
    {
      const auto speechPlugin = _currentRecord.vocabulary->languageSpeech(fieldLanguage);
      if (speechPlugin != TTSInterface::TTSPlugin::None)
      {
        auto speechInterface = _plugins.ttsPlugin(speechPlugin);
        if (speechInterface)
        {
          const auto voice = _currentRecord.vocabulary->languageVoice(fieldLanguage);
          speechInterface->say(voice, text);
        }
      }
    }
  }
}

void MainWindow::setLayout()
{
  if (_hboxLayoutInner)
  {
    _hboxLayoutInner->deleteLater();
    _hboxLayoutInner = Q_NULLPTR;
  }
  if (_ui.centralWidget->layout())
  {
    delete _ui.centralWidget->layout();
  }

  auto mainLayout = new QVBoxLayout(_ui.centralWidget);
  if (_settings.horizontalLayout())
  {
    mainLayout->addWidget(_ui.category);
    mainLayout->addWidget(_ui.recordControls->parentWidget());
    auto innerLayout = new QHBoxLayout;
    innerLayout->addWidget(_ui.questionLayout->parentWidget());
    innerLayout->addWidget(_ui.answerLayout->parentWidget());
    mainLayout->addLayout(innerLayout);
  }
  else
  {
    mainLayout->addWidget(_ui.questionLayout->parentWidget());
    mainLayout->addWidget(_ui.category);
    mainLayout->addWidget(_ui.recordControls->parentWidget());
    mainLayout->addWidget(_ui.answerLayout->parentWidget());
  }
}

void MainWindow::setRecordEnabled(bool enabled)
{
  for (auto fieldId : _currentRecord.vocabulary->fieldIds())
  {
    if (_currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      const auto builtInType = _currentRecord.vocabulary->fieldBuiltIn(fieldId);
      switch (builtInType)
      {
        case VocabularyDatabase::FieldBuiltIn::Enabled:
          _currentRecord.vocabulary->setDataText(_currentRecord.id, fieldId, QString::number(enabled ? Qt::Checked : Qt::Unchecked));
          return;
      }
    }
  }
}

void MainWindow::setRecordPriority(quintptr priority)
{
  for (auto fieldId : _currentRecord.vocabulary->fieldIds())
  {
    if (_currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      const auto builtInType = _currentRecord.vocabulary->fieldBuiltIn(fieldId);
      switch (builtInType)
      {
        case VocabularyDatabase::FieldBuiltIn::Priority:
          _currentRecord.vocabulary->setDataText(_currentRecord.id, fieldId, QString::number(priority));
          return;
      }
    }
  }
}

void MainWindow::setupRecordControls() const
{
  const auto priority = recordPriority();
  _ui.priority1->setChecked(priority == 1);
  _ui.priority2->setChecked(priority == 2);
  _ui.priority3->setChecked(priority == 3);
  _ui.priority4->setChecked(priority == 4);
  _ui.priority5->setChecked(priority == 5);
  _ui.priority6->setChecked(priority == 6);
  _ui.priority7->setChecked(priority == 7);
  _ui.priority8->setChecked(priority == 8);
  _ui.priority9->setChecked(priority == 9);
  _ui.recordEnabled->setChecked(_currentRecord.vocabulary->recordEnabled(_currentRecord.id));
}

void MainWindow::showAnswer()
{
  // answer
  _ui.actionAnswer->setEnabled(false);

  // gui
  _ui.window2->setText(learningText(Template::Learning, _directionSwitched, true));
  _ui.window2->repaint();

  // tray
  if (_settings.systemTrayIcon() && _settings.showWordsInTrayBalloon())
  {
    showTrayBalloon(_directionSwitched, true);
  }

  // speech
  say(_directionSwitched, true);

  // progress
  _progressBarTimer.setMaximum(_timeQuestion);
  _progressBarTimer.setValue(_timeQuestion);
}

void MainWindow::showTrayBalloon(bool directionSwitched, bool answer)
{
  auto text = learningText(Template::Tray, directionSwitched, false);
  if (answer)
  {
    text += " -> " + learningText(Template::Tray, directionSwitched, true);
  }

  _trayIcon.showMessage(tr(VOCABULARY_MASTER), text);
}

bool MainWindow::event(QEvent *event)
{
  switch (event->type())
  {
    case QEvent::LanguageChange:
      {
        QString category, language1, language2;
        if (_learning)
        {
          language1 = _ui.language1->text();
          language2 = _ui.language2->text();
          category = _ui.category->text();
        }
        _ui.retranslateUi(this);
        if (_learning)
        {
          _ui.language1->setText(language1);
          _ui.language2->setText(language2);
          _ui.category->setText(category);
        }
      }
      break;
    case QEvent::WindowStateChange:
      if (isMinimized() && _settings.systemTrayIcon() && _settings.minimizeToTray())
      {
        setWindowFlags(windowFlags() | Qt::CustomizeWindowHint); // just add some flag to hide window
      }
  }

  return QMainWindow::event(event);
}

#ifdef Q_OS_WIN
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
  const auto msg = static_cast<LPMSG>(message);
  if (msg->message == WM_HOTKEY)
  {
    switch (msg->wParam)
    {
      case Settings::Hotkey::Answer:
        if (_ui.actionAnswer->isEnabled())
        {
          _ui.actionAnswer->trigger();
        }
        break;
      case Settings::Hotkey::Minimize:
        showMinimized();
        break;
      case Settings::Hotkey::Next:
        if (_ui.actionNext->isEnabled())
        {
          _ui.actionNext->trigger();
        }
        break;
      case Settings::Hotkey::Restore:
        showNormal();
    }
  }

  return false;
}
#endif

void MainWindow::on_actionAbout_triggered(bool checked /* false */)
{
  MAboutBox(this).exec();
}

void MainWindow::on_actionAnswer_triggered(bool checked /* false */)
{
  _timeAnswer = TIME_NONE;
  showAnswer();
}

void MainWindow::on_actionFindInVocabulary_triggered(bool checked /* false */)
{
  openVocabulary(_currentRecord.vocabulary, true);
}

void MainWindow::on_actionLicense_triggered(bool checked /* false */)
{
  LicenseTextDialog licenseTextDialog(LicenseCommon::LicenseContentList(), &_settings, this);
  licenseTextDialog.exec();
}

void MainWindow::on_actionMute_toggled(bool checked)
{
  _settings.setMute(checked);
}

void MainWindow::on_actionNext_triggered(bool checked /* false */)
{
  if (_learningTimer.isActive())
  {
    _learningTimer.stop();
  }
  _timeQuestion = TIME_NOW;
  _timeAnswer   = TIME_NONE;

  _learningTimer.start(0);
}

void MainWindow::on_actionOrganizer_triggered(bool checked /* false */)
{
  VocabularyOrganizerDialog vocabularyOrganizerDialog(&_vocabularyOrganizer, this);
  if (vocabularyOrganizerDialog.exec() == QDialog::Accepted)
  {
    enableControls();
    refreshStatusBar();
    createVocabulariesMenu();
  }
}

void MainWindow::on_actionSendFeedback_triggered(bool checked /* false */) const
{
  MFeedback::createEmailForm();
}

void MainWindow::on_actionSettings_triggered(bool checked /* false */)
{
  SettingsDialog settingsDialog(&_plugins, &_settings, this);
  if (settingsDialog.exec() == QDialog::Accepted)
  {
    applySettings(false);
  }
}

void MainWindow::on_actionStart_triggered(bool checked /* false */)
{
  _timeQuestion = TIME_NOW;
  _timeAnswer   = TIME_NONE;
  _learning     = true;

  enableControls();
  _ui.category->setVisible(_settings.showCategoryName());
  _ui.recordControls->parentWidget()->setVisible(_settings.showRecordControls());

  _currentRecord.vocabulary.clear();
  _currentRecord.id = RECORD_NONE;
  _learningTimer.start(0);
}

void MainWindow::on_actionStop_triggered(bool checked /* false */)
{
  _learningTimer.stop();
  _learning = false;
  _progressBarTimer.setMaximum(1);
  _progressBarTimer.setValue(0);
  enableControls();

  _ui.language1->hide();
  _ui.window1->clear();
  _ui.language2->hide();
  _ui.window2->clear();
  _ui.category->hide();
  _ui.recordControls->parentWidget()->hide();
}

void MainWindow::on_learningTimer_timeout()
{
  if (_timeQuestion != TIME_NONE)
  {
    _timeQuestion--;
  }
  if (_timeAnswer != TIME_NONE)
  {
    _timeAnswer--;
  }

  _progressBarTimer.setValue(_progressBarTimer.value() - 1);

  if (_timeQuestion == 0)
  {
    _ui.actionNext->setEnabled(false);
    // disable answer
    _ui.actionAnswer->setEnabled(false);

    if (_vocabularyOrganizer.recordCount(true) == 0)
    {
      _ui.actionStop->trigger();
    }
    else
    {
      quintptr categoryId;
      const auto lastRecord          = _currentRecord;
      const auto maxCategoryPriority = qrand() % VocabularyTabWidget::CATEGORY_PRIORITY_MAX + 1;
      const auto maxRecordPriority   = qrand() % PriorityDelegate::RECORD_PRIORITY_MAX + 1;

      auto nextRecordTry = 0;
      forever
      {
        _currentRecord = _vocabularyOrganizer.recordInfo(qrand() % _vocabularyOrganizer.recordCount());
        if (nextRecordTry == MAX_NEXTRECORD_TRIES)
        {
          _ui.actionStop->trigger();
          return;
        }
        else
        {
          ++nextRecordTry;
        }

        if ((!_settings.learnDisabledWords() && !_currentRecord.vocabulary->recordEnabled(_currentRecord.id)) || recordPriority() > maxRecordPriority)
        {
          continue;
        }

        categoryId = _currentRecord.vocabulary->recordCategory(_currentRecord.id);
        if (_currentRecord.vocabulary->categoryEnabled(categoryId) && _currentRecord.vocabulary->categoryPriority(categoryId) <= maxCategoryPriority && (_currentRecord.vocabulary->recordCount(true) == 1 || _currentRecord.vocabulary != lastRecord.vocabulary || _currentRecord.id != lastRecord.id))
        {
          break;
        }
      }
/*#ifdef _DEBUG
      qDebug("Current word: %d", _currentRecord.id);
#endif*/

      // answer time
      _timeAnswer = _settings.waitForAnswer();
      // progress
      _progressBarTimer.setMaximum(_timeAnswer);
      _progressBarTimer.setValue(_timeAnswer);

      // question parameters
      _directionSwitched = learningDirection();

      // gui
      const auto lang1 = languageText(_directionSwitched, false);
      if (lang1.isEmpty())
      {
        _ui.language1->hide();
      }
      else
      {
        _ui.language1->setVisible(_settings.showLanguageNames());
        _ui.language1->setText(lang1);
      }
      const auto lang2 = languageText(_directionSwitched, true);
      if (lang2.isEmpty())
      {
        _ui.language2->hide();
      }
      else
      {
        _ui.language2->setVisible(_settings.showLanguageNames());
        _ui.language2->setText(lang2);
      }
      _ui.window1->setText(learningText(Template::Learning, _directionSwitched, false));
      _ui.window2->clear();
      _ui.category->setText(_currentRecord.vocabulary->name() + ", " + _currentRecord.vocabulary->categoryName(categoryId));
      setupRecordControls();

      // tray
      if (_settings.systemTrayIcon() && _settings.showWordsInTrayBalloon())
      {
        showTrayBalloon(_directionSwitched, false);
      }

      // sound
      if (_settings.newWordSound() && !_settings.mute())
      {
        if (_settings.newWordSoundType() == Settings::NewWordSoundType::System)
        {
          QApplication::beep();
        }
        else
        {
          QSound::play(_settings.newWordSoundFile());
        }
      }

      // flash
      if (_settings.newWordFlash())
      {
        const auto styleSheet = _ui.window1->styleSheet();

        for (auto flashNum = 0; flashNum < FLASH_COUNT; ++flashNum)
        {
          _ui.window1->setStyleSheet(QString("QAbstractScrollArea { background-color: %1 }").arg(_settings.colorFlash()));
          QTest::qWait(FLASH_WAIT);
          _ui.window1->setStyleSheet(styleSheet);
          if (flashNum < FLASH_COUNT - 1)
          {
            QTest::qWait(FLASH_WAIT);
          }
        }
      }

      // speech
      if (_settings.newWordSound())
      {
        QTest::qWait(SAY_BEEP_WAIT);
        say(_directionSwitched, false);
      }

      // next question time
      _timeQuestion = _settings.wordsFrequency();

      // enable answer
      _ui.actionAnswer->setEnabled(true);
    }
  }

  if (_timeAnswer == 0 && _ui.actionAnswer->isEnabled())
  {
    showAnswer();
  }

  if (!_learningTimer.isActive() && _ui.actionStop->isEnabled())
  {
    _learningTimer.start(MILISECONDS_PER_SECOND);
    _ui.actionNext->setEnabled(true);
  }
}

void MainWindow::on_menuTray_triggered(QAction *action)
{
  if (action == _traySettings)
  {
    _ui.actionSettings->trigger();
  }
  else
  {
    if (action == _trayExit)
    {
      close();
    }
  }
}

void MainWindow::on_menuVocabularies_triggered(QAction *action)
{
  const auto vocabularyIndex = action->data().toUInt();
  const auto vocabularyInfo  = _vocabularyOrganizer.vocabularyInfo(vocabularyIndex);

  openVocabulary(vocabularyInfo.vocabulary, false);
}

void MainWindow::on_priority1_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(1);
  }
}

void MainWindow::on_priority2_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(2);
  }
}

void MainWindow::on_priority3_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(3);
  }
}

void MainWindow::on_priority4_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(4);
  }
}

void MainWindow::on_priority5_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(5);
  }
}

void MainWindow::on_priority6_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(6);
  }
}

void MainWindow::on_priority7_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(7);
  }
}

void MainWindow::on_priority8_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(8);
  }
}

void MainWindow::on_priority9_clicked(bool checked /* false */)
{
  if (checked)
  {
    setRecordPriority(9);
  }
}

void MainWindow::on_recordEnabled_clicked(bool checked /* false */)
{
  setRecordEnabled(checked);
}

void MainWindow::on_trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick && isMinimized())
  {
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    showNormal();
  }
}

void MainWindow::on_updateChecker_finished()
{
  if (_updateChecker.checkResult() != QNetworkReply::NoError)
  {
    return;
  }

  if (_updateChecker.updateAvailable())
  {
    const auto text = tr("New version %1 of Vocabulary Master available at <a href=\"http://vocabulary-master.cz/\">homepage</a>.").arg(_updateChecker.updateVersion());
    QMessageBox::information(this, tr("Update available"), text);
  }
}

void MainWindow::on_vocabularyOrganizer_vocabularyClose(const QSharedPointer<Vocabulary> &vocabulary)
{
  if (_currentRecord.vocabulary == vocabulary)
  {
    _ui.actionNext->trigger();
  }
}