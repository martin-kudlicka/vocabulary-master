#include "mainwindow.h"

#include "settingsdialog.h"
#include <QtGui/QFileDialog>
#include "vocabularymanagerdialog.h"

const QString OPEN_CAPTION = QT_TRANSLATE_NOOP("MainWindow", "Create new vocabulary");
const QString VOCABULARY_FILTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary (*.sl3)");

const void MainWindow::ApplySettings()
{
    SetLayout();
} // ApplySettings

const void MainWindow::EnableControls()
{
    _umwMainWindow.qmVocabulary->setEnabled(_vVocabulary.IsOpen());
} // EnableControls

MainWindow::~MainWindow()
{
} // ~MainWindow

MainWindow::MainWindow(QWidget *parent /* 0 */, Qt::WindowFlags flags /* 0 */) : QMainWindow(parent, flags)
{
	_umwMainWindow.setupUi(this);

    ApplySettings();
    EnableControls();
} // MainWindow

const void MainWindow::on_qaManage_triggered(bool checked /* false */)
{
    VocabularyManagerDialog vmdManager(&_vVocabulary, this);
    vmdManager.exec();
} // on_qaManage_triggered

const void MainWindow::on_qaNew_triggered(bool checked /* false */)
{
    QFileDialog qfdNew(this, OPEN_CAPTION, QString(), VOCABULARY_FILTER);
    qfdNew.setAcceptMode(QFileDialog::AcceptSave);
    if (qfdNew.exec() == QDialog::Accepted) {
        QStringList qslFiles = qfdNew.selectedFiles();
        _vVocabulary.Open(qslFiles.at(0));

        EnableControls();
    } // if
} // on_qaNew_triggered

const void MainWindow::on_qaOpen_triggered(bool checked /* false */)
{
    QString qsFile = QFileDialog::getOpenFileName(this, OPEN_CAPTION, QString(), VOCABULARY_FILTER);
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