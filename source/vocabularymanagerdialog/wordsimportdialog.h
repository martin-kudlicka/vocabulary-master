#ifndef WORDSIMPORTDIALOG_H
#define WORDSIMPORTDIALOG_H

#include <ui_wordsimportdialog.h>

#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimportfieldsmodel.h"
#include "vocabularymanagerdialog/wordsimportdialog/wordsimporteditordelegate.h"

class ImpInterface;

class WordsImportDialog : public QDialog
{
  Q_OBJECT

  public:
             WordsImportDialog(const QString &file, Vocabulary *vocabulary, ImpInterface *plugin, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~WordsImportDialog() Q_DECL_OVERRIDE;

    int exec() Q_DECL_OVERRIDE;

  private:
    enum class Target
    {
      Preview,
      Vocabulary
    };

    bool                      _importing;
    bool                      _interrupt;
    CategoriesModel           _categoriesModel;
    ImpInterface             *_plugin;
    QString                   _file;
    WordsImportEditorDelegate _editorDelegate;
    Ui::WordsImportDialog     _ui;
    Vocabulary               *_vocabulary;
    WordsImportFieldsModel    _fieldsModel;

    virtual void accept               () Q_DECL_OVERRIDE;
            void createFieldEditors   ();
            void enableControls       () const;
            void importData           (const Target &target);
            void preparePreviewColumns() const;
    virtual void reject               () Q_DECL_OVERRIDE;

  private Q_SLOTS:
    void on_previewRefresh_clicked                   (bool checked = false);
    void on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
};

#endif