#include "mainwindow.h"

#include "settingsdialog.h"
#include <QtGui/QFileDialog>
#include "vocabularymanagerdialog.h"
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QTest>
#include <QtGui/QMessageBox>
#if !defined(FREE) && defined(Q_WS_WIN)
# include <Windows.h>
#endif
#include "common/vocabularyopenprogressdialog.h"
#if !defined(FREE) && !defined(TRY)
# include "licensedialog.h"
#endif

#ifdef FREE
const QString FREE_SUFFIX = QT_TRANSLATE_NOOP("MainWindow", " FREE");
#endif
#ifdef TRY
const QString TRY_SUFFIX = QT_TRANSLATE_NOOP("MainWindow", " TRY");
#endif
const QString VOCABULARY_SUFFIX = "sl3";
const QString VOCABULARY_FILTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary (*." + VOCABULARY_SUFFIX + ")");
const QString VOCABULARY_MASTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary Master");

const void MainWindow::ApplySettings(const bool &pStartup)
{
    SetLayout();

	// change translation
	if (!_qtTranslator.load(_sSettings.GetTranslation(), DIR_LANG)) {
		if (_sSettings.GetTranslation().isEmpty()) {
            if (_qtTranslator.load(QLocale::system().name(), DIR_LANG)) {
                _sSettings.SetTranslation(QLocale::system().name() + '.' + LANG_SUFFIX);
            } // if
		} // if
	} // if
#ifndef FREE
	_pPlugins.SetLanguage(_sSettings.GetTranslation());
#endif

    if (_sSettings.GetAlwaysOnTop()) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    } // if else
#ifndef FREE
    if (pStartup && _sSettings.GetWindowX() != Settings::DEFAULT_DIMENSION) {
        setGeometry(_sSettings.GetWindowX(), _sSettings.GetWindowY(), _sSettings.GetWindowWidth(), _sSettings.GetWindowHeight());
    } // if
#endif
    show();

#ifndef FREE
    _qstiTrayIcon.setVisible(_sSettings.GetSystemTrayIcon());
#endif

#if !defined(FREE) && defined(Q_WS_WIN)
	RegisterHotkeys();
#endif
} // ApplySettings

#ifndef FREE
const void MainWindow::CreateTrayMenu()
{
    _qaTrayManage = _qmTray.addAction(tr("&Manage"));
    _qaTraySettings = _qmTray.addAction(tr("&Settings"));
	_qaTrayExit = _qmTray.addAction(tr("&Exit"));

	connect(&_qmTray, SIGNAL(triggered(QAction *)), SLOT(on_qmTray_triggered(QAction *)));
	_qstiTrayIcon.setContextMenu(&_qmTray);
} // CreateTrayMenu
#endif

const void MainWindow::EnableControls()
{
	// menu
#if !defined(FREE) && !defined(TRY)
	_umwMainWindow.qaNew->setEnabled(_lLicense->IsLoaded());
	_umwMainWindow.qaOpen->setEnabled(_lLicense->IsLoaded());
#endif
    _umwMainWindow.qmVocabulary->setEnabled(
#if !defined(FREE) && !defined(TRY)
		_lLicense->IsLoaded() &&
#endif
		_vVocabulary.IsOpen());
#if !defined(FREE) && !defined(TRY)
	_umwMainWindow.qmOptions->setEnabled(_lLicense->IsLoaded());
#endif

	// tool bar
	_umwMainWindow.qaStart->setEnabled(
#if !defined(FREE) && !defined(TRY)
		_lLicense->IsLoaded() &&
#endif
		_vVocabulary.IsOpen() && _iTimerQuestion == 0 && _vVocabulary.GetRecordCount(true) > 0);
	_umwMainWindow.qaStop->setEnabled(_iTimerQuestion != 0);
	_umwMainWindow.qaNext->setEnabled(_iTimerQuestion != 0);
#ifndef FREE
	_umwMainWindow.qaFindInVocabulary->setEnabled(_iTimerQuestion != 0);
    _umwMainWindow.qaAnswer->setEnabled(_iTimerAnswer != 0);

    // tray
    _qaTrayManage->setEnabled(_vVocabulary.IsOpen());
#endif
} // EnableControls

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::LanguageChange:
            {
                QString qsCategory, qsLang1, qsLang2;
                if (_iTimerQuestion != 0) {
                    qsLang1 = _umwMainWindow.qlLanguage1->text();
                    qsLang2 = _umwMainWindow.qlLanguage2->text();
                    qsCategory = _umwMainWindow.qlCategory->text();
                } // if
                _umwMainWindow.retranslateUi(this);
                if (_iTimerQuestion != 0) {
                    _umwMainWindow.qlLanguage1->setText(qsLang1);
                    _umwMainWindow.qlLanguage2->setText(qsLang2);
                    _umwMainWindow.qlCategory->setText(qsCategory);
                } // if
            }
#ifdef FREE
            setWindowTitle(windowTitle() + FREE_SUFFIX);
#endif
#ifdef TRY
            setWindowTitle(windowTitle() + TRY_SUFFIX);
#endif
            break;
#ifndef FREE
        case QEvent::WindowStateChange:
		    if (isMinimized() && _sSettings.GetSystemTrayIcon() && _sSettings.GetMinimizeToTray()) {
			    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint); // just add some flag to hide window
		    } // if
#endif
	} // switch

	return QMainWindow::event(event);
} // event

const QString MainWindow::GetLanguageText(const bool &pDirectionSwitched, const bool &pAnswer) const
{
    if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
        return _vVocabulary.GetSettings(KEY_LANGUAGE1);
    } else {
        return _vVocabulary.GetSettings(KEY_LANGUAGE2);
    } // if else
} // GetLanguageText

const bool MainWindow::GetLearningDirection() const
{
	if (_sSettings.GetSwitchLearningDirection() != Qt::PartiallyChecked) {
		return _sSettings.GetSwitchLearningDirection();
	} else {
		return qrand() % 2;
	} // if else
} // if

const QString MainWindow::GetLearningText(const eTemplate &pTemplate, const bool &pDirectionSwitched, const bool &pAnswer) const
{
	QString qsTemplate;
	Vocabulary::eFieldLanguage eflLanguage;
	if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		if (pTemplate == TemplateLearning) {
			qsTemplate = _vVocabulary.GetSettings(KEY_LEARNINGTEMPLATE1);
		}
#ifndef FREE
		else {
			qsTemplate = _vVocabulary.GetSettings(KEY_TRAYTEMPLATE1);
		} // if else
#endif
		eflLanguage = Vocabulary::FieldLanguageLeft;
	} else {
		if (pTemplate == TemplateLearning) {
			qsTemplate = _vVocabulary.GetSettings(KEY_LEARNINGTEMPLATE2);
		}
#ifndef FREE
		else {
			qsTemplate = _vVocabulary.GetSettings(KEY_TRAYTEMPLATE2);
		} // if else
#endif
		eflLanguage = Vocabulary::FieldLanguageRight;
	} // if else

	// substitute variables in template
	foreach (int iFieldId, _vVocabulary.GetFieldIds()) {
		if (_vVocabulary.GetFieldLanguage(iFieldId) == eflLanguage) {
			QString qsData = _vVocabulary.GetDataText(_iCurrentRecordId, iFieldId);

			QString qsField = _vVocabulary.GetFieldTemplateName(iFieldId);
			qsTemplate.replace(VARIABLE_MARK + qsField, qsData);
		} // if
	} // foreach

	return qsTemplate;
} // GetLearningText

MainWindow::~MainWindow()
{
#ifndef TRY
    _sSettings.SetVocabularyFile(_vVocabulary.GetVocabularyFile());
#endif
#ifndef FREE
	_sSettings.SetWindowX(geometry().x());
	_sSettings.SetWindowY(geometry().y());
	_sSettings.SetWindowHeight(geometry().height());
	_sSettings.SetWindowWidth(geometry().width());

	_pPlugins.Uninitialize();
# ifndef TRY
    delete _lLicense;
# endif
#endif
} // ~MainWindow

MainWindow::MainWindow(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QMainWindow(pParent, pFlags)
{
    _iTimerAnswer = 0;
	_iTimerQuestion = 0;

	qsrand(QTime::currentTime().msec());

    // gui
	_umwMainWindow.setupUi(this);
    _umwMainWindow.qlLanguage1->hide();
    _umwMainWindow.qlLanguage2->hide();
    _umwMainWindow.qlCategory->hide();
#ifdef FREE
	_umwMainWindow.qaFindInVocabulary->setVisible(false);
    _umwMainWindow.qaAnswer->setVisible(false);
    _umwMainWindow.qaMute->setVisible(false);
    _umwMainWindow.qaLicense->setVisible(false);
	foreach (QObject *qoObject, _umwMainWindow.qtbToolBar->children()) {
		QAction *qaAction = qobject_cast<QAction *>(qoObject);
		if (qaAction && qaAction->isSeparator()) {
			qaAction->deleteLater();
		} // if
	} // foreach
#else
# ifdef TRY
    _umwMainWindow.qaOpen->setVisible(false);
    _umwMainWindow.qaLicense->setVisible(false);
# endif
	CreateTrayMenu();
#endif
    _umwMainWindow.qsbStatusBar->addWidget(&_qlVocabularyStatus);

#ifndef FREE
# ifndef TRY
    // license
    _lLicense = new License(&_sSettings);
# endif

    // plugins
	_pPlugins.Load();
	_pPlugins.Initialize();
#endif

	// translator
	QCoreApplication::installTranslator(&_qtTranslator);

    // settings
    ApplySettings(true);

#ifndef TRY
	VocabularyOpenProgressDialog vopdOpenProgress(&_vVocabulary, this);
    vopdOpenProgress.show();
    _vVocabulary.Open(_sSettings.GetVocabularyFile());
    vopdOpenProgress.hide();
#endif
    RefreshStatusBar();

    // controls
    EnableControls();
#ifndef FREE
    _umwMainWindow.qaMute->setChecked(_sSettings.GetMute());

    // learning
	if (
# ifndef TRY
        _lLicense->IsLoaded() &&
# endif
        _sSettings.GetStartLearningOnStartup() && _vVocabulary.IsOpen()) {
		on_qaStart_triggered();
	} // if

	// connections
	connect(&_qstiTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason)));
#endif
} // MainWindow

const void MainWindow::on_qaAbout_triggered(bool checked /* false */)
{
    QMessageBox::about(this, tr("About Vocabulary Master"), "<center><b>" + VOCABULARY_MASTER
#ifdef FREE
        + FREE_SUFFIX
#endif
#ifdef TRY
        + TRY_SUFFIX
#endif
        + "</b></center><center>Version 1.0.272</center><br />Copyright (C) 2011 Isshou");
} // on_qaAbout_triggered

#ifndef FREE
const void MainWindow::on_qaAnswer_triggered(bool checked /* false */)
{
/*#ifdef _DEBUG
    qDebug("(Answer) Current word: %d, timer: %d", _iCurrentRecordId, _qhCurrentAnswer.value(_iCurrentRecordId));
#endif*/
    timerEvent(&QTimerEvent(_qhCurrentAnswer.value(_iCurrentRecordId)));
} // on_qaAnswer_triggered

const void MainWindow::on_qaFindInVocabulary_triggered(bool checked /* false */)
{
    OpenVocabulary(true);
} // on_qaFindInVocabulary_triggered

# ifndef TRY
const void MainWindow::on_qaLicense_triggered(bool checked /* false */)
{
    LicenseDialog ldLicenseDialog(_lLicense, &_sSettings, this);
	if (ldLicenseDialog.exec() == QDialog::Accepted) {
		ApplySettings(false);
		EnableControls();
	} // if
} // on_qaLicense_triggered
# endif
#endif

const void MainWindow::on_qaManage_triggered(bool checked /* false */)
{
    OpenVocabulary(
#ifndef FREE
		false
#endif
		);
} // on_qaManage_triggered

#ifndef FREE
const void MainWindow::on_qaMute_toggled(bool checked)
{
    _sSettings.SetMute(checked);
} // on_qaMute_toggled
#endif

const void MainWindow::on_qaNew_triggered(bool checked /* false */)
{
#ifndef TRY
    QFileDialog qfdNew(this, tr("Create new vocabulary"), QFileInfo(_vVocabulary.GetVocabularyFile()).absolutePath(), VOCABULARY_FILTER);
    qfdNew.setAcceptMode(QFileDialog::AcceptSave);
    if (qfdNew.exec() == QDialog::Accepted) {
        QFileInfo qfiFile(qfdNew.selectedFiles().at(0));
		QString qsFile;
		if (qfiFile.suffix() != VOCABULARY_SUFFIX) {
			qsFile = qfdNew.selectedFiles().at(0) + "." + VOCABULARY_SUFFIX;
		} else {
			qsFile = qfdNew.selectedFiles().at(0);
		} // if else
        _vVocabulary.New(qsFile);
#else
        _vVocabulary.New(QString());
#endif

        EnableControls();
        RefreshStatusBar();
#ifndef TRY
    } // if
#endif
} // on_qaNew_triggered

const void MainWindow::on_qaNext_triggered(bool checked /* false */)
{
	on_qaStop_triggered();
	on_qaStart_triggered();
} // on_qaNext_triggered

#ifndef TRY
const void MainWindow::on_qaOpen_triggered(bool checked /* false */)
{
    QString qsFile = QFileDialog::getOpenFileName(this, tr("Open vocabulary"), QFileInfo(_vVocabulary.GetVocabularyFile()).absolutePath(), VOCABULARY_FILTER);
    if (!qsFile.isEmpty()) {
        VocabularyOpenProgressDialog vopdOpenProgress(&_vVocabulary, this);
        vopdOpenProgress.show();
        _vVocabulary.Open(qsFile);
        vopdOpenProgress.hide();

        EnableControls();
        RefreshStatusBar();

        if (_iTimerQuestion != 0) {
            on_qaNext_triggered();
        } // if
    } // if
} // on_qaOpen_triggered
#endif

const void MainWindow::on_qaSettings_triggered(bool checked /* false */)
{
	SettingsDialog sdDialog(
#ifndef FREE
        &_pPlugins,
#endif
        &_sSettings, this);
    if (sdDialog.exec() == QDialog::Accepted) {
        ApplySettings(false);
    } // if
} // on_qaSettings_triggered

const void MainWindow::on_qaStart_triggered(bool checked /* false */)
{
	_iTimerQuestion = startTimer(_sSettings.GetWordsFrequency() * MILISECONDS_PER_SECOND);

	EnableControls();
    _umwMainWindow.qlCategory->show();

    _iCurrentRecordId = RECORD_NONE;
	timerEvent(&QTimerEvent(_iTimerQuestion));
} // on_qaStart_triggered

const void MainWindow::on_qaStop_triggered(bool checked /* false */)
{
    if (_iTimerAnswer != 0) {
        killTimer(_iTimerAnswer);
        _iTimerAnswer = 0;
    } // if
	killTimer(_iTimerQuestion);
	_iTimerQuestion = 0;
	EnableControls();

    _umwMainWindow.qlLanguage1->hide();
	_umwMainWindow.qtbWindow1->clear();
    _umwMainWindow.qlLanguage2->hide();
	_umwMainWindow.qtbWindow2->clear();
    _umwMainWindow.qlCategory->hide();
} // on_qaStop_triggered

#ifndef FREE
const void MainWindow::on_qmTray_triggered(QAction *action)
{
    if (action == _qaTrayManage) {
        on_qaManage_triggered();
    } else {
        if (action == _qaTraySettings) {
            on_qaSettings_triggered();
        } else {
	        if (action == _qaTrayExit) {
		        close();
	        } // if
        } // if else
    } // if else
} // on_qmTray_triggered

const void MainWindow::on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick && isMinimized()) {
		setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
		showNormal();
	} // if
} // on_qstiTrayIcon_activated
#endif

const void MainWindow::OpenVocabulary(
#ifndef FREE
    const bool &pCurrentRecord
#endif
    )
{
    VocabularyManagerDialog vmdManager(&_vVocabulary,
#ifndef FREE
        &_pPlugins,
#endif
        this);
#ifndef FREE
    if (pCurrentRecord) {
        vmdManager.ExecOnRecord(_iCurrentRecordId);
    } else {
#endif
        vmdManager.exec();
#ifndef FREE
    } // if else
#endif

	_umwMainWindow.qaStart->setEnabled(_vVocabulary.IsOpen() && _iTimerQuestion == 0 && _vVocabulary.GetRecordCount() > 0);
    RefreshStatusBar();
} // OpenVocabulary

const void MainWindow::RefreshStatusBar()
{
#ifndef TRY
    if (_vVocabulary.GetName().isEmpty()) {
        _qlVocabularyStatus.setText("");
    } else {
#endif
        QString qsInfo;
#ifdef FREE
        qsInfo = QString("%1, %2").arg(_vVocabulary.GetName()).arg(_vVocabulary.GetRecordCount());
#elif defined TRY
        qsInfo = tr("memory, %1/%2").arg(_vVocabulary.GetRecordCount(true)).arg(_vVocabulary.GetRecordCount());
#else
        qsInfo = QString("%1, %2/%3").arg(_vVocabulary.GetName()).arg(_vVocabulary.GetRecordCount(true)).arg(_vVocabulary.GetRecordCount());
#endif
        _qlVocabularyStatus.setText(tr("%1 records").arg(qsInfo));
#ifndef TRY
    } // if else
#endif
} // RefreshStatusBar

#if !defined(FREE) && defined(Q_WS_WIN)
const void MainWindow::RegisterHotkeys() const
{
	for (int iHotkey = 0; iHotkey < Settings::HotkeyCount - 1; iHotkey++) {
		Settings::sHotKeyInfo shkiHotkey = _sSettings.GetHotkey(static_cast<Settings::eHotkey>(iHotkey));

		if (shkiHotkey.qui32VirtualKey == SettingsDialog::VIRTUALKEY_NONE) {
			UnregisterHotKey(winId(), iHotkey);
		} else {
			UINT uiModifiers;
			if (shkiHotkey.qsText.contains(MODIFIER_ALT)) {
				uiModifiers = MOD_ALT;
			} else {
				uiModifiers = 0;
			} // if else
			if (shkiHotkey.qsText.contains(MODIFIER_CTRL)) {
				uiModifiers |= MOD_CONTROL;
			} // if
			if (shkiHotkey.qsText.contains(MODIFIER_SHIFT)) {
				uiModifiers |= MOD_SHIFT;
			} // if

			RegisterHotKey(winId(), iHotkey, uiModifiers, shkiHotkey.qui32VirtualKey);
		} // if else
	} // for
} // RegisterHotkeys
#endif

#ifndef FREE
const void MainWindow::Say(const bool &pDirectionSwitched, const bool &pAnswer) const
{
    if (!_sSettings.GetMute()) {
	    Vocabulary::eFieldLanguage eflLanguage;
        QString qsSpeech, qsVoice;
	    if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
            qsSpeech = KEY_SPEECH1;
            qsVoice = KEY_VOICE1;
		    eflLanguage = Vocabulary::FieldLanguageLeft;
	    } else {
            qsSpeech = KEY_SPEECH2;
            qsVoice = KEY_VOICE2;
		    eflLanguage = Vocabulary::FieldLanguageRight;
	    } // if else

        // get text to speech
        QString qsText;
        foreach (int iFieldId, _vVocabulary.GetFieldIds()) {
            if (_vVocabulary.GetFieldLanguage(iFieldId) == eflLanguage && _vVocabulary.GetFieldAttributes(iFieldId) & Vocabulary::FieldAttributeSpeech) {
                if (!qsText.isEmpty()) {
                    qsText += ' ';
                } // if
                qsText += _vVocabulary.GetDataText(_iCurrentRecordId, iFieldId);
            } // if
        } // foreach

        if (!qsText.isEmpty()) {
	        int iSpeech = _vVocabulary.GetSettings(qsSpeech).toInt();
            qsVoice = _vVocabulary.GetSettings(qsVoice);
	        if (iSpeech != TTSInterface::TTPluginNone) {
		        TTSInterface *tiPlugin = _pPlugins.GetTTSPlugin(static_cast<TTSInterface::eTTSPlugin>(iSpeech));
                if (tiPlugin) {
		            tiPlugin->Say(qsVoice, qsText);
                } // if
	        } // if
        } // if
    } // if
} // Say
#endif

const void MainWindow::SetLayout()
{
    QBoxLayout *qblLayout;

    if (_umwMainWindow.qwCentral->layout()) {
        delete _umwMainWindow.qwCentral->layout();
    } // if

#ifndef FREE
    if (_sSettings.GetHorizontalLayout()) {
        qblLayout = new QHBoxLayout(_umwMainWindow.qwCentral);
    } else {
#endif
        qblLayout = new QVBoxLayout(_umwMainWindow.qwCentral);
#ifndef FREE
    } // if else
#endif

#ifndef FREE
    if (_sSettings.GetHorizontalLayout()) {
        qblLayout->addWidget(_umwMainWindow.qlCategory);
    } // if
#endif
    qblLayout->addWidget(_umwMainWindow.qvblQuestion->parentWidget());
#ifndef FREE
    if (!_sSettings.GetHorizontalLayout()) {
#endif
        qblLayout->addWidget(_umwMainWindow.qlCategory);
#ifndef FREE
    } // if
#endif
    qblLayout->addWidget(_umwMainWindow.qvblAnswer->parentWidget());
} // SetLayout

#ifndef FREE
const void MainWindow::ShowTrayBalloon(const bool &pDirectionSwitched, const bool &pAnswer)
{
	QString qsText = GetLearningText(TemplateTray, pDirectionSwitched, false);
	if (pAnswer) {
		qsText += " -> " + GetLearningText(TemplateTray, pDirectionSwitched, true);
	} // if

	_qstiTrayIcon.showMessage(VOCABULARY_MASTER, qsText);
} // ShowTrayBalloon
#endif

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _iTimerQuestion) {
        if (_vVocabulary.GetRecordCount(true) == 0) {
            on_qaStop_triggered();
        } else {
            int iCategoryId;
            int iLastRecordId = _iCurrentRecordId;
            while (true) {
	            _iCurrentRecordId = _vVocabulary.GetRecordId(qrand() % _vVocabulary.GetRecordCount());
                iCategoryId = _vVocabulary.GetRecordCategory(_iCurrentRecordId);
#ifndef FREE
                if (_vVocabulary.GetCategoryEnabled(iCategoryId) && (_vVocabulary.GetRecordCount(true) == 1 || _iCurrentRecordId != iLastRecordId)) {
#endif
                    break;
#ifndef FREE
                } // if
#endif
            } // while
/*#ifdef _DEBUG
            qDebug("Current word: %d", _iCurrentRecordId);
#endif*/

            // question parameters
            sAnswer saAnswer;
	        saAnswer.iWord = _iCurrentRecordId;
	        saAnswer.bDirectionSwitched = GetLearningDirection();

            // gui
		    QString qsLang1 = GetLanguageText(saAnswer.bDirectionSwitched, false);
		    if (qsLang1.isEmpty()) {
			    _umwMainWindow.qlLanguage1->hide();
		    } else {
			    _umwMainWindow.qlLanguage1->show();
			    _umwMainWindow.qlLanguage1->setText(qsLang1);
		    } // if else
		    QString qsLang2 = GetLanguageText(saAnswer.bDirectionSwitched, true);
		    if (qsLang2.isEmpty()) {
			    _umwMainWindow.qlLanguage2->hide();
		    } else {
			    _umwMainWindow.qlLanguage2->show();
			    _umwMainWindow.qlLanguage2->setText(qsLang2);
		    } // if else
	        _umwMainWindow.qtbWindow1->setText(GetLearningText(TemplateLearning, saAnswer.bDirectionSwitched, false));
	        _umwMainWindow.qtbWindow2->clear();
            _umwMainWindow.qlCategory->setText(_vVocabulary.GetCategoryName(iCategoryId));

#ifndef FREE
		    // tray
		    if (_sSettings.GetSystemTrayIcon() && _sSettings.GetShowWordsInTrayBalloon()) {
			    ShowTrayBalloon(saAnswer.bDirectionSwitched, false);
		    } // if

            // sound
	        if (_sSettings.GetNewWordSound() && !_sSettings.GetMute()) {
		        QApplication::beep();
	        } // if

            // flash
	        if (_sSettings.GetNewWordFlash()) {
                QString qsStyleSheet = _umwMainWindow.qtbWindow1->styleSheet();

		        for (int iI = 0; iI < FLASH_COUNT && !_qhCurrentAnswer.contains(_iCurrentRecordId); iI++) {
                    _umwMainWindow.qtbWindow1->setStyleSheet(QString("QAbstractScrollArea { background-color: %1 }").arg(_sSettings.GetColorFlash()));
			        QTest::qWait(FLASH_WAIT);
                    _umwMainWindow.qtbWindow1->setStyleSheet(qsStyleSheet);
			        if (iI < FLASH_COUNT - 1) {
				        QTest::qWait(FLASH_WAIT);
			        } // if
		        } // for
	        } // if

            // speech
            if (_sSettings.GetNewWordSound() && !_qhCurrentAnswer.contains(_iCurrentRecordId)) {
                QTest::qWait(SAY_BEEP_WAIT);
                Say(saAnswer.bDirectionSwitched, false);
            } // if
#endif

            // answer timer
            _iTimerAnswer = startTimer(_sSettings.GetWaitForAnswer() * MILISECONDS_PER_SECOND);
/*#ifdef _DEBUG
            qDebug("Answer: %d, word: %d, timer: %d", _iTimerAnswer, saAnswer.iWord, _iTimerAnswer);
#endif*/
            _taHash.insert(_iTimerAnswer, saAnswer);
            _qhCurrentAnswer.insert(saAnswer.iWord, _iTimerAnswer);

#ifndef FREE
            // answer
            _umwMainWindow.qaAnswer->setEnabled(true);
#endif
        } // if else
    } else {
        // check for answer timer
        if (_taHash.contains(event->timerId())) {
            killTimer(event->timerId());
            if (_iTimerAnswer == event->timerId()) {
                _iTimerAnswer = 0;
            } // if

            sAnswer saAnswer = _taHash.value(event->timerId());
            _taHash.remove(event->timerId());
            _qhCurrentAnswer.remove(saAnswer.iWord);

            if (saAnswer.iWord == _iCurrentRecordId) {
#ifndef FREE
                // answer
                _umwMainWindow.qaAnswer->setEnabled(false);
#endif

                // gui
                _umwMainWindow.qtbWindow2->setText(GetLearningText(TemplateLearning, saAnswer.bDirectionSwitched, true));
                _umwMainWindow.qtbWindow2->repaint();

#ifndef FREE
				// tray
				if (_sSettings.GetSystemTrayIcon() && _sSettings.GetShowWordsInTrayBalloon()) {
					ShowTrayBalloon(saAnswer.bDirectionSwitched, true);
				} // if

                // speech
                Say(saAnswer.bDirectionSwitched, true);
#endif
            } // if
        } // if
    } // if else
} // timerEvent

#if !defined(FREE) && defined(Q_WS_WIN)
bool MainWindow::winEvent(MSG *message, long *result)
{
	if (message->message == WM_HOTKEY) {
		switch (message->wParam) {
			case Settings::HotkeyAnswer:
				if (_umwMainWindow.qaAnswer->isEnabled()) {
					on_qaAnswer_triggered();
				} // if
				break;
			case Settings::HotkeyMinimize:
				showMinimized();
				break;
			case Settings::HotkeyNext:
				if (_umwMainWindow.qaNext->isEnabled()) {
					on_qaNext_triggered();
				} // if
				break;
			case Settings::HotkeyRestore:
				showNormal();
		} // switch
	} // if

	return false;
} // winEvent
#endif