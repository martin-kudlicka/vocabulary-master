#ifndef ANKIIMPORTWIDGET_H
#define ANKIIMPORTWIDGET_H

#include <ui_ankiimportwidget.h>

#include "ankiimportwidget/decksmodel.h"

class AnkiImportWidget : public QWidget
{
    public:
        AnkiImportWidget(const QSqlDatabase *pAnki, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        DecksModel _dmDecksModel;
        const QSqlDatabase *_qsdAnki;
        Ui::qwAnkiImport _qwaiAnkiImport;

        const void PrepareDecks();
}; // AnkiImportWidget

#endif // ANKIIMPORTWIDGET_H