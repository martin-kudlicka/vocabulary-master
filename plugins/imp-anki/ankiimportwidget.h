#ifndef ANKIIMPORTWIDGET_H
#define ANKIIMPORTWIDGET_H

#include <ui_ankiimportwidget.h>

#include "ankiimportwidget/decksmodel.h"
#include "ankiimportwidget/modelsmodel.h"
#include "ankiimportwidget/fieldsmodel.h"
#include "../../common/marklineeditpersistentdelegate.h"

class AnkiImportWidget : public QWidget
{
  Q_OBJECT

  public:
    AnkiImportWidget(const QSqlDatabase *database, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);

    qlonglong   fieldId(quintptr position) const;
    QStringList marks  ()                  const;

  private:
          DecksModel                     _decksModel;
          FieldsModel                    _fieldsModel;
          MarkLineEditPersistentDelegate _markEditDelegate;
          ModelsModel                    _modelsModel;
    const QSqlDatabase                  *_database;
          Ui::AnkiImportWidget           _ui;

    virtual ~AnkiImportWidget() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    void prepareTreeView(QTreeView *treeView, QAbstractItemModel *itemModel);

  private Q_SLOTS:
    void on_decks_selectionModel_selectionChanged (const QItemSelection &selected, const QItemSelection &deselected);
    void on_models_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif