#ifndef WORDSEXPORTDIALOG_H
#define WORDSEXPORTDIALOG_H

#include <ui_wordsexportdialog.h>

#include "vocabulary.h"
#include "plugins.h"

class WordsExportDialog : public QDialog
{
	Q_OBJECT

	public:
		WordsExportDialog(const Vocabulary *pVocabulary, const Plugins::tExpPluginList &pExpPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		Plugins::tExpPluginList _teplExpPlugins;
		Ui::qdWordsExport _qdweWordsExport;
		const Vocabulary *_vVocabulary;
}; // WordsExportDialog

#endif // WORDSEXPORTDIALOG_H