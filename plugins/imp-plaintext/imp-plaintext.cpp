#include "imp-plaintext.h"

#include "plaintextimportwidget.h"
#include <QtGui/QBoxLayout>

const QString ImpPlaintext::GetFilter() const
{
    return tr("plaintext (*.txt)");
} // GetFilter

const QStringList ImpPlaintext::GetMarks() const
{
    // TODO
    return QStringList();
} // GetMarks

const int ImpPlaintext::GetRecordCount() const
{
    // TODO
    return 0;
} // GetRecordCount

const QString ImpPlaintext::GetRecordData(const int &pRecord, const QString &pMark) const
{
    // TODO
    return QString();
} // GetRecordData

const bool ImpPlaintext::Open(const QString &pFile)
{
    if (_qfPlaintext.isOpen()) {
        _qfPlaintext.close();
    } // if
    _qfPlaintext.setFileName(pFile);
    return _qfPlaintext.open(QIODevice::ReadOnly | QIODevice::Text);
} // Open

const void ImpPlaintext::SetupUI(QGroupBox *pParent)
{
    PlaintextImportWidget *piwWidget = new PlaintextImportWidget(pParent);
    QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
    pLayout->insertWidget(WIDGET_POSITION, piwWidget);
} // SetupUI

Q_EXPORT_PLUGIN2(imp-plaintext, ImpPlaintext)