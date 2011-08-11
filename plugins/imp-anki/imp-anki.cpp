#include "imp-anki.h"

#include "ankiimportwidget.h"
#include <QtGui/QBoxLayout>

const QString ImpAnki::GetFilter() const
{
	return "Anki (*.anki)";
} // GetFilter

ImpAnki::ImpAnki() : ImpInterface()
{
    _qsdAnki = QSqlDatabase::addDatabase("QSQLITE", "Anki");
} // ImpAnki

const bool ImpAnki::Open(const QString &pFile)
{
    if (_qsdAnki.isOpen()) {
        _qsdAnki.close();
    } // if
    _qsdAnki.setDatabaseName(pFile);
    return _qsdAnki.open();
} // Open

const void ImpAnki::SetupUI(QGroupBox *pParent)
{
    AnkiImportWidget *aiwWidget = new AnkiImportWidget(&_qsdAnki, pParent);
    QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
    pLayout->insertWidget(WIDGET_POSITION, aiwWidget);
} // SetupUI

Q_EXPORT_PLUGIN2(imp-anki, ImpAnki)