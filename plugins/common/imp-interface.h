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
        static const quint8 WIDGET_POSITION = 0;

        virtual const void        close      ()                                             = 0;
		virtual const QString     filter     () const                                       = 0;
		virtual const QStringList marks      () const                                       = 0;
		virtual const quint16     recordCount() const                                       = 0;
		virtual const QString     recordData (const quint16 &recordId, const QString &mark) = 0;
		virtual const bool        open       (const QString &fileName)                      = 0;
        virtual const void        setupUI    (QGroupBox *parent)                            = 0;

	protected:
		virtual ~ImpInterface();
}; // ImpInterface

Q_DECLARE_INTERFACE(ImpInterface, IID_IMPINTERFACE);

#endif // IMPINTERFACE_H