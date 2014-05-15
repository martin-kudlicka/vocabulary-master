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

        virtual const void        close      ();
        virtual const QString     filter     () const;
        virtual const QStringList marks      () const;
		virtual const bool        open       (const QString &fileName);
        virtual const quint16     recordCount() const;
        virtual const QString     recordData (const quint16 &record, const QString &mark);
        virtual const void        setupUI    (QGroupBox *parent);
}; // ImpPlaintext

#endif // IMPPLAINTEXT_H
