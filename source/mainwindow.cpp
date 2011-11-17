#include "mainwindow.h"

#include "settingsdialog.h"
#include "vocabularymanagerdialog.h"
#include <QtCore/QTime>
#include <QTest>
#include <QtGui/QMessageBox>
#ifndef FREE
# ifdef Q_WS_WIN
#  include <Windows.h>
# endif
# ifndef TRY
#  include "licensedialog.h"
# endif
#endif
#ifndef FREE
# include <QtGui/QSound>
# include "vocabularymanagerdialog/prioritydelegate.h"
#endif
#include "vocabularyorganizerdialog.h"

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
    _qmTray.clear();
    CreateTrayMenu();

	_umwMainWindow.qtbToolBar->setVisible(_sSettings.GetShowToolBar());
	_umwMainWindow.qlLanguage1->setVisible(_qtLearning.isActive() && _sSettings.GetShowLanguageNames());
	_umwMainWindow.qlLanguage2->setVisible(_qtLearning.isActive() && _sSettings.GetShowLanguageNames());
	_umwMainWindow.qlCategory->setVisible(_qtLearning.isActive() && _sSettings.GetShowCategoryName());
	_umwMainWindow.qhblRecordControls->parentWidget()->setVisible(_qtLearning.isActive() && _sSettings.GetShowRecordControls());
	_umwMainWindow.qsbStatusBar->setVisible(_sSettings.GetShowStatusBar());
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
    /*_qaTrayManage = _qmTray.addAction(tr("&Manage"));
    _qaTrayManage->setIcon(QIcon(":/res/mainwindow/menubar/manage.png"));*/
    _qaTraySettings = _qmTray.addAction(tr("&Settings"));
    _qaTraySettings->setIcon(QIcon(":/res/mainwindow/menubar/settings.png"));
    _qmTray.addSeparator();
	_qaTrayExit = _qmTray.addAction(tr("&Exit"));
    _qaTrayExit->setIcon(QIcon(":/res/mainwindow/menubar/exit.png"));

	connect(&_qmTray, SIGNAL(triggered(QAction *)), SLOT(on_qmTray_triggered(QAction *)));
	_qstiTrayIcon.setContextMenu(&_qmTray);
} // CreateTrayMenu
#endif

const void MainWindow::EnableControls()
{
	// menu
#if !defined(FREE) && !defined(TRY)
	_umwMainWindow.qaOrganizer->setEnabled(_lLicense->IsLoaded());
#endif
    _umwMainWindow.qmVocabularies->setEnabled(
#if !defined(FREE) && !defined(TRY)
		_lLicense->IsLoaded() &&
#endif
		_voOrganizer.IsOpen());
#if !defined(FREE) && !defined(TRY)
	_umwMainWindow.qmOptions->setEnabled(_lLicense->IsLoaded());
#endif

	// tool bar
	_umwMainWindow.qaStart->setEnabled(
#if !defined(FREE) && !defined(TRY)
		_lLicense->IsLoaded() &&
#endif
		_voOrganizer.IsOpen() && !_qtLearning.isActive() && _voOrganizer.GetRecordCount(true) > 0);
	_umwMainWindow.qaStop->setEnabled(_qtLearning.isActive());
	_umwMainWindow.qaNext->setEnabled(_qtLearning.isActive());
#ifndef FREE
	_umwMainWindow.qaFindInVocabulary->setEnabled(_qtLearning.isActive());
    _umwMainWindow.qaAnswer->setEnabled(_qtLearning.isActive() && _iTimeAnswer >= TIME_NOW);

    // tray
    //_qaTrayManage->setEnabled(_vVocabulary.IsOpen());
#endif
} // EnableControls

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::LanguageChange:
            {
                QString qsCategory, qsLang1, qsLang2;
                if (_qtLearning.isActive()) {
                    qsLang1 = _umwMainWindow.qlLanguage1->text();
                    qsLang2 = _umwMainWindow.qlLanguage2->text();
                    qsCategory = _umwMainWindow.qlCategory->text();
                } // if
                _umwMainWindow.retranslateUi(this);
                if (_qtLearning.isActive()) {
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
		return _sriCurrentRecord.vVocabulary->GetLanguageName(VocabularyDatabase::FieldLanguageLeft);
    } else {
        return _sriCurrentRecord.vVocabulary->GetLanguageName(VocabularyDatabase::FieldLanguageRight);
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
	VocabularyDatabase::eFieldLanguage eflLanguage;
	if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		if (pTemplate == TemplateLearning) {
			qsTemplate = _sriCurrentRecord.vVocabulary->GetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageLeft);
		}
#ifndef FREE
		else {
			qsTemplate = _sriCurrentRecord.vVocabulary->GetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageLeft);
		} // if else
#endif
		eflLanguage = VocabularyDatabase::FieldLanguageLeft;
	} else {
		if (pTemplate == TemplateLearning) {
			qsTemplate = _sriCurrentRecord.vVocabulary->GetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageRight);
		}
#ifndef FREE
		else {
			qsTemplate = _sriCurrentRecord.vVocabulary->GetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageRight);
		} // if else
#endif
		eflLanguage = VocabularyDatabase::FieldLanguageRight;
	} // if else

	// substitute variables in template
	foreach (int iFieldId, _sriCurrentRecord.vVocabulary->GetFieldIds()) {
		if (_sriCurrentRecord.vVocabulary->GetFieldLanguage(iFieldId) == eflLanguage) {
			QString qsData = _sriCurrentRecord.vVocabulary->GetDataText(_sriCurrentRecord.iId, iFieldId);

			QString qsField = _sriCurrentRecord.vVocabulary->GetFieldTemplateName(iFieldId);
			qsTemplate.replace(VARIABLE_MARK + qsField, qsData);
		} // if
	} // foreach

	return qsTemplate;
} // GetLearningText

#ifndef FREE
const int MainWindow::GetRecordPriority() const
{
    foreach (int iFieldId, _sriCurrentRecord.vVocabulary->GetFieldIds()) {
        if (_sriCurrentRecord.vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
            VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _sriCurrentRecord.vVocabulary->GetFieldBuiltIn(iFieldId);
            switch (efbBuiltIn) {
                case VocabularyDatabase::FieldBuiltInPriority:
                    return _sriCurrentRecord.vVocabulary->GetDataText(_sriCurrentRecord.iId, iFieldId).toInt();
            } // switch
        } // if
    } // foreach

    return PriorityDelegate::RECORD_PRIORITY_MIN;
} // GetRecordPriority
#endif

MainWindow::~MainWindow()
{
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

MainWindow::MainWindow(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QMainWindow(pParent, pFlags), _ucUpdateChecker(&_sSettings), _voOrganizer(&_sSettings)
{
	_qtLearning.setSingleShot(true);
#ifndef FREE
    _qhblInner = NULL;
#endif

	qsrand(QTime::currentTime().msec());

    // gui
	_umwMainWindow.setupUi(this);
#ifdef FREE
	_umwMainWindow.qhblRecordControls->parentWidget()->hide();
	_umwMainWindow.qlLanguage1->hide();
	_umwMainWindow.qlLanguage2->hide();
	_umwMainWindow.qlCategory->hide();

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
	_qstiTrayIcon.setToolTip("Vocabulary Master");
#endif
    _umwMainWindow.qsbStatusBar->addWidget(&_qlVocabularyStatus);
    _qpbTimer.setTextVisible(false);
    _umwMainWindow.qsbStatusBar->addWidget(&_qpbTimer, 1);

#ifndef FREE
# ifndef TRY
    // license
    _lLicense = new License(&_sSettings);
# endif

    // plugins
	_pPlugins.Load();
	_pPlugins.Initialize();

    // system tray icon
    QIcon qiTrayIcon(":/res/mainwindow/mainwindow.png");
    _qstiTrayIcon.setIcon(qiTrayIcon);
#endif

	// translator
	QCoreApplication::installTranslator(&_qtTranslator);

    // settings
    ApplySettings(true);

#ifndef TRY
	_voOrganizer.Open(this);
#endif
    RefreshStatusBar();

    // controls
    EnableControls();
#ifndef FREE
    _umwMainWindow.qaMute->setChecked(_sSettings.GetMute());

	// connections
	connect(&_qstiTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason)));
#endif
	connect(&_qtLearning, SIGNAL(timeout()), SLOT(on_qtLearning_timeout()));
	connect(&_ucUpdateChecker, SIGNAL(Finished()), SLOT(on_ucUpdateChecker_Finished()));

#ifndef FREE
	// learning
	if (
# ifndef TRY
		_lLicense->IsLoaded() &&
# endif
		_sSettings.GetStartLearningOnStartup() && _voOrganizer.IsOpen()) {
			on_qaStart_triggered();
	} // if
#endif

	// update check
	if (_sSettings.GetUpdateCheck()) {
		_ucUpdateChecker.CheckForUpdate();
	} // if
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
        + "</b></center><center>Version " + _ucUpdateChecker.GetCurrentVersion() + "</center><br />Copyright (C) 2011 Isshou");
} // on_qaAbout_triggered

#ifndef FREE
const void MainWindow::on_qaAnswer_triggered(bool checked /* false */)
{
    _iTimeAnswer = TIME_NONE;
    ShowAnswer();
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

const void MainWindow::on_qaNext_triggered(bool checked /* false */)
{
	if (_qtLearning.isActive()) {
		_qtLearning.stop();
	} // if
	_iTimeQuestion = TIME_NOW;
    _iTimeAnswer = TIME_NONE;

	_qtLearning.start(0);
} // on_qaNext_triggered

const void MainWindow::on_qaOrganizer_triggered(bool checked /* false */)
{
	VocabularyOrganizerDialog vodOrganizerDialog(&_voOrganizer, this);
	if (vodOrganizerDialog.exec() == QDialog::Accepted) {
		EnableControls();
		RefreshStatusBar();
	} // if
} // on_qaOrganizer_triggered

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
    _iTimeQuestion = TIME_NOW;
    _iTimeAnswer = TIME_NONE;

	_sriCurrentRecord.vVocabulary = NULL;
	_sriCurrentRecord.iId = RECORD_NONE;
	_qtLearning.start(0);

	EnableControls();
#ifdef FREE
	_umwMainWindow.qlCategory->show();
#else
    _umwMainWindow.qlCategory->setVisible(_sSettings.GetShowCategoryName());
	_umwMainWindow.qhblRecordControls->parentWidget()->setVisible(_sSettings.GetShowRecordControls());
#endif
} // on_qaStart_triggered

const void MainWindow::on_qaStop_triggered(bool checked /* false */)
{
	_qtLearning.stop();
	_qpbTimer.setMaximum(1);
    _qpbTimer.setValue(0);
	EnableControls();

    _umwMainWindow.qlLanguage1->hide();
	_umwMainWindow.qtbWindow1->clear();
    _umwMainWindow.qlLanguage2->hide();
	_umwMainWindow.qtbWindow2->clear();
    _umwMainWindow.qlCategory->hide();
#ifndef FREE
	_umwMainWindow.qhblRecordControls->parentWidget()->hide();
#endif
} // on_qaStop_triggered

#ifndef FREE
const void MainWindow::on_qcbRecordEnabled_clicked(bool checked /* false */)
{
	SetRecordEnabled(checked);
} // on_qcbRecordEnabled_clicked

const void MainWindow::on_qmTray_triggered(QAction *action)
{
    /*if (action == _qaTrayManage) {
        on_qaManage_triggered();
    } else {*/
        if (action == _qaTraySettings) {
            on_qaSettings_triggered();
        } else {
	        if (action == _qaTrayExit) {
		        close();
	        } // if
        } // if else
    //} // if else
} // on_qmTray_triggered

const void MainWindow::on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick && isMinimized()) {
		setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
		showNormal();
	} // if
} // on_qstiTrayIcon_activated

const void MainWindow::on_qtbPriority1_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(1);
	} // if
} // on_qtbPriority1_clicked

const void MainWindow::on_qtbPriority2_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(2);
	} // if
} // on_qtbPriority2_clicked

const void MainWindow::on_qtbPriority3_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(3);
	} // if
} // on_qtbPriority3_clicked

const void MainWindow::on_qtbPriority4_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(4);
	} // if
} // on_qtbPriority4_clicked

const void MainWindow::on_qtbPriority5_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(5);
	} // if
} // on_qtbPriority5_clicked

const void MainWindow::on_qtbPriority6_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(6);
	} // if
} // on_qtbPriority6_clicked

const void MainWindow::on_qtbPriority7_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(7);
	} // if
} // on_qtbPriority7_clicked

const void MainWindow::on_qtbPriority8_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(8);
	} // if
} // on_qtbPriority8_clicked

const void MainWindow::on_qtbPriority9_clicked(bool checked /* false */)
{
	if (checked) {
		SetRecordPriority(9);
	} // if
} // on_qtbPriority9_clicked
#endif

const void MainWindow::on_qtLearning_timeout()
{
    if (_iTimeQuestion != TIME_NONE) {
        _iTimeQuestion--;
    } // if
    if (_iTimeAnswer != TIME_NONE) {
        _iTimeAnswer--;
    } // if

    _qpbTimer.setValue(_qpbTimer.value() - 1);

    if (_iTimeQuestion == 0) {
		_umwMainWindow.qaNext->setEnabled(false);
#ifndef FREE
		// disable answer
		_umwMainWindow.qaAnswer->setEnabled(false);
#endif

        if (_voOrganizer.GetRecordCount(true) == 0) {
            on_qaStop_triggered();
        } else {
            int iCategoryId;
			VocabularyOrganizer::sRecordInfo sriLastRecord = _sriCurrentRecord;
#ifndef FREE
			int iMaxCategoryPriority = qrand() % VocabularyTabWidget::CATEGORY_PRIORITY_MAX + 1;
            int iMaxRecordPriority = qrand() % PriorityDelegate::RECORD_PRIORITY_MAX + 1;

			int iNextRecordTry = 0;
#endif
            while (true) {
	            _sriCurrentRecord = _voOrganizer.GetRecordInfo(qrand() % _voOrganizer.GetRecordCount());
#ifndef FREE
				if (iNextRecordTry == MAX_NEXTRECORD_TRIES) {
					on_qaStop_triggered(false);
					return;
				} else {
					iNextRecordTry++;
				} // if else

				if ((!_sSettings.GetLearnDisabledWords() && !_sriCurrentRecord.vVocabulary->GetRecordEnabled(_sriCurrentRecord.iId)) || GetRecordPriority() > iMaxRecordPriority) {
					continue;
				} // if
#endif

                iCategoryId = _sriCurrentRecord.vVocabulary->GetRecordCategory(_sriCurrentRecord.iId);
#ifndef FREE
                if (_sriCurrentRecord.vVocabulary->GetCategoryEnabled(iCategoryId) && _sriCurrentRecord.vVocabulary->GetCategoryPriority(iCategoryId) <= iMaxCategoryPriority  && (_sriCurrentRecord.vVocabulary->GetRecordCount(true) == 1 || _sriCurrentRecord.vVocabulary != sriLastRecord.vVocabulary || _sriCurrentRecord.iId != sriLastRecord.iId)) {
#endif
                    break;
#ifndef FREE
                } // if
#endif
            } // while
/*#ifdef _DEBUG
            qDebug("Current word: %d", _sriCurrentRecord.iId);
#endif*/

			// answer time
			_iTimeAnswer = _sSettings.GetWaitForAnswer();
			// progress
			_qpbTimer.setMaximum(_iTimeAnswer);
			_qpbTimer.setValue(_iTimeAnswer);

            // question parameters
	        _saCurrentAnswer.iWord = _sriCurrentRecord.iId;
	        _saCurrentAnswer.bDirectionSwitched = GetLearningDirection();

            // gui
		    QString qsLang1 = GetLanguageText(_saCurrentAnswer.bDirectionSwitched, false);
		    if (qsLang1.isEmpty()) {
			    _umwMainWindow.qlLanguage1->hide();
		    } else {
#ifdef FREE
				_umwMainWindow.qlLanguage1->show();
#else
			    _umwMainWindow.qlLanguage1->setVisible(_sSettings.GetShowLanguageNames());
#endif
			    _umwMainWindow.qlLanguage1->setText(qsLang1);
		    } // if else
		    QString qsLang2 = GetLanguageText(_saCurrentAnswer.bDirectionSwitched, true);
		    if (qsLang2.isEmpty()) {
			    _umwMainWindow.qlLanguage2->hide();
		    } else {
#ifdef FREE
				_umwMainWindow.qlLanguage2->show();
#else
			    _umwMainWindow.qlLanguage2->setVisible(_sSettings.GetShowLanguageNames());
#endif
			    _umwMainWindow.qlLanguage2->setText(qsLang2);
		    } // if else
	        _umwMainWindow.qtbWindow1->setText(GetLearningText(TemplateLearning, _saCurrentAnswer.bDirectionSwitched, false));
	        _umwMainWindow.qtbWindow2->clear();
            _umwMainWindow.qlCategory->setText(_sriCurrentRecord.vVocabulary->GetCategoryName(iCategoryId));
#ifndef FREE
			SetupRecordControls();
#endif

#ifndef FREE
		    // tray
		    if (_sSettings.GetSystemTrayIcon() && _sSettings.GetShowWordsInTrayBalloon()) {
			    ShowTrayBalloon(_saCurrentAnswer.bDirectionSwitched, false);
		    } // if

            // sound
	        if (_sSettings.GetNewWordSound() && !_sSettings.GetMute()) {
                if (_sSettings.GetNewWordSoundType() == Settings::NewWordSoundTypeSystem) {
		            QApplication::beep();
                } else {
                    QSound::play(_sSettings.GetNewWordSoundFile());
                } // if else
	        } // if

            // flash
	        if (_sSettings.GetNewWordFlash()) {
                QString qsStyleSheet = _umwMainWindow.qtbWindow1->styleSheet();

		        for (int iI = 0; iI < FLASH_COUNT && !_qsQueuedAnswers.contains(_sriCurrentRecord.iId); iI++) {
                    _umwMainWindow.qtbWindow1->setStyleSheet(QString("QAbstractScrollArea { background-color: %1 }").arg(_sSettings.GetColorFlash()));
			        QTest::qWait(FLASH_WAIT);
                    _umwMainWindow.qtbWindow1->setStyleSheet(qsStyleSheet);
			        if (iI < FLASH_COUNT - 1) {
				        QTest::qWait(FLASH_WAIT);
			        } // if
		        } // for
	        } // if

            // speech
            if (_sSettings.GetNewWordSound() && !_qsQueuedAnswers.contains(_sriCurrentRecord.iId)) {
                QTest::qWait(SAY_BEEP_WAIT);
                Say(_saCurrentAnswer.bDirectionSwitched, false);
            } // if
#endif

            // next question time
            _iTimeQuestion = _sSettings.GetWordsFrequency();
            _qsQueuedAnswers.insert(_saCurrentAnswer.iWord);

#ifndef FREE
            // enable answer
            _umwMainWindow.qaAnswer->setEnabled(true);
#endif
        } // if else
    } // if

    if (_iTimeAnswer == 0 && _umwMainWindow.qaStop->isEnabled()) {
        ShowAnswer();
    } // if else

	if (!_qtLearning.isActive() && _umwMainWindow.qaStop->isEnabled()) {
		_qtLearning.start(MILISECONDS_PER_SECOND);
		_umwMainWindow.qaNext->setEnabled(true);
	} // if
} // on_qtLearning_timeout

const void MainWindow::on_ucUpdateChecker_Finished()
{
	if (_ucUpdateChecker.GetCheckResult() != QNetworkReply::NoError) {
		return;
	} // if

	if (_ucUpdateChecker.IsUpdateAvailable()) {
		QString qsText = tr("New version %1 of Vocabulary Master available at <a href=\"http://vocabulary-master.cz/\">homepage</a>.").arg(_ucUpdateChecker.GetUpdateVersion());
		QMessageBox::information(this, tr("Update available"), qsText);
	} // if
} // on_ucUpdateChecker_Finished

const void MainWindow::OpenVocabulary(
#ifndef FREE
    const bool &pCurrentRecord
#endif
    )
{
    VocabularyManagerDialog vmdManager(_sriCurrentRecord.vVocabulary,
#ifndef FREE
        &_sSettings,
        &_pPlugins,
#endif
        this);
#ifndef FREE
    if (pCurrentRecord) {
        vmdManager.ExecOnRecord(_sriCurrentRecord.iId);
    } else {
#endif
        vmdManager.exec();
#ifndef FREE
    } // if else
#endif

	_umwMainWindow.qaStart->setEnabled(_voOrganizer.IsOpen() && !_qtLearning.isActive() && _voOrganizer.GetRecordCount() > 0);
    RefreshStatusBar();
} // OpenVocabulary

const void MainWindow::RefreshStatusBar()
{
    if (!_voOrganizer.IsOpen()) {
        _qlVocabularyStatus.setText("");
    } else {
        _qlVocabularyStatus.setText(tr("%1 vocabularies, %2/%3 records").arg(_voOrganizer.GetVocabularyCount()).arg(_voOrganizer.GetRecordCount(true)).arg(_voOrganizer.GetRecordCount()));
    } // if else
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
	    VocabularyDatabase::eFieldLanguage eflLanguage;
	    if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		    eflLanguage = VocabularyDatabase::FieldLanguageLeft;
	    } else {
		    eflLanguage = VocabularyDatabase::FieldLanguageRight;
	    } // if else

        // get text to speech
        QString qsText;
        foreach (int iFieldId, _sriCurrentRecord.vVocabulary->GetFieldIds()) {
            if (_sriCurrentRecord.vVocabulary->GetFieldLanguage(iFieldId) == eflLanguage && _sriCurrentRecord.vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeSpeech)) {
                if (!qsText.isEmpty()) {
                    qsText += ' ';
                } // if
                qsText += _sriCurrentRecord.vVocabulary->GetDataText(_sriCurrentRecord.iId, iFieldId);
            } // if
        } // foreach

        if (!qsText.isEmpty()) {
			TTSInterface::eTTSPlugin etpSpeech = _sriCurrentRecord.vVocabulary->GetLanguageSpeech(eflLanguage);
	        if (etpSpeech != TTSInterface::TTPluginNone) {
		        TTSInterface *tiPlugin = _pPlugins.GetTTSPlugin(etpSpeech);
                if (tiPlugin) {
					QString qsVoice = _sriCurrentRecord.vVocabulary->GetLanguageVoice(eflLanguage);
		            tiPlugin->Say(qsVoice, qsText);
                } // if
	        } // if
        } // if
    } // if
} // Say
#endif

const void MainWindow::SetLayout()
{
#ifndef FREE
    if (_qhblInner) {
        _qhblInner->deleteLater();
        _qhblInner = NULL;
    } // if
#endif
    if (_umwMainWindow.qwCentral->layout()) {
        delete _umwMainWindow.qwCentral->layout();
    } // if

    QVBoxLayout *qvblMain = new QVBoxLayout(_umwMainWindow.qwCentral);
#ifndef FREE
    if (_sSettings.GetHorizontalLayout()) {
        qvblMain->addWidget(_umwMainWindow.qlCategory);
		qvblMain->addWidget(_umwMainWindow.qhblRecordControls->parentWidget());
        QHBoxLayout *qhblInner = new QHBoxLayout;
        qhblInner->addWidget(_umwMainWindow.qvblQuestion->parentWidget());
        qhblInner->addWidget(_umwMainWindow.qvblAnswer->parentWidget());
        qvblMain->addLayout(qhblInner);
    } else {
#endif
        qvblMain->addWidget(_umwMainWindow.qvblQuestion->parentWidget());
        qvblMain->addWidget(_umwMainWindow.qlCategory);
#ifndef FREE
		qvblMain->addWidget(_umwMainWindow.qhblRecordControls->parentWidget());
#endif
        qvblMain->addWidget(_umwMainWindow.qvblAnswer->parentWidget());
#ifndef FREE
    } // if else
#endif
} // SetLayout

#ifndef FREE
const void MainWindow::SetRecordEnabled(const bool &pEnabled)
{
	foreach (int iFieldId, _sriCurrentRecord.vVocabulary->GetFieldIds()) {
		if (_sriCurrentRecord.vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
			VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _sriCurrentRecord.vVocabulary->GetFieldBuiltIn(iFieldId);
			switch (efbBuiltIn) {
				case VocabularyDatabase::FieldBuiltInEnabled:
					_sriCurrentRecord.vVocabulary->SetDataText(_sriCurrentRecord.iId, iFieldId, QString::number(pEnabled ? Qt::Checked : Qt::Unchecked));
					return;
			} // switch
		} // if
	} // foreach
} // SetRecordEnabled

const void MainWindow::SetRecordPriority(const int &pPriority)
{
	foreach (int iFieldId, _sriCurrentRecord.vVocabulary->GetFieldIds()) {
		if (_sriCurrentRecord.vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
			VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _sriCurrentRecord.vVocabulary->GetFieldBuiltIn(iFieldId);
			switch (efbBuiltIn) {
				case VocabularyDatabase::FieldBuiltInPriority:
					_sriCurrentRecord.vVocabulary->SetDataText(_sriCurrentRecord.iId, iFieldId, QString::number(pPriority));
					return;
			} // switch
		} // if
	} // foreach
} // SetRecordPriority

const void MainWindow::SetupRecordControls() const
{
	int iPriority = GetRecordPriority();
	_umwMainWindow.qtbPriority1->setChecked(iPriority == 1);
	_umwMainWindow.qtbPriority2->setChecked(iPriority == 2);
	_umwMainWindow.qtbPriority3->setChecked(iPriority == 3);
	_umwMainWindow.qtbPriority4->setChecked(iPriority == 4);
	_umwMainWindow.qtbPriority5->setChecked(iPriority == 5);
	_umwMainWindow.qtbPriority6->setChecked(iPriority == 6);
	_umwMainWindow.qtbPriority7->setChecked(iPriority == 7);
	_umwMainWindow.qtbPriority8->setChecked(iPriority == 8);
	_umwMainWindow.qtbPriority9->setChecked(iPriority == 9);
	_umwMainWindow.qcbRecordEnabled->setChecked(_sriCurrentRecord.vVocabulary->GetRecordEnabled(_sriCurrentRecord.iId));
} // SetupRecordControls
#endif

const void MainWindow::ShowAnswer()
{
    if (_saCurrentAnswer.iWord == _sriCurrentRecord.iId) {
        _qsQueuedAnswers.clear();

#ifndef FREE
        // answer
        _umwMainWindow.qaAnswer->setEnabled(false);
#endif

        // gui
        _umwMainWindow.qtbWindow2->setText(GetLearningText(TemplateLearning, _saCurrentAnswer.bDirectionSwitched, true));
        _umwMainWindow.qtbWindow2->repaint();

#ifndef FREE
        // tray
        if (_sSettings.GetSystemTrayIcon() && _sSettings.GetShowWordsInTrayBalloon()) {
            ShowTrayBalloon(_saCurrentAnswer.bDirectionSwitched, true);
        } // if

        // speech
        Say(_saCurrentAnswer.bDirectionSwitched, true);
#endif

        // progress
        _qpbTimer.setMaximum(_iTimeQuestion);
        _qpbTimer.setValue(_iTimeQuestion);
    } // if
} // ShowAnswer

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