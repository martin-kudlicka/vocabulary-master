#ifndef IMPPLAINTEXT_H
#define IMPPLAINTEXT_H

#include "../common/imp-interface.h"
#include <QtCore/QFile>

class ImpPlaintext : public QObject, private ImpInterface
{
    Q_OBJECT
    Q_INTERFACES(ImpInterface)

    private:
        QFile _qfPlaintext;

        virtual const QString GetFilter() const;
        virtual const QStringList GetMarks() const;
        virtual const int GetRecordCount() const;
        virtual const QString GetRecordData(const int &pRecord, const QString &pMark) const;
        virtual const bool Open(const QString &pFile);
        virtual const void SetupUI(QGroupBox *pParent);
}; // ImpPlaintext

#endif // IMPPLAINTEXT_H
