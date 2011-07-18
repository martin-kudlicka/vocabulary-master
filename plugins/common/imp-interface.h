#ifndef IMPINTERFACE_H
#define IMPINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>

static const char *IMP_INTERFACE = "Imp Interface 1.0";

class ImpInterface
{
	public:
		virtual const QString GetFilter() const = 0;
		virtual const bool Open(const QString &pFile) = 0;
}; // ImpInterface

Q_DECLARE_INTERFACE(ImpInterface, IMP_INTERFACE);

#endif // IMPINTERFACE_H