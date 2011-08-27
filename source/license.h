#ifndef LICENSE_H
#define LICENSE_H

#include <QtCore/QDate>
#include <QtCore/QUuid>
#include "settings.h"

class License
{
	Q_ENUMS(eStatus)

    public:
		enum eStatus {
			StatusNone,
			StatusOk,
			StatusExpired,
			StatusInvalid
		}; // eStatus
        License(const Settings *pSettings);

		const QString &GetEmail() const;
        const QString &GetFirstName() const;
		const QString &GetLastName() const;
		const eStatus &GetStatus() const;
		const QUuid &GetUid() const;
		const QDate &GetValidTo() const;
		const void RefreshLicense();

    private:
		eStatus _esStatus;
		QDate _qdValidTo;
		QString _qsEmail;
		QString _qsFirstName;
		QString _qsLastName;
		QUuid _quIdentifier;
		const Settings *_sSettings;
}; // License

#endif // LICENSE_H