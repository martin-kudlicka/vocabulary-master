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
        AnkiImportWidget(const QSqlDatabase *database, QWidget *parent = NULL, Qt::WindowFlags flags = 0);

		const qlonglong   fieldId(const quint8 &position) const;
		const QStringList marks()                         const;

    private:
              DecksModel                     _decksModel;
              FieldsModel                    _fieldsModel;
              MarkLineEditPersistentDelegate _markEditDelegate;
              ModelsModel                    _modelsModel;
        const QSqlDatabase                  *_database;
              Ui::AnkiImportWidget           _ui;

		virtual ~AnkiImportWidget();

        const void prepareTreeView(QTreeView *treeView, QAbstractItemModel *itemModel);

    private slots:
        const void on_decks_selectionModel_selectionChanged (const QItemSelection &selected, const QItemSelection &deselected);
        const void on_models_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // AnkiImportWidget

#endif // ANKIIMPORTWIDGET_H