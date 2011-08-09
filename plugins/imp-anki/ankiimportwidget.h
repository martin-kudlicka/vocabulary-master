#ifndef ANKIIMPORTWIDGET_H
#define ANKIIMPORTWIDGET_H

#include <ui_ankiimportwidget.h>

#include <QtSql/QSqlDatabase>

class AnkiImportWidget : public QWidget
{
    public:
        AnkiImportWidget(QSqlDatabase *pAnki, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        QSqlDatabase *_qsdAnki;
        Ui::qwAnkiImport _qwaiAnkiImport;
}; // AnkiImportWidget

#endif // ANKIIMPORTWIDGET_H