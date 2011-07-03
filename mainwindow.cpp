#include "mainwindow.h"

#include "settingsdialog.h"
#include <QtGui/QFileDialog>
#include "vocabularymanagerdialog.h"
#include <QtCore/QTimer>

const QString FORMAT_TEXT = "<center style=\"font-size:xx-large\">%1</center>";
const QString VOCABULARY_SUFFIX = "sl3";
const QString VOCABULARY_FILTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary (*." + VOCABULARY_SUFFIX + ")");

const void MainWindow::ApplySettings()
{
    SetLayout();

    if (_sSettings.GetAlwaysOnTop()) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    } // if else
    if (_sSettings.GetWindowX() != Settings::DEFAULT_DIMENSION) {
        setGeometry(_sSettings.GetWindowX(), _sSettings.GetWindowY(), _sSettings.GetWindowWidth(), _sSettings.GetWindowHeight());
    } // if

	show();
} // ApplySettings

const void MainWindow::EnableControls()
{
	// menu
    _umwMainWindow.qmVocabulary->setEnabled(_vVocabulary.IsOpen());

	// tool bar
	_umwMainWindow.qaStart->setEnabled(_vVocabulary.IsOpen() && _iTimerLearing == 0);
	_umwMainWindow.qaStop->setEnabled(_vVocabulary.IsOpen() && _iTimerLearing != 0);
	_umwMainWindow.qaNext->setEnabled(_vVocabulary.IsOpen() && _iTimerLearing != 0);
} // EnableControls

const QString MainWindow::GetLangText(const bool &pAnswer) const
{
	if ((!_sSettings.GetSwitchLearningDirection() && !pAnswer) || (_sSettings.GetSwitchLearningDirection() && pAnswer)) {
		return COLUMN_LANG1;
	} else {
		return COLUMN_LANG2;
	} // if else
} // GetLangText

MainWindow::~MainWindow()
{
    _sSettings.SetVocabularyFile(_vVocabulary.GetVocabularyFile());
    _sSettings.SetWindowX(geometry().x());
    _sSettings.SetWindowY(geometry().y());
    _sSettings.SetWindowHeight(geometry().height());
    _sSettings.SetWindowWidth(geometry().width());
} // ~MainWindow

MainWindow::MainWindow(QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QMainWindow(pParent, pFlags)
{
	_iCurrentWord = -1;
	_iTimerLearing = 0;

	_umwMainWindow.setupUi(this);

    ApplySettings();

    _vVocabulary.Open(_sSettings.GetVocabularyFile());

    EnableControls();
} // MainWindow

const void MainWindow::on_qaManage_triggered(bool checked /* false */)
{
    VocabularyManagerDialog vmdManager(&_vVocabulary, this);
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
        ApplySettings();
    } // if
} // on_qaSettings_triggered

const void MainWindow::on_qaStart_triggered(bool checked /* false */)
{
	_iTimerLearing = startTimer(_sSettings.GetWordsFrequency() * MILISECONDS_PER_SECOND);
	EnableControls();
	timerEvent(NULL);
} // on_qaStart_triggered

const void MainWindow::on_qaStop_triggered(bool checked /* false */)
{
	killTimer(_iTimerLearing);
	_iTimerLearing = 0;
	EnableControls();

	_umwMainWindow.qtbWindow1->clear();
	_umwMainWindow.qtbWindow2->clear();
} // on_qaStop_triggered

const void MainWindow::OnShowTranslation()
{
	if (_iTimerLearing != 0) {
		_umwMainWindow.qtbWindow2->setText(FORMAT_TEXT.arg(_vVocabulary.GetWord(_iCurrentWord, GetLangText(true))));
	} // if
} // OnShowTranslation

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
	_iCurrentWord = qrand() % _vVocabulary.GetWordCount();
	_umwMainWindow.qtbWindow1->setText(FORMAT_TEXT.arg(_vVocabulary.GetWord(_iCurrentWord, GetLangText(false))));

	if (_sSettings.GetNewWordSound()) {
		QApplication::beep();
	} // if

	QTimer::singleShot(_sSettings.GetWaitForAnswer() * MILISECONDS_PER_SECOND, this, SLOT(OnShowTranslation()));
} // timerEvent