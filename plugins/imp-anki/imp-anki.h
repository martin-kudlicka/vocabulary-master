#ifndef IMPANKI_H
#define IMPANKI_H

#include "../common/imp-interface.h"

class ImpAnki : public QObject, private ImpInterface
{
	Q_OBJECT
	Q_INTERFACES(ImpInterface)

	private:
		const QString GetFilter() const;
		const bool Open(const QString &pFile) const;
}; // ImpAnki

#endif // IMPANKI_H
