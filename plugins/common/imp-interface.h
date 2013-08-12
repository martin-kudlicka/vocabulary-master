#ifndef IMPINTERFACE_H
#define IMPINTERFACE_H

#include <QtCore/QString>
#include <QtPlugin>
#include <QtWidgets/QGroupBox>
#include "translationhandler.h"

#define IID_IMPINTERFACE "cz.martinkudlicka.VocabularyMaster.ImpInterface"

class ImpInterface : public TranslationHandler
{
	public:
        static const int WIDGET_POSITION = 0;

        virtual const void Close() = 0;
		virtual const QString GetFilter() const = 0;
		virtual const QStringList GetMarks() const = 0;
		virtual const int GetRecordCount() const = 0;
		virtual const QString GetRecordData(const int &pRecord, const QString &pMark) = 0;
		virtual const bool Open(const QString &pFile) = 0;
        virtual const void SetupUI(QGroupBox *pParent) = 0;
}; // ImpInterface

Q_DECLARE_INTERFACE(ImpInterface, IID_IMPINTERFACE);

#endif // IMPINTERFACE_H