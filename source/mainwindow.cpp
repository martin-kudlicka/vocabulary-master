#include "mainwindow.h"

#include "settingsdialog.h"
#include <QtGui/QFileDialog>
#include "vocabularymanagerdialog.h"
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QTest>

const QString FORMAT_NOTE = "<center style=\"font-size:%1px\">%2</center>";
const QString FORMAT_WORD = "<center style=\"font-size:%1px\">%2</center>";
const QString VOCABULARY_SUFFIX = "sl3";
const QString VOCABULARY_FILTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary (*." + VOCABULARY_SUFFIX + ")");

const void MainWindow::ApplySettings(const bool &pStartup)
{
    SetLayout();

    if (_sSettings.GetAlwaysOnTop()) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    } // if else
    if (pStartup && _sSettings.GetWindowX() != Settings::DEFAULT_DIMENSION) {
        setGeometry(_sSettings.GetWindowX(), _sSettings.GetWindowY(), _sSettings.GetWindowWidth(), _sSettings.GetWindowHeight());
    } // if

    _qstiTrayIcon.setVisible(_sSettings.GetSystemTrayIcon());

	show();
} // ApplySettings

const void MainWindow::CreateTrayMenu()
{
	_qaTrayExit = _qmTray.addAction(tr("Exit"));

	connect(&_qmTray, SIGNAL(triggered(QAction *)), SLOT(on_qmTray_triggered(QAction *)));
	_qstiTrayIcon.setContextMenu(&_qmTray);
} // CreateTrayMenu

const void MainWindow::EnableControls()
{
	// menu
    _umwMainWindow.qmVocabulary->setEnabled(_vVocabulary.IsOpen());

	// tool bar
	_umwMainWindow.qaStart->setEnabled(_vVocabulary.IsOpen() && _iTimerQuestion == 0 && _vVocabulary.GetWordCount() > 0);
	_umwMainWindow.qaStop->setEnabled(_iTimerQuestion != 0);
	_umwMainWindow.qaNext->setEnabled(_iTimerQuestion != 0);
    _umwMainWindow.qaAnswer->setEnabled(_iTimerAnswer != 0);
} // EnableControls

bool MainWindow::event(QEvent *event)
{
	if (event->type() == QEvent::WindowStateChange) {
		if (isMinimized() && _sSettings.GetSystemTrayIcon() && _sSettings.GetMinimizeToTray()) {
			setWindowFlags(windowFlags() | Qt::CustomizeWindowHint); // just add some flag to hide window
		} // if
	} // if

	return QMainWindow::event(event);
} // event

const QString MainWindow::GetLangColumn(const bool &pDirectionSwitched, const bool &pAnswer) const
{
	if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		return COLUMN_LANG1;
	} else {
		return COLUMN_LANG2;
	} // if else
} // GetLangColumn

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

const QString MainWindow::GetLearningText(const bool &pDirectionSwitched, const bool &pAnswer) const
{
    QString qsWord = FORMAT_WORD.arg(QString::number(_sSettings.GetFontSizeWord())).arg(_vVocabulary.GetWord(_iCurrentWord, GetLangColumn(pDirectionSwitched, pAnswer)));
    QString qsNote = _vVocabulary.GetNote(_iCurrentWord, GetNoteColumn(pDirectionSwitched, pAnswer));
    if (!qsNote.isEmpty()) {
        qsNote = FORMAT_NOTE.arg(QString::number(_sSettings.GetFontSizeNote())).arg(qsNote);
        return qsWord + qsNote;
    } else {
        return qsWord;
    } // if else
} // GetLearningText

const QString MainWindow::GetNoteColumn(const bool &pDirectionSwitched, const bool &pAnswer) const
{
    if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
        return COLUMN_NOTE1;
    } else {
        return COLUMN_NOTE2;
    } // if else
} // GetNoteColumn

MainWindow::~MainWindow()
{
    _sSettings.SetVocabularyFile(_vVocabulary.GetVocabularyFile());
    _sSettings.SetWindowX(geometry().x());
    _sSettings.SetWindowY(geometry().y());
    _sSettings.SetWindowHeight(geometry().height());
    _sSettings.SetWindowWidth(geometry().width());

	_pPlugins.Uninitialize();
} // ~MainWindow

MainWindow::MainWindow(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QMainWindow(pParent, pFlags)
{
	_iCurrentWord = -1;
    _iTimerAnswer = 0;
	_iTimerQuestion = 0;

	qsrand(QTime::currentTime().msec());

    // gui
	_umwMainWindow.setupUi(this);
	CreateTrayMenu();

    // plugins
	_pPlugins.Load();
	_pPlugins.Initialize();

    // settings
    ApplySettings(true);

    _vVocabulary.Open(_sSettings.GetVocabularyFile());

    // controls
    EnableControls();
    _umwMainWindow.qaMute->setChecked(_sSettings.GetMute());

    // learning
	if (_sSettings.GetStartLearningOnStartup() && _vVocabulary.IsOpen()) {
		on_qaStart_triggered();
	} // if

	// connections
	connect(&_qstiTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason)));
} // MainWindow

const void MainWindow::on_qaAnswer_triggered(bool checked /* false */)
{
    timerEvent(&QTimerEvent(_iTimerAnswer));
} // on_qaAnswer_triggered

const void MainWindow::on_qaManage_triggered(bool checked /* false */)
{
    VocabularyManagerDialog vmdManager(&_vVocabulary, &_pPlugins, this);
    vmdManager.exec();
} // on_qaManage_triggered

const void MainWindow::on_qaMute_toggled(bool checked)
{
    _sSettings.SetMute(checked);
} // on_qaMute_toggled

const void MainWindow::on_qaNew_triggered(bool checked /* false */)
{
    QFileDialog qfdNew(this, tr("Create new vocabulary"), QString(), VOCABULARY_FILTER);
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

        EnableControls();
    } // if
} // on_qaNew_triggered

const void MainWindow::on_qaNext_triggered(bool checked /* false */)
{
	on_qaStop_triggered();
	on_qaStart_triggered();
} // on_qaNext_triggered

const void MainWindow::on_qaOpen_triggered(bool checked /* false */)
{
    QString qsFile = QFileDialog::getOpenFileName(this, tr("Open vocabulary"), QString(), VOCABULARY_FILTER);
    if (!qsFile.isEmpty()) {
        _vVocabulary.Open(qsFile);

        EnableControls();
    } // if
} // on_qaOpen_triggered

const void MainWindow::on_qaSettings_triggered(bool checked /* false */)
{
	SettingsDialog sdDialog(&_sSettings);
    if (sdDialog.exec() == QDialog::Accepted) {
        ApplySettings(false);
    } // if
} // on_qaSettings_triggered

const void MainWindow::on_qaStart_triggered(bool checked /* false */)
{
	_iTimerQuestion = startTimer(_sSettings.GetWordsFrequency() * MILISECONDS_PER_SECOND);

	EnableControls();
    _umwMainWindow.qlLanguage1->setVisible(true);
    _umwMainWindow.qlLanguage2->setVisible(true);

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

    _umwMainWindow.qlLanguage1->setVisible(false);
	_umwMainWindow.qtbWindow1->clear();
    _umwMainWindow.qlLanguage2->setVisible(false);
	_umwMainWindow.qtbWindow2->clear();
} // on_qaStop_triggered

const void MainWindow::on_qmTray_triggered(QAction *action)
{
	if (action == _qaTrayExit) {
		close();
	} // if
} // on_qmTray_triggered

const void MainWindow::on_qstiTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick && isMinimized()) {
		setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
		showNormal();
	} // if
} // on_qstiTrayIcon_activated

const void MainWindow::Say(const bool &pDirectionSwitched, const bool &pAnswer) const
{
    if (!_sSettings.GetMute()) {
	    QString qsSpeech, qsVoice;
	    if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		    qsSpeech = KEY_SPEECH1;
		    qsVoice = KEY_VOICE1;
	    } else {
		    qsSpeech = KEY_SPEECH2;
		    qsVoice = KEY_VOICE2;
	    } // if else

	    int iSpeech = _vVocabulary.GetSettings(qsSpeech).toInt();
        qsVoice = _vVocabulary.GetSettings(qsVoice);
	    if (iSpeech != TTSInterface::TTPluginNone) {
		    TTSInterface *tiPlugin = _pPlugins.GetTTSPlugin(static_cast<TTSInterface::eTTSPlugin>(iSpeech));
            if (tiPlugin) {
		        tiPlugin->Say(qsVoice, _vVocabulary.GetWord(_iCurrentWord, GetLangColumn(pDirectionSwitched, pAnswer)));
            } // if
	    } // if
    } // if
} // Say

const void MainWindow::SetLayout()
{
    QBoxLayout *qblLayout;

    if (_umwMainWindow.qwCentral->layout()) {
        delete _umwMainWindow.qwCentral->layout();
    } // if

    if (_sSettings.GetHorizontalLayout()) {
        qblLayout = new QHBoxLayout(_umwMainWindow.qwCentral);
    } else {
        qblLayout = new QVBoxLayout(_umwMainWindow.qwCentral);
    } // if else

    qblLayout->addWidget(_umwMainWindow.qvblQuestion->parentWidget());
    qblLayout->addWidget(_umwMainWindow.qvblAnswer->parentWidget());
} // SetLayout

const void MainWindow::ShowTrayBalloon(const bool &pDirectionSwitched, const bool &pAnswer)
{
	QString qsText = _vVocabulary.GetWord(_iCurrentWord, GetLangColumn(pDirectionSwitched, false));
	if (pAnswer) {
		qsText += " -> " + _vVocabulary.GetWord(_iCurrentWord, GetLangColumn(pDirectionSwitched, true));
	} // if

	_qstiTrayIcon.showMessage("Vocabulary Master", qsText);
} // ShowTrayBalloon

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _iTimerQuestion) {
        int iLastWord = _iCurrentWord;
        while (true) {
	        _iCurrentWord = qrand() % _vVocabulary.GetWordCount();
            int iCategoryId = _vVocabulary.GetWordCategory(_iCurrentWord);
            if (_vVocabulary.GetCategoryEnabled(iCategoryId) && _iCurrentWord != iLastWord) {
                break;
            } // if
        } // while

        // question parameters
        sAnswer saAnswer;
	    saAnswer.iWord = _iCurrentWord;
	    saAnswer.bDirectionSwitched = GetLearningDirection();

        // gui
        _umwMainWindow.qlLanguage1->setText(GetLanguageText(saAnswer.bDirectionSwitched, false));
        _umwMainWindow.qlLanguage2->setText(GetLanguageText(saAnswer.bDirectionSwitched, true));
	    _umwMainWindow.qtbWindow1->setText(GetLearningText(saAnswer.bDirectionSwitched, false));
	    _umwMainWindow.qtbWindow2->clear();

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

		    for (int iI = 0; iI < FLASH_COUNT; iI++) {
                _umwMainWindow.qtbWindow1->setStyleSheet(QString("QAbstractScrollArea { background-color: %1 }").arg(_sSettings.GetColorFlash()));
			    QTest::qWait(FLASH_WAIT);
                _umwMainWindow.qtbWindow1->setStyleSheet(qsStyleSheet);
			    if (iI < FLASH_COUNT - 1) {
				    QTest::qWait(FLASH_WAIT);
			    } // if
		    } // for
	    } // if

        // speech
        if (_sSettings.GetNewWordSound()) {
            QTest::qWait(SAY_BEEP_WAIT);
            Say(saAnswer.bDirectionSwitched, false);
        } // if

        // answer timer
        _iTimerAnswer = startTimer(_sSettings.GetWaitForAnswer() * MILISECONDS_PER_SECOND);
        _taHash.insert(_iTimerAnswer, saAnswer);

        // answer
        _umwMainWindow.qaAnswer->setEnabled(true);
    } else {
        // check for answer timer
        if (_taHash.contains(event->timerId())) {
            killTimer(event->timerId());
            if (_iTimerAnswer == event->timerId()) {
                _iTimerAnswer = 0;
            } // if

            sAnswer saAnswer = _taHash.value(event->timerId());
            _taHash.remove(event->timerId());

            if (saAnswer.iWord == _iCurrentWord) {
                // answer
                _umwMainWindow.qaAnswer->setEnabled(false);

                // gui
                _umwMainWindow.qtbWindow2->setText(GetLearningText(saAnswer.bDirectionSwitched, true));
                _umwMainWindow.qtbWindow2->repaint();

				// tray
				if (_sSettings.GetSystemTrayIcon() && _sSettings.GetShowWordsInTrayBalloon()) {
					ShowTrayBalloon(saAnswer.bDirectionSwitched, true);
				} // if

                // speech
                Say(saAnswer.bDirectionSwitched, true);
            } // if
        } // if
    } // if else
} // timerEvent