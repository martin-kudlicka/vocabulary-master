#ifndef WORDSEXPORTDIALOG_H
#define WORDSEXPORTDIALOG_H

#include <ui_wordsexportdialog.h>

#include "common/categoriesmodel.h"
#include "vocabularymanagerdialog/wordsexportdialog/wordsexportfieldsmodel.h"
#include "../../plugins/common/exp-interface.h"
#include "vocabularymanagerdialog/wordsexportdialog/exppluginsmodel.h"
#include "../../common/marklineeditpersistentdelegate.h"

class WordsExportDialog : public QDialog
{
  Q_OBJECT

  public:
             WordsExportDialog(const QSharedPointer<Vocabulary> &vocabulary, const Plugins::ExpPluginList &expPlugins, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~WordsExportDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
          CategoriesModel                _categoriesModel;
          ExpPluginsModel                _expPluginsModel;
          MarkLineEditPersistentDelegate _markDelegate;
          Plugins::ExpPluginList         _expPlugins;
          Ui::WordsExportDialog          _ui;
          QHash<quintptr, quintptr>      _expPluginPage;
    const QSharedPointer<Vocabulary>     _vocabulary;
          WordsExportFieldsModel         _fieldsModel;

  private Q_SLOTS:
    void on_exportButton_clicked                     (bool checked = false);
    void on_expPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_plugin_progressExportSetMax              (quintptr max)                                               const;
    void on_plugin_progressExportSetValue            (quintptr value)                                             const;
    void on_plugin_vocabularyGetCategoryIds          (ExpInterface::CategoryIdList *categoryIds)                  const;
    void on_plugin_vocabularyGetCategoryName         (quintptr categoryId, QString *name)                         const;
    void on_plugin_vocabularyGetMarks                (QStringList *marks)                                         const;
    void on_plugin_vocabularyGetMarkText             (quintptr recordId, const QString &mark, QString *text)      const;
    void on_plugin_vocabularyGetRecordCount          (quintptr categoryId, quintptr *count)                       const;
    void on_plugin_vocabularyGetRecordIds            (quintptr categoryId, ExpInterface::RecordIdList *recordIds) const;
};

#endif