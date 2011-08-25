#ifndef LICENSE_H
#define LICENSE_H

#include <QtCore/QByteArray>
#include <QtCore/QDate>
#include <QtCore/QUuid>

class License
{
    public:
        License(const QByteArray &pLicenseData);

		const QDate &GetValidTo() const;
        const QString &GetFirstName() const;
		const QString &GetLastName() const;
		const QUuid &GetUid() const;

    private:
		QDate _qdValidTo;
		QString _qsFirstName;
		QString _qsLastName;
		QUuid _quIdentifier;
}; // License

#endif // LICENSE_H