#ifndef IMPANKI_H
#define IMPANKI_H

#include "../common/imp-interface.h"
#include <QtSql/QSqlDatabase>

class ImpAnki : public QObject, private ImpInterface
{
	Q_OBJECT
	Q_INTERFACES(ImpInterface)

	private:
        QSqlDatabase _qsdFile;

		virtual const QString GetFilter() const;
		virtual const bool Open(const QString &pFile);
}; // ImpAnki

#endif // IMPANKI_H
