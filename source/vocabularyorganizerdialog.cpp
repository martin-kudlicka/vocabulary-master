#include "vocabularyorganizerdialog.h"
#include <QtGui/QFileDialog>

#ifndef TRY
const QString VOCABULARY_SUFFIX = "sl3";
const QString VOCABULARY_FILTER = QT_TRANSLATE_NOOP("MainWindow", "Vocabulary (*." + VOCABULARY_SUFFIX + ")");
#endif

const void VocabularyOrganizerDialog::on_qpbNew_clicked(bool checked /* false */)
{
#ifndef TRY
	QString qsInitPath;
	if (_voOrganizer->GetVocabularyCount() > 0) {
		qsInitPath = QFileInfo(_voOrganizer->GetVocabularyInfo(0)->GetVocabularyFile()).absolutePath();
	} else {
		qsInitPath = QDir::homePath();
	} // if else

	QFileDialog qfdNew(this, tr("Create new vocabulary"), qsInitPath, VOCABULARY_FILTER);
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
} // on_qpbNew_clicked

const void VocabularyOrganizerDialog::on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	_qdvmOrganizer.qpbClose->setEnabled(_qdvmOrganizer.qtvVocabularies->currentIndex().isValid());
} // on_qtvVocabulariesSelectionModel_selectionChanged

VocabularyOrganizerDialog::VocabularyOrganizerDialog(VocabularyOrganizer *pOrganizer, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _vomModel(pOrganizer)
{
	_voOrganizer = pOrganizer;

	_qdvmOrganizer.setupUi(this);

	_qdvmOrganizer.qtvVocabularies->setModel(&_vomModel);
	connect(_qdvmOrganizer.qtvVocabularies->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvVocabulariesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // VocabularyOrganizerDialog