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

        virtual const void Close();
        virtual const QString GetFilter() const;
        virtual const QStringList GetMarks() const;
        virtual const int GetRecordCount() const;
        virtual const QString GetRecordData(const int &pRecord, const QString &pMark);
        virtual const bool Open(const QString &pFile);
        virtual const void SetupUI(QGroupBox *pParent);
}; // ImpPlaintext

#endif // IMPPLAINTEXT_H
