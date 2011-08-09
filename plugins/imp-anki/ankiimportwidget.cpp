#include "ankiimportwidget.h"

AnkiImportWidget::AnkiImportWidget(const QSqlDatabase *pAnki, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags), _dmDecksModel(pAnki)
{
    _qsdAnki = pAnki;

    _qwaiAnkiImport.setupUi(this);

    PrepareDecks();
} // AnkiImportWidget

const void AnkiImportWidget::PrepareDecks()
{
    _qwaiAnkiImport.qtvDecks->setModel(&_dmDecksModel);
    for (int iColumn = 0; iColumn < _qwaiAnkiImport.qtvDecks->header()->count(); iColumn++) {
        _qwaiAnkiImport.qtvDecks->header()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for
} // PrepareDecks