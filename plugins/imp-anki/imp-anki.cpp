#include "imp-anki.h"

#include "ankiimportwidget.h"
#include <QtGui/QVBoxLayout>

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

const void ImpAnki::SetupUI(QDialog *pParent)
{
    AnkiImportWidget *aiwWidget = new AnkiImportWidget(&_qsdAnki, pParent);
    QVBoxLayout *pLayout = qobject_cast<QVBoxLayout *>(pParent->layout());
    pLayout->insertWidget(WIDGET_POSITION, aiwWidget);
} // SetupUI

Q_EXPORT_PLUGIN2(imp-anki, ImpAnki)