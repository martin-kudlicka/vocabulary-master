#include "ankiimportwidget.h"

AnkiImportWidget::AnkiImportWidget(QSqlDatabase *pAnki, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags)
{
    _qsdAnki = pAnki;

    _qwaiAnkiImport.setupUi(this);
} // AnkiImportWidget