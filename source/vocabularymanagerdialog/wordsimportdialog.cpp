#include "vocabularymanagerdialog/wordsimportdialog.h"

#include <QtGui/QMessageBox>

int WordsImportDialog::exec()
{
	// check file according to filter
	foreach (ImpInterface *iiPlugin, _pPlugins->GetImpPlugins()) {
		if (_qsFilter == iiPlugin->GetFilter()) {
			_iiPlugin = iiPlugin;
			break;
		} // if
	} // foreach

	bool bOpen = false;
	if (_iiPlugin) {
		bOpen = _iiPlugin->Open(_qsFile);
	} // if

	if (bOpen) {
		return QDialog::exec();
	} else {
		QMessageBox::critical(this, tr("Words import"), tr("Can't import data from selected file."));
		return QDialog::Rejected;
	} // if else
} // exec

WordsImportDialog::WordsImportDialog(const QString &pFile, const QString &pFilter, const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_qsFile = pFile;
	_qsFilter = pFilter;
	_vVocabulary = pVocabulary;
	_pPlugins = pPlugins;

	_iiPlugin = NULL;

	_qdwiWordsImport.setupUi(this);
} // WordsImportDialog