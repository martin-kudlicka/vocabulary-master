#ifndef LICENSE_H
#define LICENSE_H

#include <QtCore/QDate>
#include <QtCore/QUuid>
#include "settings.h"

class License
{
    public:
        License(const Settings *pSettings);

		const QString &GetEmail() const;
        const QString &GetFirstName() const;
		const QString &GetLastName() const;
		const QUuid &GetUid() const;
		const QDate &GetValidTo() const;
		const void RefreshLicense();

    private:
		QDate _qdValidTo;
		const Settings *_sSettings;
		QString _qsEmail;
		QString _qsFirstName;
		QString _qsLastName;
		QUuid _quIdentifier;
}; // License

#endif // LICENSE_H