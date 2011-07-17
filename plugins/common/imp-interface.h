#ifndef IMPINTERFACE_H
#define IMPINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>

static const char *IMP_INTERFACE = "Imp Interface 1.0";
#ifndef _DEBUG
const QString DIR_PLUGINS = "plugins";
#endif

class ImpInterface
{
	public:
		virtual const QString GetFilter() const = 0;
		virtual const bool Open(const QString &pFile) const = 0;

	private:
		QString _qsFile;
}; // ImpInterface

Q_DECLARE_INTERFACE(ImpInterface, IMP_INTERFACE);

#endif // IMPINTERFACE_H