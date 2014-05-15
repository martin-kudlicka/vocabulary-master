#ifndef LICENSE_H
#define LICENSE_H

#include <QtCore/QDate>
#include <QtCore/QUuid>
#include "settings.h"

class License
{
    public:
		enum Status
		{
			StatusNone,
			StatusOk,
			StatusExpired,
			StatusInvalid
		}; // Status

        License(const Settings *settings);

		const QString &email()     const;
        const QString &firstName() const;
		const bool     isLoaded()  const;
		//const bool     isOk()      const;
		const QString &lastName()  const;
		const void     refreshLicense();
		const Status  &status()    const;
		const QUuid   &uid()      const;
		const QDate   &validTo()   const;

    private:
		      QDate     _validTo;
		      QString   _email;
		      QString   _firstName;
		      QString   _lastName;
		      QUuid     _identifier;
		const Settings *_settings;
		      Status    _status;
}; // License

#endif // LICENSE_H