#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "../plugins/common/imp-interface.h"
#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"

class WordsImportDialog : public QDialog
{
	Q_OBJECT

	public:
        ~WordsImportDialog();
		WordsImportDialog(const QString &pFile, Vocabulary *pVocabulary, ImpInterface *pPlugin, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

		int exec();

	private:
        static const int IMPORT_REFRESHINTERVAL = 50;

		enum eTarget {
			TargetPreview,
			TargetVocabulary
		}; // eTarget

        bool _bImporting;
        bool _bInterrupt;
        CategoriesModel _cmCategoriesModel;
		ImpInterface *_iiPlugin;
		QString _qsFile;
		Ui::qdWordsImport _qdwiWordsImport;
		Vocabulary *_vVocabulary;
        WordsImportFieldsModel _wifmFieldsModel;

        virtual void accept();
        const void CreateFieldEditors() const;
        const void EnableControls() const;
		const void ImportData(const eTarget &pTarget);
		const void PreparePreviewColumns() const;
        virtual void reject();

	private slots:
		const void on_qpbPreviewRefresh_clicked(bool checked = false);
        const void on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // WordsImportDialog

#endif // WORDSIMPORTDIALOG_H