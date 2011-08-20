#include "vocabularymanagerdialog/wordsexportdialog.h"

WordsExportDialog::WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_vVocabulary = pVocabulary;
	_teplExpPlugins = pExpPlugins;

	_qdweWordsExport.setupUi(this);
} // WordsExportDialog