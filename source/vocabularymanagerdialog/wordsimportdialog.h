#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "../plugins/common/imp-interface.h"
#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimporteditordelegate.h"

class WordsImportDialog : public QDialog
{
	Q_OBJECT

	public:
		         WordsImportDialog(const QString &file, Vocabulary *vocabulary, ImpInterface *plugin, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~WordsImportDialog();

		int exec();

	private:
        static const quint8 IMPORT_REFRESHINTERVAL = 20;

		enum Target
		{
			TargetPreview,
			TargetVocabulary
		}; // Target

        bool                      _importing;
        bool                      _interrupt;
        CategoriesModel           _categoriesModel;
		ImpInterface             *_plugin;
		QString                   _file;
		WordsImportEditorDelegate _editorDelegate;
		Ui::WordsImportDialog     _ui;
		Vocabulary               *_vocabulary;
        WordsImportFieldsModel    _fieldsModel;

        virtual void accept               ();
                void createFieldEditors   ();
                void enableControls       () const;
		        void importData           (const Target &target);
		        void preparePreviewColumns() const;
        virtual void reject               ();

	private slots:
		void on_previewRefresh_clicked                   (bool checked = false);
        void on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // WordsImportDialog

#endif // WORDSIMPORTDIALOG_H