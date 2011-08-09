#include "vocabularymanagerdialog/wordsimportdialog.h"

#include <QtGui/QMessageBox>

int WordsImportDialog::exec()
{
    if (!_iiPlugin->Open(_qsFile)) {
        QMessageBox::critical(QApplication::activeWindow(), tr("Words import"), tr("Can't import data from selected file."));
        return QDialog::Rejected;
    } // if

    _iiPlugin->SetupUI(this);

	return QDialog::exec();
} // exec

WordsImportDialog::WordsImportDialog(const QString &pFile, const Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_qsFile = pFile;
	_vVocabulary = pVocabulary;
	_iiPlugin = pPlugin;

	_qdwiWordsImport.setupUi(this);
} // WordsImportDialog