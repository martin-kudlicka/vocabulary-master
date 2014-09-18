#include "mainwindow.h"

#include "settingsdialog.h"
#include "vocabularymanagerdialog.h"
#include <QtCore/QTime>
#include <QtTest/QTest>
#include <QtWidgets/QMessageBox>
#ifndef EDITION_FREE
# ifdef Q_OS_WIN
#  include <qt_windows.h>
# endif
# ifndef EDITION_TRY
#  include "licensedialog.h"
# endif
#endif
#ifndef EDITION_FREE
# include <QtMultimedia/QSound>
# include "vocabularymanagerdialog/prioritydelegate.h"
#endif
#include "vocabularyorganizerdialog.h"

#ifdef EDITION_FREE
const char *EDITION_FREE_SUFFIX = QT_TRANSLATE_NOOP("MainWindow", " Free edition");
#endif
#ifdef EDITION_TRY
const char *EDITION_TRY_SUFFIX = QT_TRANSLATE_NOOP("MainWindow", " Try edition");
#endif
const char *VOCABULARY_MASTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary Master");

MainWindow::MainWindow(QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags), _learning(false),
#ifndef EDITION_FREE
	_hboxLayoutInner(NULL),
#endif
	_updateChecker(&_settings), _vocabularyOrganizer(&_settings)
{
	_learningTimer.setSingleShot(true);

	qsrand(QTime::currentTime().msec());

    // gui
	_ui.setupUi(this);
#ifdef EDITION_FREE
	_ui.recordControls->parentWidget()->hide();
	_ui.language1->hide();
	_ui.language2->hide();
	_ui.category->hide();

	_ui.actionFindInVocabulary->setVisible(false);
    _ui.actionAnswer->setVisible(false);
    _ui.actionMute->setVisible(false);
    _ui.actionLicense->setVisible(false);
	foreach (const QObject *object, _ui.toolBar->children())
	{
		const QAction *action = qobject_cast<QAction *>(object);
		if (action && action->isSeparator())
		{
			action->deleteLater();
		} // if
	} // foreach
#else
# ifdef EDITION_TRY
    _ui.actionOpen->setVisible(false);
    _ui.actionLicense->setVisible(false);
# endif
	_trayIcon.setToolTip("Vocabulary Master");
#endif
    _ui.statusBar->addWidget(&_vocabularyStatus);
    _progressBarTimer.setTextVisible(false);
    _ui.statusBar->addWidget(&_progressBarTimer, 1);

#ifndef EDITION_FREE
# ifndef EDITION_TRY
    // license
    _license = new License(&_settings);
# endif

    // plugins
	_plugins.load();
	_plugins.initialize();

    // system tray icon
    const QIcon trayIcon(":/res/mainwindow/mainwindow.png");
    _trayIcon.setIcon(trayIcon);
#endif

	// translator
	QCoreApplication::installTranslator(&_translator);

    // settings
    applySettings(true);

#ifndef EDITION_TRY
	_vocabularyOrganizer.OpenAll(this);
#endif
    refreshStatusBar();

	// menus
#ifndef EDITION_FREE
	createTrayMenu();
#endif
	createVocabulariesMenu();

    // controls
    enableControls();
#ifndef EDITION_FREE
    _ui.actionMute->setChecked(_settings.mute());
#endif

	// connections
#ifndef EDITION_FREE
	connect(&_menuTrayVocabularies, SIGNAL(triggered(QAction *)),                         SLOT(on_menuVocabularies_triggered(QAction *)));
	connect(&_trayIcon,             SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(on_trayIcon_activated(QSystemTrayIcon::ActivationReason)));
#endif
	connect(&_learningTimer,        SIGNAL(timeout()),                                    SLOT(on_learningTimer_timeout()));
	connect(&_updateChecker,        SIGNAL(Finished()),                                   SLOT(on_updateChecker_finished()));
	connect(&_vocabularyOrganizer,  SIGNAL(VocabularyClose(const Vocabulary *)),          SLOT(on_vocabularyOrganizer_vocabularyClose(const Vocabulary *)));

#ifndef EDITION_FREE
	// learning
	if (
# ifndef EDITION_TRY
		_license->isLoaded() &&
# endif
		_settings.startLearningOnStartup() && _vocabularyOrganizer.IsOpen())
	{
		on_actionStart_triggered();
	} // if
#endif

	// update check
	if (_settings.updateCheck())
	{
		_updateChecker.checkForUpdate();
	} // if
} // MainWindow

MainWindow::~MainWindow()
{
#ifndef EDITION_FREE
	_settings.setWindowX(geometry().x());
	_settings.setWindowY(geometry().y());
	_settings.setWindowHeight(geometry().height());
	_settings.setWindowWidth(geometry().width());

	_plugins.uninitialize();
# ifndef EDITION_TRY
    delete _license;
# endif
#endif
} // ~MainWindow

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
            } // if
		} // if
	} // if
#ifndef EDITION_FREE
	_plugins.setLanguage(_settings.translation());

	_ui.toolBar->setVisible(_settings.showToolBar());
	_ui.language1->setVisible(_learning && _settings.showLanguageNames());
	_ui.language2->setVisible(_learning && _settings.showLanguageNames());
	_ui.category->setVisible(_learning && _settings.showCategoryName());
	_ui.recordControls->parentWidget()->setVisible(_learning && _settings.showRecordControls());
	_ui.statusBar->setVisible(_settings.showStatusBar());
#endif

    if (_settings.alwaysOnTop())
	{
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }
	else
	{
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    } // if else
#ifndef EDITION_FREE
    if (startup && _settings.windowX() != Settings::DEFAULT_DIMENSION)
	{
        setGeometry(_settings.windowX(), _settings.windowY(), _settings.windowWidth(), _settings.windowHeight());
    } // if
#endif
    show();

#ifndef EDITION_FREE
    _trayIcon.setVisible(_settings.systemTrayIcon());
#endif

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
	registerHotkeys();
#endif
} // applySettings

#ifndef EDITION_FREE
void MainWindow::createTrayMenu()
{
    _trayVocabularies = _menuTray.addAction(QIcon(":/res/mainwindow/menubar/manage.png"), tr("&Vocabularies"));
	_trayVocabularies->setMenu(&_menuTrayVocabularies);
    _traySettings = _menuTray.addAction(QIcon(":/res/mainwindow/menubar/settings.png"), tr("&Settings"));
    _menuTray.addSeparator();
	_trayExit = _menuTray.addAction(QIcon(":/res/mainwindow/menubar/exit.png"), tr("&Exit"));

	connect(&_menuTray, SIGNAL(triggered(QAction *)), SLOT(on_menuTray_triggered(QAction *)));
	_trayIcon.setContextMenu(&_menuTray);
} // createTrayMenu
#endif

void MainWindow::createVocabulariesMenu()
{
	_ui.menuVocabularies->clear();
#ifndef EDITION_FREE
	_menuTrayVocabularies.clear();
#endif

	for (quint8 vocabularyIndex = 0; vocabularyIndex < _vocabularyOrganizer.GetVocabularyCount(); vocabularyIndex++)
	{
		const VocabularyOrganizer::VocabularyInfo vocabularyInfo = _vocabularyOrganizer.GetVocabularyInfo(vocabularyIndex);
		const QString name                                       = vocabularyInfo.vVocabulary->GetName();

		// main menu
		QAction *action = _ui.menuVocabularies->addAction(QIcon(":/res/mainwindow/menubar/vocabulary.png"), name);
		action->setData(vocabularyIndex);

#ifndef EDITION_FREE
		// tray menu
		action = _menuTrayVocabularies.addAction(QIcon(":/res/mainwindow/menubar/vocabulary.png"), name);
		action->setData(vocabularyIndex);
#endif
	} // for
} // createVocabulariesMenu

void MainWindow::enableControls()
{
	// menu
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	_ui.actionOrganizer->setEnabled(_license->isLoaded());
#endif
    _ui.menuVocabularies->setEnabled(
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
		_license->isLoaded() &&
#endif
		_vocabularyOrganizer.IsOpen());
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	_ui.menuOptions->setEnabled(_license->isLoaded());
#endif

	// tool bar
	_ui.actionStart->setEnabled(
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
		_license->isLoaded() &&
#endif
		_vocabularyOrganizer.IsOpen() && !_learning && _vocabularyOrganizer.GetRecordCount(true) > 0);
	_ui.actionStop->setEnabled(_learning);
	_ui.actionNext->setEnabled(_learning);
#ifndef EDITION_FREE
	_ui.actionFindInVocabulary->setEnabled(_learning);
    _ui.actionAnswer->setEnabled(_learning && _timeAnswer >= TIME_NOW);

    // tray
    _trayVocabularies->setEnabled(_vocabularyOrganizer.IsOpen());
#endif
} // enableControls

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
                    category  = _ui.category->text();
                } // if
                _ui.retranslateUi(this);
                if (_learning)
				{
                    _ui.language1->setText(language1);
                    _ui.language2->setText(language2);
                    _ui.category->setText(category);
                } // if
            }
#ifdef EDITION_FREE
            setWindowTitle(windowTitle() + tr(EDITION_FREE_SUFFIX));
#endif
#ifdef EDITION_TRY
            setWindowTitle(windowTitle() + tr(EDITION_TRY_SUFFIX));
#endif
            break;
#ifndef EDITION_FREE
        case QEvent::WindowStateChange:
		    if (isMinimized() && _settings.systemTrayIcon() && _settings.minimizeToTray())
			{
			    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint); // just add some flag to hide window
		    } // if
#endif
	} // switch

	return QMainWindow::event(event);
} // event

QString MainWindow::languageText(bool directionSwitched, bool answer) const
{
    if ((!directionSwitched && !answer) || (directionSwitched && answer))
	{
		return _currentRecord.vVocabulary->GetLanguageName(VocabularyDatabase::FieldLanguageLeft);
    }
	else
	{
        return _currentRecord.vVocabulary->GetLanguageName(VocabularyDatabase::FieldLanguageRight);
    } // if else
} // languageText

bool MainWindow::learningDirection() const
{
	if (_settings.switchLearningDirection() != Qt::PartiallyChecked)
	{
		return _settings.switchLearningDirection();
	}
	else
	{
		return qrand() % 2;
	} // if else
} // learningDirection

QString MainWindow::learningText(Template templateType, bool directionSwitched, bool answer) const
{
	QString templateText;
	VocabularyDatabase::FieldLanguage fieldLanguage;
	if ((!directionSwitched && !answer) || (directionSwitched && answer))
	{
		if (templateType == TemplateLearning)
		{
			templateText = _currentRecord.vVocabulary->GetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageLeft);
		}
#ifndef EDITION_FREE
		else
		{
			templateText = _currentRecord.vVocabulary->GetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageLeft);
		} // if else
#endif
		fieldLanguage = VocabularyDatabase::FieldLanguageLeft;
	}
	else
	{
		if (templateType == TemplateLearning)
		{
			templateText = _currentRecord.vVocabulary->GetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageRight);
		}
#ifndef EDITION_FREE
		else
		{
			templateText = _currentRecord.vVocabulary->GetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageRight);
		} // if else
#endif
		fieldLanguage = VocabularyDatabase::FieldLanguageRight;
	} // if else

	// substitute variables in template
	foreach (const quint8 &fieldId, _currentRecord.vVocabulary->fieldIds())
	{
		if (_currentRecord.vVocabulary->fieldLanguage(fieldId) == fieldLanguage) {
			const QString data = _currentRecord.vVocabulary->dataText(_currentRecord.iId, fieldId);

			QString field = _currentRecord.vVocabulary->fieldTemplateName(fieldId);
			templateText.replace(VARIABLE_MARK + field, data);
		} // if
	} // foreach

	return templateText;
} // learningText

void MainWindow::openVocabulary(Vocabulary *vocabulary
#ifndef EDITION_FREE
    , bool currentRecord
#endif
    )
{
    VocabularyManagerDialog vocabularyManagerDialog(vocabulary,
#ifndef EDITION_FREE
        &_settings,
        &_plugins,
#endif
        this);
#ifndef EDITION_FREE
    if (currentRecord)
	{
        vocabularyManagerDialog.ExecOnRecord(_currentRecord.iId);
    }
	else
	{
#endif
        vocabularyManagerDialog.exec();
#ifndef EDITION_FREE
    } // if else
#endif

	_ui.actionStart->setEnabled(_vocabularyOrganizer.IsOpen() && !_learning && _vocabularyOrganizer.GetRecordCount() > 0);
    refreshStatusBar();
} // openVocabulary

#ifndef EDITION_FREE
quint8 MainWindow::recordPriority() const
{
    foreach (const quint8 &fieldId, _currentRecord.vVocabulary->fieldIds())
	{
        if (_currentRecord.vVocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
            const VocabularyDatabase::FieldBuiltIn builtInType = _currentRecord.vVocabulary->fieldBuiltIn(fieldId);
            switch (builtInType)
			{
                case VocabularyDatabase::FieldBuiltInPriority:
                    return _currentRecord.vVocabulary->dataText(_currentRecord.iId, fieldId).toInt();
            } // switch
        } // if
    } // foreach

    return PriorityDelegate::RECORD_PRIORITY_MIN;
} // recordPriority
#endif

void MainWindow::refreshStatusBar()
{
    if (!_vocabularyOrganizer.IsOpen())
	{
        _vocabularyStatus.setText(tr("no vocabulary"));
    }
	else
	{
#ifdef EDITION_FREE
		const QString count = QString("%1").arg(_vocabularyOrganizer.GetRecordCount());
#else
		const QString count = QString("%1/%2").arg(_vocabularyOrganizer.GetRecordCount(true)).arg(_vocabularyOrganizer.GetRecordCount());
#endif
		_vocabularyStatus.setText(tr("%1 voc., %2 records").arg(_vocabularyOrganizer.GetVocabularyCount()).arg(count));
    } // if else
} // refreshStatusBar

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
void MainWindow::registerHotkeys() const
{
	for (quint8 hotkeyIndex = 0; hotkeyIndex < Settings::HotkeyCount - 1; hotkeyIndex++)
	{
		const Settings::HotkeyInfo hotkeyInfo = _settings.hotkey(static_cast<Settings::Hotkey>(hotkeyIndex));

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
			} // if else
			if (hotkeyInfo.text.contains(MODIFIER_CTRL))
			{
				modifiers |= MOD_CONTROL;
			} // if
			if (hotkeyInfo.text.contains(MODIFIER_SHIFT))
			{
				modifiers |= MOD_SHIFT;
			} // if

			RegisterHotKey(reinterpret_cast<HWND>(winId()), hotkeyIndex, modifiers, hotkeyInfo.virtualKey);
		} // if else
	} // for
} // registerHotkeys
#endif

#ifndef EDITION_FREE
void MainWindow::say(bool directionSwitched, bool answer) const
{
    if (!_settings.mute())
	{
	    VocabularyDatabase::FieldLanguage fieldLanguage;
	    if ((!directionSwitched && !answer) || (directionSwitched && answer))
		{
		    fieldLanguage = VocabularyDatabase::FieldLanguageLeft;
	    }
		else
		{
		    fieldLanguage = VocabularyDatabase::FieldLanguageRight;
	    } // if else

        // get text to speech
        QString text;
        foreach (const quint8 &fieldId, _currentRecord.vVocabulary->fieldIds())
		{
            if (_currentRecord.vVocabulary->fieldLanguage(fieldId) == fieldLanguage && _currentRecord.vVocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeSpeech))
			{
                if (!text.isEmpty())
				{
                    text += ' ';
                } // if
                text += _currentRecord.vVocabulary->dataText(_currentRecord.iId, fieldId);
            } // if
        } // foreach

        if (!text.isEmpty())
		{
			const TTSInterface::TTSPlugin speechPlugin = _currentRecord.vVocabulary->GetLanguageSpeech(fieldLanguage);
	        if (speechPlugin != TTSInterface::TTPluginNone)
			{
		        TTSInterface *speechInterface = _plugins.ttsPlugin(speechPlugin);
                if (speechInterface)
				{
					const QString voice = _currentRecord.vVocabulary->GetLanguageVoice(fieldLanguage);
		            speechInterface->say(voice, text);
                } // if
	        } // if
        } // if
    } // if
} // say
#endif

void MainWindow::setLayout()
{
#ifndef EDITION_FREE
    if (_hboxLayoutInner)
	{
        _hboxLayoutInner->deleteLater();
        _hboxLayoutInner = NULL;
    } // if
#endif
    if (_ui.centralWidget->layout())
	{
        delete _ui.centralWidget->layout();
    } // if

    QVBoxLayout *mainLayout = new QVBoxLayout(_ui.centralWidget);
#ifndef EDITION_FREE
    if (_settings.horizontalLayout())
	{
        mainLayout->addWidget(_ui.category);
		mainLayout->addWidget(_ui.recordControls->parentWidget());
        QHBoxLayout *innerLayout = new QHBoxLayout;
        innerLayout->addWidget(_ui.questionLayout->parentWidget());
        innerLayout->addWidget(_ui.answerLayout->parentWidget());
        mainLayout->addLayout(innerLayout);
    }
	else
	{
#endif
        mainLayout->addWidget(_ui.questionLayout->parentWidget());
        mainLayout->addWidget(_ui.category);
#ifndef EDITION_FREE
		mainLayout->addWidget(_ui.recordControls->parentWidget());
#endif
        mainLayout->addWidget(_ui.answerLayout->parentWidget());
#ifndef EDITION_FREE
    } // if else
#endif
} // setLayout

#ifndef EDITION_FREE
void MainWindow::setRecordEnabled(bool enabled)
{
	foreach (quint8 fieldId, _currentRecord.vVocabulary->fieldIds())
	{
		if (_currentRecord.vVocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			const VocabularyDatabase::FieldBuiltIn builtInType = _currentRecord.vVocabulary->fieldBuiltIn(fieldId);
			switch (builtInType)
			{
				case VocabularyDatabase::FieldBuiltInEnabled:
					_currentRecord.vVocabulary->setDataText(_currentRecord.iId, fieldId, QString::number(enabled ? Qt::Checked : Qt::Unchecked));
					return;
			} // switch
		} // if
	} // foreach
} // setRecordEnabled

void MainWindow::setRecordPriority(quint8 priority)
{
	foreach (quint8 fieldId, _currentRecord.vVocabulary->fieldIds())
	{
		if (_currentRecord.vVocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			const VocabularyDatabase::FieldBuiltIn builtInType = _currentRecord.vVocabulary->fieldBuiltIn(fieldId);
			switch (builtInType)
			{
				case VocabularyDatabase::FieldBuiltInPriority:
					_currentRecord.vVocabulary->setDataText(_currentRecord.iId, fieldId, QString::number(priority));
					return;
			} // switch
		} // if
	} // foreach
} // setRecordPriority

void MainWindow::setupRecordControls() const
{
	const quint8 priority = recordPriority();
	_ui.priority1->setChecked(priority == 1);
	_ui.priority2->setChecked(priority == 2);
	_ui.priority3->setChecked(priority == 3);
	_ui.priority4->setChecked(priority == 4);
	_ui.priority5->setChecked(priority == 5);
	_ui.priority6->setChecked(priority == 6);
	_ui.priority7->setChecked(priority == 7);
	_ui.priority8->setChecked(priority == 8);
	_ui.priority9->setChecked(priority == 9);
	_ui.recordEnabled->setChecked(_currentRecord.vVocabulary->recordEnabled(_currentRecord.iId));
} // setupRecordControls
#endif

void MainWindow::showAnswer()
{
#ifndef EDITION_FREE
    // answer
    _ui.actionAnswer->setEnabled(false);
#endif

    // gui
    _ui.window2->setText(learningText(TemplateLearning, _directionSwitched, true));
    _ui.window2->repaint();

#ifndef EDITION_FREE
    // tray
    if (_settings.systemTrayIcon() && _settings.showWordsInTrayBalloon())
	{
        showTrayBalloon(_directionSwitched, true);
    } // if

    // speech
    say(_directionSwitched, true);
#endif

    // progress
    _progressBarTimer.setMaximum(_timeQuestion);
    _progressBarTimer.setValue(_timeQuestion);
} // showAnswer

#ifndef EDITION_FREE
void MainWindow::showTrayBalloon(bool directionSwitched, bool answer)
{
	QString text = learningText(TemplateTray, directionSwitched, false);
	if (answer)
	{
		text += " -> " + learningText(TemplateTray, directionSwitched, true);
	} // if

	_trayIcon.showMessage(tr(VOCABULARY_MASTER), text);
} // showTrayBalloon
#endif

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
bool MainWindow::winEvent(MSG *message, long *result)
{
	if (message->message == WM_HOTKEY)
	{
		switch (message->wParam)
		{
			case Settings::HotkeyAnswer:
				if (_ui.actionAnswer->isEnabled())
				{
					on_actionAnswer_triggered();
				} // if
				break;
			case Settings::HotkeyMinimize:
				showMinimized();
				break;
			case Settings::HotkeyNext:
				if (_ui.actionNext->isEnabled())
				{
					on_actionNext_triggered();
				} // if
				break;
			case Settings::HotkeyRestore:
				showNormal();
		} // switch
	} // if

	return false;
} // winEvent
#endif

void MainWindow::on_actionAbout_triggered(bool checked /* false */)
{
    QMessageBox::about(this, tr("About Vocabulary Master"), "<center><b>" + tr(VOCABULARY_MASTER)
#ifdef EDITION_FREE
        + tr(EDITION_FREE_SUFFIX)
#endif
#ifdef EDITION_TRY
        + tr(EDITION_TRY_SUFFIX)
#endif
        + "</b></center><center>Version " + _updateChecker.currentVersion() + "</center><br />Copyright (C) 2011 Isshou");
} // on_actionAbout_triggered

#ifndef EDITION_FREE
void MainWindow::on_actionAnswer_triggered(bool checked /* false */)
{
    _timeAnswer = TIME_NONE;
    showAnswer();
} // on_actionAnswer_triggered

void MainWindow::on_actionFindInVocabulary_triggered(bool checked /* false */)
{
    openVocabulary(_currentRecord.vVocabulary, true);
} // on_actionFindInVocabulary_triggered

# ifndef EDITION_TRY
void MainWindow::on_actionLicense_triggered(bool checked /* false */)
{
    LicenseDialog ldLicenseDialog(_license, &_settings, this);
	if (ldLicenseDialog.exec() == QDialog::Accepted)
	{
		applySettings(false);
		enableControls();
	} // if
} // on_actionLicense_triggered
# endif

void MainWindow::on_actionMute_toggled(bool checked)
{
    _settings.setMute(checked);
} // on_actionMute_toggled
#endif

void MainWindow::on_actionNext_triggered(bool checked /* false */)
{
	if (_learningTimer.isActive())
	{
		_learningTimer.stop();
	} // if
	_timeQuestion = TIME_NOW;
    _timeAnswer   = TIME_NONE;

	_learningTimer.start(0);
} // on_actionNext_triggered

void MainWindow::on_actionOrganizer_triggered(bool checked /* false */)
{
	VocabularyOrganizerDialog vodOrganizerDialog(&_vocabularyOrganizer, this);
	if (vodOrganizerDialog.exec() == QDialog::Accepted)
	{
		enableControls();
		refreshStatusBar();
		createVocabulariesMenu();
	} // if
} // on_actionOrganizer_triggered

void MainWindow::on_actionSettings_triggered(bool checked /* false */)
{
	SettingsDialog sdDialog(
#ifndef EDITION_FREE
        &_plugins,
#endif
        &_settings, this);
    if (sdDialog.exec() == QDialog::Accepted)
	{
        applySettings(false);
    } // if
} // on_actionSettings_triggered

void MainWindow::on_actionStart_triggered(bool checked /* false */)
{
    _timeQuestion = TIME_NOW;
    _timeAnswer   = TIME_NONE;
	_learning     = true;

	enableControls();
#ifdef EDITION_FREE
	_ui.category->show();
#else
    _ui.category->setVisible(_settings.showCategoryName());
	_ui.recordControls->parentWidget()->setVisible(_settings.showRecordControls());
#endif

	_currentRecord.vVocabulary = NULL;
	_currentRecord.iId         = RECORD_NONE;
	_learningTimer.start(0);
} // on_actionStart_triggered

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
#ifndef EDITION_FREE
	_ui.recordControls->parentWidget()->hide();
#endif
} // on_actionStop_triggered

void MainWindow::on_learningTimer_timeout()
{
    if (_timeQuestion != TIME_NONE)
	{
        _timeQuestion--;
    } // if
    if (_timeAnswer != TIME_NONE)
	{
        _timeAnswer--;
    } // if

    _progressBarTimer.setValue(_progressBarTimer.value() - 1);

    if (_timeQuestion == 0)
	{
		_ui.actionNext->setEnabled(false);
#ifndef EDITION_FREE
		// disable answer
		_ui.actionAnswer->setEnabled(false);
#endif

        if (_vocabularyOrganizer.GetRecordCount(true) == 0)
		{
            on_actionStop_triggered();
        }
		else
		{
            quint8 categoryId;
			const VocabularyOrganizer::RecordInfo lastRecord = _currentRecord;
#ifndef EDITION_FREE
			const quint8 maxCategoryPriority                 = qrand() % VocabularyTabWidget::CATEGORY_PRIORITY_MAX + 1;
            const quint8 maxRecordPriority                   = qrand() % PriorityDelegate::RECORD_PRIORITY_MAX + 1;

			quint16 nextRecordTry                            = 0;
#endif
            while (true)
			{
	            _currentRecord = _vocabularyOrganizer.GetRecordInfo(qrand() % _vocabularyOrganizer.GetRecordCount());
#ifndef EDITION_FREE
				if (nextRecordTry == MAX_NEXTRECORD_TRIES)
				{
					on_actionStop_triggered(false);
					return;
				}
				else
				{
					nextRecordTry++;
				} // if else

				if ((!_settings.learnDisabledWords() && !_currentRecord.vVocabulary->recordEnabled(_currentRecord.iId)) || recordPriority() > maxRecordPriority)
				{
					continue;
				} // if
#endif

                categoryId = _currentRecord.vVocabulary->recordCategory(_currentRecord.iId);
#ifndef EDITION_FREE
                if (_currentRecord.vVocabulary->GetCategoryEnabled(categoryId) && _currentRecord.vVocabulary->GetCategoryPriority(categoryId) <= maxCategoryPriority  && (_currentRecord.vVocabulary->recordCount(true) == 1 || _currentRecord.vVocabulary != lastRecord.vVocabulary || _currentRecord.iId != lastRecord.iId))
				{
#endif
                    break;
#ifndef EDITION_FREE
                } // if
#endif
            } // while
/*#ifdef _DEBUG
            qDebug("Current word: %d", _currentRecord.iId);
#endif*/

			// answer time
			_timeAnswer = _settings.waitForAnswer();
			// progress
			_progressBarTimer.setMaximum(_timeAnswer);
			_progressBarTimer.setValue(_timeAnswer);

            // question parameters
	        _directionSwitched = learningDirection();

            // gui
		    const QString lang1 = languageText(_directionSwitched, false);
		    if (lang1.isEmpty())
			{
			    _ui.language1->hide();
		    }
			else
			{
#ifdef EDITION_FREE
				_ui.language1->show();
#else
			    _ui.language1->setVisible(_settings.showLanguageNames());
#endif
			    _ui.language1->setText(lang1);
		    } // if else
		    const QString lang2 = languageText(_directionSwitched, true);
		    if (lang2.isEmpty())
			{
			    _ui.language2->hide();
		    }
			else
			{
#ifdef EDITION_FREE
				_ui.language2->show();
#else
			    _ui.language2->setVisible(_settings.showLanguageNames());
#endif
			    _ui.language2->setText(lang2);
		    } // if else
	        _ui.window1->setText(learningText(TemplateLearning, _directionSwitched, false));
	        _ui.window2->clear();
            _ui.category->setText(_currentRecord.vVocabulary->GetName() + ", " + _currentRecord.vVocabulary->GetCategoryName(categoryId));
#ifndef EDITION_FREE
			setupRecordControls();
#endif

#ifndef EDITION_FREE
		    // tray
		    if (_settings.systemTrayIcon() && _settings.showWordsInTrayBalloon())
			{
			    showTrayBalloon(_directionSwitched, false);
		    } // if

            // sound
	        if (_settings.newWordSound() && !_settings.mute())
			{
                if (_settings.newWordSoundType() == Settings::NewWordSoundTypeSystem)
				{
		            QApplication::beep();
                }
				else
				{
                    QSound::play(_settings.newWordSoundFile());
                } // if else
	        } // if

            // flash
	        if (_settings.newWordFlash())
			{
                const QString styleSheet = _ui.window1->styleSheet();

		        for (quint8 flashNum = 0; flashNum < FLASH_COUNT; flashNum++)
				{
                    _ui.window1->setStyleSheet(QString("QAbstractScrollArea { background-color: %1 }").arg(_settings.colorFlash()));
			        QTest::qWait(FLASH_WAIT);
                    _ui.window1->setStyleSheet(styleSheet);
			        if (flashNum < FLASH_COUNT - 1)
					{
				        QTest::qWait(FLASH_WAIT);
			        } // if
		        } // for
	        } // if

            // speech
            if (_settings.newWordSound())
			{
                QTest::qWait(SAY_BEEP_WAIT);
                say(_directionSwitched, false);
            } // if
#endif

            // next question time
            _timeQuestion = _settings.wordsFrequency();

#ifndef EDITION_FREE
            // enable answer
            _ui.actionAnswer->setEnabled(true);
#endif
        } // if else
    } // if

    if (_timeAnswer == 0 && _ui.actionStop->isEnabled())
	{
        showAnswer();
    } // if else

	if (!_learningTimer.isActive() && _ui.actionStop->isEnabled())
	{
		_learningTimer.start(MILISECONDS_PER_SECOND);
		_ui.actionNext->setEnabled(true);
	} // if
} // on_learningTimer_timeout

#ifndef EDITION_FREE
void MainWindow::on_menuTray_triggered(QAction *action)
{
    if (action == _traySettings)
	{
        on_actionSettings_triggered();
    }
	else
	{
        if (action == _trayExit)
		{
	        close();
        } // if
    } // if else
} // on_menuTray_triggered
#endif

void MainWindow::on_menuVocabularies_triggered(QAction *action)
{
	const quint8 vocabularyIndex                             = action->data().toUInt();
	const VocabularyOrganizer::VocabularyInfo vocabularyInfo = _vocabularyOrganizer.GetVocabularyInfo(vocabularyIndex);

	openVocabulary(vocabularyInfo.vVocabulary
#ifndef EDITION_FREE
		, false
#endif
		);
} // on_menuVocabularies_triggered

#ifndef EDITION_FREE
void MainWindow::on_priority1_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(1);
	} // if
} // on_priority1_clicked

void MainWindow::on_priority2_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(2);
	} // if
} // on_priority2_clicked

void MainWindow::on_priority3_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(3);
	} // if
} // on_priority3_clicked

void MainWindow::on_priority4_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(4);
	} // if
} // on_priority4_clicked

void MainWindow::on_priority5_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(5);
	} // if
} // on_priority5_clicked

void MainWindow::on_priority6_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(6);
	} // if
} // on_priority6_clicked

void MainWindow::on_priority7_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(7);
	} // if
} // on_priority7_clicked

void MainWindow::on_priority8_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(8);
	} // if
} // on_priority8_clicked

void MainWindow::on_priority9_clicked(bool checked /* false */)
{
	if (checked)
	{
		setRecordPriority(9);
	} // if
} // on_priority9_clicked

void MainWindow::on_recordEnabled_clicked(bool checked /* false */)
{
	setRecordEnabled(checked);
} // on_recordEnabled_clicked

void MainWindow::on_trayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick && isMinimized())
	{
		setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
		showNormal();
	} // if
} // on_trayIcon_activated
#endif

void MainWindow::on_updateChecker_finished()
{
	if (_updateChecker.checkResult() != QNetworkReply::NoError)
	{
		return;
	} // if

	if (_updateChecker.updateAvailable())
	{
		const QString text = tr("New version %1 of Vocabulary Master available at <a href=\"http://vocabulary-master.cz/\">homepage</a>.").arg(_updateChecker.updateVersion());
		QMessageBox::information(this, tr("Update available"), text);
	} // if
} // on_updateChecker_finished

void MainWindow::on_vocabularyOrganizer_vocabularyClose(const Vocabulary *vocabulary)
{
	if (_currentRecord.vVocabulary == vocabulary)
	{
		on_actionNext_triggered();
	} // if
} // on_vocabularyOrganizer_vocabularyClose