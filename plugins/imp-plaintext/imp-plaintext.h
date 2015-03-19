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

		virtual ~ImpPlaintext() override;

        virtual void        close      ()                                    override;
        virtual QString     filter     () const                              override;
        virtual QStringList marks      () const                              override;
		virtual bool        open       (const QString &fileName)             override;
		virtual quint16     recordCount() const                              override;
        virtual QString     recordData (quint16 record, const QString &mark) override;
        virtual void        setupUI    (QGroupBox *parent)                   override;
}; // ImpPlaintext

#endif // IMPPLAINTEXT_H
