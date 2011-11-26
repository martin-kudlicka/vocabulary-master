#include "vocabularyorganizerdialog.h"
#ifndef TRY
# include <QtGui/QFileDialog>

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

#ifndef TRY
const QString VocabularyOrganizerDialog::GetOpenPath() const
{
	if (_voOrganizer->GetVocabularyCount() > 0) {
		VocabularyOrganizer::sVocabulary svVocabulary = _voOrganizer->GetVocabularyInfo(0);
		return QFileInfo(svVocabulary.sviVocabularyInfo.qsFile).absolutePath();
	} else {
		return QDir::homePath();
	} // if else
} // GetOpenPath
#endif

const void VocabularyOrganizerDialog::on_qpbClose_clicked(bool checked /* false */)
{
	int iIndex = _qdvmOrganizer.qtvVocabularies->currentIndex().row();

	_voOrganizer->Remove(iIndex);
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

		_voOrganizer->AddNew(qsFile);
		_vomModel.AddRow();
	} // if
#else
	_voOrganizer->AddNew();
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
		VocabularyOrganizer::sVocabulary svVocabulary;
		svVocabulary.sviVocabularyInfo.qsFile = qsFile;
#ifndef FREE
		svVocabulary.sviVocabularyInfo.bEnabled = true;
#endif

		_voOrganizer->AddExisting(svVocabulary, this);
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

VocabularyOrganizerDialog::VocabularyOrganizerDialog(VocabularyOrganizer *pOrganizer, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _vomModel(pOrganizer, this)
{
	_voOrganizer = pOrganizer;

	_qdvmOrganizer.setupUi(this);
#ifdef FREE
	EnableControls();
#elif defined TRY
	delete _qdvmOrganizer.qpbOpen;
#endif

	_qdvmOrganizer.qtvVocabularies->setModel(&_vomModel);

	_qdvmOrganizer.qtvVocabularies->header()->setResizeMode(VocabularyOrganizerModel::ColumnVocabularyFile, QHeaderView::Stretch);
#if !defined(FREE) && !defined(TRY)
	_qdvmOrganizer.qtvVocabularies->header()->setResizeMode(VocabularyOrganizerModel::ColumnEnabled, QHeaderView::ResizeToContents);
#endif

	connect(_qdvmOrganizer.qtvVocabularies->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // VocabularyOrganizerDialog