#include "mainwindow.h"

#include "settingsdialog.h"
#include <QtGui/QFileDialog>
#include "vocabularymanagerdialog.h"
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QTest>

const QString FORMAT_NOTE = "<center style=\"font-size:small\">%1</center>";
const QString FORMAT_WORD = "<center style=\"font-size:xx-large\">%1</center>";
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

	show();
} // ApplySettings

const void MainWindow::EnableControls()
{
	// menu
    _umwMainWindow.qmVocabulary->setEnabled(_vVocabulary.IsOpen());

	// tool bar
	_umwMainWindow.qaStart->setEnabled(_vVocabulary.IsOpen() && _iTimerQuestion == 0 && _vVocabulary.GetWordCount() > 0);
	_umwMainWindow.qaStop->setEnabled(_vVocabulary.IsOpen() && _iTimerQuestion != 0);
	_umwMainWindow.qaNext->setEnabled(_vVocabulary.IsOpen() && _iTimerQuestion != 0);
} // EnableControls

const QString MainWindow::GetLangColumn(const bool &pDirectionSwitched, const bool &pAnswer) const
{
	if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		return COLUMN_LANG1;
	} else {
		return COLUMN_LANG2;
	} // if else
} // GetLangColumn

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
    QString qsWord = FORMAT_WORD.arg(_vVocabulary.GetWord(_iCurrentWord, GetLangColumn(pDirectionSwitched, pAnswer)));
    QString qsNote = _vVocabulary.GetNote(_iCurrentWord, GetNoteColumn(pDirectionSwitched, pAnswer));
    if (!qsNote.isEmpty()) {
        qsNote = FORMAT_NOTE.arg(qsNote);
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

	_umwMainWindow.setupUi(this);

	_pPlugins.Load();
	_pPlugins.Initialize();
    ApplySettings(true);

    _vVocabulary.Open(_sSettings.GetVocabularyFile());

    EnableControls();

	if (_sSettings.GetStartLearningOnStartup() && _vVocabulary.IsOpen()) {
		on_qaStart_triggered();
	} // if
} // MainWindow

const void MainWindow::on_qaManage_triggered(bool checked /* false */)
{
    VocabularyManagerDialog vmdManager(&_vVocabulary, &_pPlugins, this);
    vmdManager.exec();
} // on_qaManage_triggered

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

	_umwMainWindow.qtbWindow1->clear();
	_umwMainWindow.qtbWindow2->clear();
} // on_qaStop_triggered

const void MainWindow::Say(const bool &pDirectionSwitched, const bool &pAnswer) const
{
	QString qsSpeech, qsVoice;
	if ((!pDirectionSwitched && !pAnswer) || (pDirectionSwitched && pAnswer)) {
		qsSpeech = KEY_SPEECH1;
		qsVoice = KEY_VOICE1;
	} else {
		qsSpeech = KEY_SPEECH2;
		qsVoice = KEY_VOICE2;
	} // if else

	int iSpeech = _vVocabulary.GetSettings(qsSpeech).toInt();
	if (iSpeech != TTSInterface::TTPluginNone) {
		TTSInterface *tiPlugin = _pPlugins.GetPlugin(static_cast<TTSInterface::eTTSPlugin>(iSpeech));
		tiPlugin->Say(qsVoice, _vVocabulary.GetWord(_iCurrentWord, GetLangColumn(pDirectionSwitched, pAnswer)));
	} // if
} // Say

const void MainWindow::SetLayout()
{
    QBoxLayout *qblLayout;

    if (_sSettings.GetHorizontalLayout()) {
        qblLayout = new QHBoxLayout(this);
    } else {
        qblLayout = new QVBoxLayout(this);
    } // if else

    qblLayout->addWidget(_umwMainWindow.qtbWindow1);
    qblLayout->addWidget(_umwMainWindow.qtbWindow2);

    if (_umwMainWindow.qwCentral->layout()) {
        delete _umwMainWindow.qwCentral->layout();
    } // if
    _umwMainWindow.qwCentral->setLayout(qblLayout);
} // SetLayout

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _iTimerQuestion) {
	    _iCurrentWord = qrand() % _vVocabulary.GetWordCount();

        // question parameters
        sAnswer saAnswer;
	    saAnswer.iWord = _iCurrentWord;
	    saAnswer.bDirectionSwitched = GetLearningDirection();

        // gui
	    _umwMainWindow.qtbWindow1->setText(GetLearningText(saAnswer.bDirectionSwitched, false));
	    _umwMainWindow.qtbWindow2->clear();

        // sound
	    if (_sSettings.GetNewWordSound()) {
		    QApplication::beep();
	    } // if
        // flash
	    if (_sSettings.GetNewWordFlash()) {
		    QPalette qpOriginal = _umwMainWindow.qtbWindow1->palette();

		    for (int iI = 0; iI < FLASH_COUNT; iI++) {
                QPalette qpPalette;

                qpPalette.setColor(QPalette::Active, QPalette::Base, Qt::green);
                _umwMainWindow.qtbWindow1->setPalette(qpPalette);
			    QTest::qWait(FLASH_WAIT);
			    _umwMainWindow.qtbWindow1->setPalette(qpOriginal);
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
    } else {
        // check for answer timer
        if (_taHash.contains(event->timerId())) {
            sAnswer saAnswer = _taHash.value(event->timerId());
            _taHash.remove(event->timerId());

            if (saAnswer.iWord == _iCurrentWord) {
                // gui
                _umwMainWindow.qtbWindow2->setText(GetLearningText(saAnswer.bDirectionSwitched, true));
                _umwMainWindow.qtbWindow2->repaint();
                // speech
                Say(saAnswer.bDirectionSwitched, true);
            } // if
        } // if
    } // if else
} // timerEvent