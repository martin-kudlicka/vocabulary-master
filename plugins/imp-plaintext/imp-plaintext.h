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
		static const int CACHED_NONE = -2;

		int _iCachedRecord;
		PlaintextFile _pfPlaintext;
		PlaintextImportWidget *_piwWidget;
		QStringList _qslCachedCapture;

        virtual const void close();
        virtual const QString filter() const;
        virtual const QStringList marks() const;
        virtual const quint16 recordCount() const;
        virtual const QString recordData(const quint16 &pRecord, const QString &pMark);
        virtual const bool open(const QString &pFile);
        virtual const void setupUI(QGroupBox *pParent);
}; // ImpPlaintext

#endif // IMPPLAINTEXT_H
