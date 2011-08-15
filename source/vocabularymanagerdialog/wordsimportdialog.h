#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "../plugins/common/imp-interface.h"
#include "vocabularymanagerdialog/wordsimportdialog/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"

class WordsImportDialog : public QDialog
{
	Q_OBJECT

	public:
        ~WordsImportDialog();
		WordsImportDialog(const QString &pFile, const Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		int exec();

	private:
		enum eTarget {
			TargetPreview,
			TargetVocabulary
		}; // eTarget

        CategoriesModel _cmCategoriesModel;
		ImpInterface *_iiPlugin;
		QString _qsFile;
		Ui::qdWordsImport _qdwiWordsImport;
		const Vocabulary *_vVocabulary;
        WordsImportFieldsModel _wifmFieldsModel;

        virtual void accept();
        const void CreateFieldEditors() const;
        const void EnableControls(const bool &pImport) const;
		const void ImportData(const eTarget &pTarget) const;
		const void PreparePreviewColumns() const;

	private slots:
		const void on_qpbPreviewRefresh_clicked(bool checked = false) const;
        const void on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // WordsImportDialog

#endif // WORDSIMPORTDIALOG_H