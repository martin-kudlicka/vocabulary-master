#ifndef IMPINTERFACE_H
#define IMPINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>
#include <QtGui/QGroupBox>

static const char *IMP_INTERFACE = "Imp Interface 1.0";
const QString TEMPLATE_MARK = "${%1}";

class ImpInterface
{
	public:
        static const int WIDGET_POSITION = 0;

		virtual const QString GetFilter() const = 0;
		virtual const QStringList GetMarks() const = 0;
		virtual const int GetRecordCount() const = 0;
		virtual const QString GetRecordData(const int &pRecord, const QString &pMark) = 0;
		virtual const bool Open(const QString &pFile) = 0;
        virtual const void SetupUI(QGroupBox *pParent) = 0;
}; // ImpInterface

Q_DECLARE_INTERFACE(ImpInterface, IMP_INTERFACE);

#endif // IMPINTERFACE_H