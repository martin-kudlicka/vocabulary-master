#ifndef IMPPLAINTEXT_H
#define IMPPLAINTEXT_H

#include "../common/imp-interface.h"
#include "plaintextimportwidget.h"

class ImpPlaintext : public QObject, private ImpInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_IMPINTERFACE FILE "imp-plaintext.json")
    Q_INTERFACES(ImpInterface)

    private:
		static const qint8 CACHED_NONE = -2;

		qint16                 _cachedRecord;
		PlaintextFile          _plaintextFile;
		PlaintextImportWidget *_widget;
		QStringList            _cachedCapture;

		virtual ~ImpPlaintext();

        virtual void        close      ();
        virtual QString     filter     () const;
        virtual QStringList marks      () const;
		virtual bool        open       (const QString &fileName);
        virtual quint16     recordCount() const;
        virtual QString     recordData (quint16 record, const QString &mark);
        virtual void        setupUI    (QGroupBox *parent);
}; // ImpPlaintext

#endif // IMPPLAINTEXT_H
