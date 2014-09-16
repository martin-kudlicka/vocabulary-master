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

        virtual void        close      ()                                      = 0;
		virtual QString     filter     () const                                = 0;
		virtual QStringList marks      () const                                = 0;
		virtual bool        open       (const QString &fileName)               = 0;
		virtual quint16     recordCount() const                                = 0;
		virtual QString     recordData (quint16 recordId, const QString &mark) = 0;
        virtual void        setupUI    (QGroupBox *parent)                     = 0;

	protected:
		virtual ~ImpInterface();
}; // ImpInterface

Q_DECLARE_INTERFACE(ImpInterface, IID_IMPINTERFACE);

#endif // IMPINTERFACE_H