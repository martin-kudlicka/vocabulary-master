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

MainWindow::MainWindow(QWidget *parent /* nullptr */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags), _learning(false),
#ifndef EDITION_FREE
	_hboxLayoutInner(nullptr),
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
	for (const QObject *object : _ui.toolBar->children())
	{
		const QAction *action = qobject_cast<QAction *>(object);
		if (action && action->isSeparator())
		{
			action->deleteLater();
		}
	}
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
	_vocabularyOrganizer.openAll(this);
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
	connect(&_vocabularyOrganizer,  SIGNAL(vocabularyClose(const Vocabulary *)),          SLOT(on_vocabularyOrganizer_vocabularyClose(const Vocabulary *)));

#ifndef EDITION_FREE
	// learning
	if (
# ifndef EDITION_TRY
		_license->isLoaded() &&
# endif
		_settings.startLearningOnStartup() && _vocabularyOrganizer.isOpen())
	{
		on_actionStart_triggered();
	}
#endif

	// update check
	if (_settings.updateCheck())
	{
		_updateChecker.checkForUpdate();
	}
}

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
    }
#ifndef EDITION_FREE
    if (startup && _settings.windowX() != Settings::DEFAULT_DIMENSION)
	{
        setGeometry(_settings.windowX(), _settings.windowY(), _settings.windowWidth(), _settings.windowHeight());
    }
#endif
    show();

#ifndef EDITION_FREE
    _trayIcon.setVisible(_settings.systemTrayIcon());
#endif

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
	registerHotkeys();
#endif
}

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
}
#endif

void MainWindow::createVocabulariesMenu()
{
	_ui.menuVocabularies->clear();
#ifndef EDITION_FREE
	_menuTrayVocabularies.clear();
#endif

	for (quint8 vocabularyIndex = 0; vocabularyIndex < _vocabularyOrganizer.vocabularyCount(); vocabularyIndex++)
	{
		const VocabularyOrganizer::VocabularyInfo vocabularyInfo = _vocabularyOrganizer.vocabularyInfo(vocabularyIndex);
		const QString name                                       = vocabularyInfo.vocabulary->name();

		// main menu
		QAction *action = _ui.menuVocabularies->addAction(QIcon(":/res/mainwindow/menubar/vocabulary.png"), name);
		action->setData(vocabularyIndex);

#ifndef EDITION_FREE
		// tray menu
		action = _menuTrayVocabularies.addAction(QIcon(":/res/mainwindow/menubar/vocabulary.png"), name);
		action->setData(vocabularyIndex);
#endif
	}
}

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
		_vocabularyOrganizer.isOpen());
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	_ui.menuOptions->setEnabled(_license->isLoaded());
#endif

	// tool bar
	_ui.actionStart->setEnabled(
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
		_license->isLoaded() &&
#endif
		_vocabularyOrganizer.isOpen() && !_learning && _vocabularyOrganizer.recordCount(true) > 0);
	_ui.actionStop->setEnabled(_learning);
	_ui.actionNext->setEnabled(_learning);
#ifndef EDITION_FREE
	_ui.actionFindInVocabulary->setEnabled(_learning);
    _ui.actionAnswer->setEnabled(_learning && _timeAnswer >= TIME_NOW);

    // tray
    _trayVocabularies->setEnabled(_vocabularyOrganizer.isOpen());
#endif
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
                    category  = _ui.category->text();
                }
                _ui.retranslateUi(this);
                if (_learning)
				{
                    _ui.language1->setText(language1);
                    _ui.language2->setText(language2);
                    _ui.category->setText(category);
                }
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
		    }
#endif
	}

	return QMainWindow::event(event);
}

QString MainWindow::languageText(bool directionSwitched, bool answer) const
{
    if ((!directionSwitched && !answer) || (directionSwitched && answer))
	{
		return _currentRecord.vocabulary->languageName(VocabularyDatabase::FieldLanguageLeft);
    }
	else
	{
        return _currentRecord.vocabulary->languageName(VocabularyDatabase::FieldLanguageRight);
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
		if (templateType == TemplateLearning)
		{
			templateText = _currentRecord.vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguageLeft);
		}
#ifndef EDITION_FREE
		else
		{
			templateText = _currentRecord.vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguageLeft);
		}
#endif
		fieldLanguage = VocabularyDatabase::FieldLanguageLeft;
	}
	else
	{
		if (templateType == TemplateLearning)
		{
			templateText = _currentRecord.vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguageRight);
		}
#ifndef EDITION_FREE
		else
		{
			templateText = _currentRecord.vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguageRight);
		}
#endif
		fieldLanguage = VocabularyDatabase::FieldLanguageRight;
	}

	// substitute variables in template
	for (quint8 fieldId : _currentRecord.vocabulary->fieldIds())
	{
		if (_currentRecord.vocabulary->fieldLanguage(fieldId) == fieldLanguage)
		{
			const QString data = _currentRecord.vocabulary->dataText(_currentRecord.id, fieldId);

			QString field = _currentRecord.vocabulary->fieldTemplateName(fieldId);
			templateText.replace(VARIABLE_MARK + field, data);
		}
	}

	return templateText;
}

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	const LPMSG msg = static_cast<LPMSG>(message);
	if (msg->message == WM_HOTKEY)
	{
		switch (msg->wParam)
		{
			case Settings::HotkeyAnswer:
				if (_ui.actionAnswer->isEnabled())
				{
					on_actionAnswer_triggered();
				}
				break;
			case Settings::HotkeyMinimize:
				showMinimized();
				break;
			case Settings::HotkeyNext:
				if (_ui.actionNext->isEnabled())
				{
					on_actionNext_triggered();
				}
				break;
			case Settings::HotkeyRestore:
				showNormal();
		}
	}

	return false;
}
#endif

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
        vocabularyManagerDialog.execOnRecord(_currentRecord.id);
    }
	else
	{
#endif
        vocabularyManagerDialog.exec();
#ifndef EDITION_FREE
    }
#endif

	_ui.actionStart->setEnabled(_vocabularyOrganizer.isOpen() && !_learning && _vocabularyOrganizer.recordCount() > 0);
    refreshStatusBar();
}

#ifndef EDITION_FREE
quint8 MainWindow::recordPriority() const
{
    for (quint8 fieldId : _currentRecord.vocabulary->fieldIds())
	{
        if (_currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
            const VocabularyDatabase::FieldBuiltIn builtInType = _currentRecord.vocabulary->fieldBuiltIn(fieldId);
            switch (builtInType)
			{
                case VocabularyDatabase::FieldBuiltInPriority:
                    return _currentRecord.vocabulary->dataText(_currentRecord.id, fieldId).toUInt();
            }
        }
    }

    return PriorityDelegate::RECORD_PRIORITY_MIN;
}
#endif

void MainWindow::refreshStatusBar()
{
    if (!_vocabularyOrganizer.isOpen())
	{
        _vocabularyStatus.setText(tr("no vocabulary"));
    }
	else
	{
#ifdef EDITION_FREE
		const QString count = QString("%1").arg(_vocabularyOrganizer.recordCount());
#else
		const QString count = QString("%1/%2").arg(_vocabularyOrganizer.recordCount(true)).arg(_vocabularyOrganizer.recordCount());
#endif
		_vocabularyStatus.setText(tr("%1 voc., %2 records").arg(_vocabularyOrganizer.vocabularyCount()).arg(count));
    }
}

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
	    }

        // get text to speech
        QString text;
        for (quint8 fieldId : _currentRecord.vocabulary->fieldIds())
		{
            if (_currentRecord.vocabulary->fieldLanguage(fieldId) == fieldLanguage && _currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeSpeech))
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
			const TTSInterface::TTSPlugin speechPlugin = _currentRecord.vocabulary->languageSpeech(fieldLanguage);
	        if (speechPlugin != TTSInterface::TTSPlugin::None)
			{
		        TTSInterface *speechInterface = _plugins.ttsPlugin(speechPlugin);
                if (speechInterface)
				{
					const QString voice = _currentRecord.vocabulary->languageVoice(fieldLanguage);
		            speechInterface->say(voice, text);
                }
	        }
        }
    }
}
#endif

void MainWindow::setLayout()
{
#ifndef EDITION_FREE
    if (_hboxLayoutInner)
	{
        _hboxLayoutInner->deleteLater();
        _hboxLayoutInner = nullptr;
    }
#endif
    if (_ui.centralWidget->layout())
	{
        delete _ui.centralWidget->layout();
    }

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
    }
#endif
}

#ifndef EDITION_FREE
void MainWindow::setRecordEnabled(bool enabled)
{
	for (quint8 fieldId : _currentRecord.vocabulary->fieldIds())
	{
		if (_currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			const VocabularyDatabase::FieldBuiltIn builtInType = _currentRecord.vocabulary->fieldBuiltIn(fieldId);
			switch (builtInType)
			{
				case VocabularyDatabase::FieldBuiltInEnabled:
					_currentRecord.vocabulary->setDataText(_currentRecord.id, fieldId, QString::number(enabled ? Qt::Checked : Qt::Unchecked));
					return;
			}
		}
	}
}

void MainWindow::setRecordPriority(quint8 priority)
{
	for (quint8 fieldId : _currentRecord.vocabulary->fieldIds())
	{
		if (_currentRecord.vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			const VocabularyDatabase::FieldBuiltIn builtInType = _currentRecord.vocabulary->fieldBuiltIn(fieldId);
			switch (builtInType)
			{
				case VocabularyDatabase::FieldBuiltInPriority:
					_currentRecord.vocabulary->setDataText(_currentRecord.id, fieldId, QString::number(priority));
					return;
			}
		}
	}
}

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
	_ui.recordEnabled->setChecked(_currentRecord.vocabulary->recordEnabled(_currentRecord.id));
}
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
    }

    // speech
    say(_directionSwitched, true);
#endif

    // progress
    _progressBarTimer.setMaximum(_timeQuestion);
    _progressBarTimer.setValue(_timeQuestion);
}

#ifndef EDITION_FREE
void MainWindow::showTrayBalloon(bool directionSwitched, bool answer)
{
	QString text = learningText(TemplateTray, directionSwitched, false);
	if (answer)
	{
		text += " -> " + learningText(TemplateTray, directionSwitched, true);
	}

	_trayIcon.showMessage(tr(VOCABULARY_MASTER), text);
}
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
}

#ifndef EDITION_FREE
void MainWindow::on_actionAnswer_triggered(bool checked /* false */)
{
    _timeAnswer = TIME_NONE;
    showAnswer();
}

void MainWindow::on_actionFindInVocabulary_triggered(bool checked /* false */)
{
    openVocabulary(_currentRecord.vocabulary, true);
}

# ifndef EDITION_TRY
void MainWindow::on_actionLicense_triggered(bool checked /* false */)
{
    LicenseDialog ldLicenseDialog(_license, &_settings, this);
	if (ldLicenseDialog.exec() == QDialog::Accepted)
	{
		applySettings(false);
		enableControls();
	}
}
# endif

void MainWindow::on_actionMute_toggled(bool checked)
{
    _settings.setMute(checked);
}
#endif

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
	VocabularyOrganizerDialog vodOrganizerDialog(&_vocabularyOrganizer, this);
	if (vodOrganizerDialog.exec() == QDialog::Accepted)
	{
		enableControls();
		refreshStatusBar();
		createVocabulariesMenu();
	}
}

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
    }
}

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

	_currentRecord.vocabulary = nullptr;
	_currentRecord.id         = RECORD_NONE;
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
#ifndef EDITION_FREE
	_ui.recordControls->parentWidget()->hide();
#endif
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
#ifndef EDITION_FREE
		// disable answer
		_ui.actionAnswer->setEnabled(false);
#endif

        if (_vocabularyOrganizer.recordCount(true) == 0)
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
	            _currentRecord = _vocabularyOrganizer.recordInfo(qrand() % _vocabularyOrganizer.recordCount());
#ifndef EDITION_FREE
				if (nextRecordTry == MAX_NEXTRECORD_TRIES)
				{
					on_actionStop_triggered(false);
					return;
				}
				else
				{
					nextRecordTry++;
				}

				if ((!_settings.learnDisabledWords() && !_currentRecord.vocabulary->recordEnabled(_currentRecord.id)) || recordPriority() > maxRecordPriority)
				{
					continue;
				}
#endif

                categoryId = _currentRecord.vocabulary->recordCategory(_currentRecord.id);
#ifndef EDITION_FREE
                if (_currentRecord.vocabulary->categoryEnabled(categoryId) && _currentRecord.vocabulary->categoryPriority(categoryId) <= maxCategoryPriority  && (_currentRecord.vocabulary->recordCount(true) == 1 || _currentRecord.vocabulary != lastRecord.vocabulary || _currentRecord.id != lastRecord.id))
				{
#endif
                    break;
#ifndef EDITION_FREE
                }
#endif
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
		    }
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
		    }
	        _ui.window1->setText(learningText(TemplateLearning, _directionSwitched, false));
	        _ui.window2->clear();
            _ui.category->setText(_currentRecord.vocabulary->name() + ", " + _currentRecord.vocabulary->categoryName(categoryId));
#ifndef EDITION_FREE
			setupRecordControls();
#endif

#ifndef EDITION_FREE
		    // tray
		    if (_settings.systemTrayIcon() && _settings.showWordsInTrayBalloon())
			{
			    showTrayBalloon(_directionSwitched, false);
		    }

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
                }
	        }

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
			        }
		        }
	        }

            // speech
            if (_settings.newWordSound())
			{
                QTest::qWait(SAY_BEEP_WAIT);
                say(_directionSwitched, false);
            }
#endif

            // next question time
            _timeQuestion = _settings.wordsFrequency();

#ifndef EDITION_FREE
            // enable answer
            _ui.actionAnswer->setEnabled(true);
#endif
        }
    }

    if (_timeAnswer == 0 && _ui.actionStop->isEnabled())
	{
        showAnswer();
    }

	if (!_learningTimer.isActive() && _ui.actionStop->isEnabled())
	{
		_learningTimer.start(MILISECONDS_PER_SECOND);
		_ui.actionNext->setEnabled(true);
	}
}

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
        }
    }
}
#endif

void MainWindow::on_menuVocabularies_triggered(QAction *action)
{
	const quint8 vocabularyIndex                             = action->data().toUInt();
	const VocabularyOrganizer::VocabularyInfo vocabularyInfo = _vocabularyOrganizer.vocabularyInfo(vocabularyIndex);

	openVocabulary(vocabularyInfo.vocabulary
#ifndef EDITION_FREE
		, false
#endif
		);
}

#ifndef EDITION_FREE
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
#endif

void MainWindow::on_updateChecker_finished()
{
	if (_updateChecker.checkResult() != QNetworkReply::NoError)
	{
		return;
	}

	if (_updateChecker.updateAvailable())
	{
		const QString text = tr("New version %1 of Vocabulary Master available at <a href=\"http://vocabulary-master.cz/\">homepage</a>.").arg(_updateChecker.updateVersion());
		QMessageBox::information(this, tr("Update available"), text);
	}
}

void MainWindow::on_vocabularyOrganizer_vocabularyClose(const Vocabulary *vocabulary)
{
	if (_currentRecord.vocabulary == vocabulary)
	{
		on_actionNext_triggered();
	}
}