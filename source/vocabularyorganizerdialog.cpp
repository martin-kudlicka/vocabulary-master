#include "vocabularyorganizerdialog.h"
#include <QtGui/QFileDialog>

#ifndef TRY
const QString VOCABULARY_SUFFIX = "sl3";
const QString VOCABULARY_FILTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary (*." + VOCABULARY_SUFFIX + ")");

void VocabularyOrganizerDialog::accept()
{
	_voOrganizer->SaveAll();
	QDialog::accept();
} // accept
#endif

#ifdef FREE
const void VocabularyOrganizerDialog::EnableControls() const
{
	_qdvmOrganizer.qpbOpen->setEnabled(_voOrganizer->GetVocabularyCount() <= 1);
} // EnableControls
#endif

const QString VocabularyOrganizerDialog::GetOpenPath() const
{
	if (_voOrganizer->GetVocabularyCount() > 0) {
		return QFileInfo(_voOrganizer->GetVocabularyInfo(0)->GetVocabularyFile()).absolutePath();
	} else {
		return QDir::homePath();
	} // if else
} // GetOpenPath

const void VocabularyOrganizerDialog::on_qpbClose_clicked(bool checked /* false */)
{
	int iIndex = _qdvmOrganizer.qtvVocabularies->currentIndex().row();

	_voOrganizer->Close(iIndex);
	_vomModel.RemoveRow(iIndex);

	on_qtvVocabulariesSelectionModel_selectionChanged(QItemSelection(), QItemSelection());

#ifdef FREE
	EnableControls();
#endif
} // on_qpbClose_clicked

const void VocabularyOrganizerDialog::on_qpbNew_clicked(bool checked /* false */)
{
#ifndef TRY
	QFileDialog qfdNew(this, tr("Create new vocabulary"), GetOpenPath(), VOCABULARY_FILTER);
	qfdNew.setAcceptMode(QFileDialog::AcceptSave);
	if (qfdNew.exec() == QDialog::Accepted) {
		QFileInfo qfiFile(qfdNew.selectedFiles().at(0));
		QString qsFile;
		if (qfiFile.suffix() != VOCABULARY_SUFFIX) {
			qsFile = qfdNew.selectedFiles().at(0) + "." + VOCABULARY_SUFFIX;
		} else {
			qsFile = qfdNew.selectedFiles().at(0);
		} // if else

		_voOrganizer->New(qsFile);
		_vomModel.AddRow();
	} // if
#else
	_voOrganizer->New();
	_vomModel.AddRow();
#endif

#ifdef FREE
	EnableControls();
#endif
} // on_qpbNew_clicked

#ifndef TRY
const void VocabularyOrganizerDialog::on_qpbOpen_clicked(bool checked /* false */)
{
	QString qsFile = QFileDialog::getOpenFileName(this, tr("Open vocabulary"), GetOpenPath(), VOCABULARY_FILTER);
	if (!qsFile.isEmpty()) {
		_voOrganizer->Open(qsFile, this);
		_vomModel.AddRow();
	} // if

#ifdef FREE
	EnableControls();
#endif
} // on_qpbOpen_clicked
#endif

const void VocabularyOrganizerDialog::on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	_qdvmOrganizer.qpbClose->setEnabled(_qdvmOrganizer.qtvVocabularies->currentIndex().isValid());
} // on_qtvVocabulariesSelectionModel_selectionChanged

#ifndef TRY
void VocabularyOrganizerDialog::reject()
{
	accept();
} // reject
#endif

VocabularyOrganizerDialog::VocabularyOrganizerDialog(VocabularyOrganizer *pOrganizer, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _vomModel(pOrganizer)
{
	_voOrganizer = pOrganizer;

	_qdvmOrganizer.setupUi(this);
#ifdef FREE
	EnableControls();
#elif defined TRY
	_qdvmOrganizer->qpbOpen->deleteLater();
#endif

	_qdvmOrganizer.qtvVocabularies->setModel(&_vomModel);
	connect(_qdvmOrganizer.qtvVocabularies->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // VocabularyOrganizerDialog