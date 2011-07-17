#include "vocabularymanagerdialog/wordsimportdialog.h"

#include "vocabularymanagerdialog/wordsimportdialog/importfilter/importfilteranki.h"
#include <QtGui/QMessageBox>

// same order as WordsImportDialog::eFileType
const QStringList IMPORT_FILTERS = QStringList() << ("Anki (*.anki)");

WordsImportDialog::~WordsImportDialog()
{
	if (_ifImportFilter) {
		delete _ifImportFilter;
	} // if
} // ~WordsImportDialog

int WordsImportDialog::exec()
{
	// check file according to filter
	eFileType eftFilter = static_cast<eFileType>(IMPORT_FILTERS.indexOf(_qsFilter));
	switch (eftFilter) {
		case FileTypeAnki:
			_ifImportFilter = new ImportFilterAnki(_qsFile);
			break;
	} // switch

	if (_ifImportFilter->Open()) {
		return QDialog::exec();
	} else {
		QMessageBox::critical(this, tr("Words import"), tr("Can't import data from selected file."));
		return QDialog::Rejected;
	} // if else
} // exec

const QString WordsImportDialog::GetFilter()
{
	return IMPORT_FILTERS.join(";;");
} // GetFilter

WordsImportDialog::WordsImportDialog(const QString &pFile, const QString &pFilter, const Vocabulary *pVocabulary, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_qsFile = pFile;
	_qsFilter = pFilter;
	_vVocabulary = pVocabulary;

	_ifImportFilter = NULL;

	_qdwiWordsImport.setupUi(this);
} // WordsImportDialog