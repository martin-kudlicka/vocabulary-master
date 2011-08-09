#ifndef ANKIIMPORTWIDGET_H
#define ANKIIMPORTWIDGET_H

#include <ui_ankiimportwidget.h>

#include "ankiimportwidget/decksmodel.h"
#include "ankiimportwidget/modelsmodel.h"

class AnkiImportWidget : public QWidget
{
    Q_OBJECT

    public:
        AnkiImportWidget(const QSqlDatabase *pAnki, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        DecksModel _dmDecksModel;
        ModelsModel _mmModelsModel;
        const QSqlDatabase *_qsdAnki;
        Ui::qwAnkiImport _qwaiAnkiImport;

        const void PrepareTreeView(QTreeView *pTreeView, QAbstractItemModel *pItemModel) const;

    private slots:
        const void on_qtvDecksSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
}; // AnkiImportWidget

#endif // ANKIIMPORTWIDGET_H