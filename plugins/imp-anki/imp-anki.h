#ifndef IMPANKI_H
#define IMPANKI_H

#include "../common/imp-interface.h"
#include "ankiimportwidget.h"

class ImpAnki : public QObject, private ImpInterface
{
	Q_OBJECT
	Q_INTERFACES(ImpInterface)

    public:
        ImpAnki();

	private:
		enum eColumnPosition {
			ColumnPosition1
		}; // eColumnPosition
		enum eFieldNum {
			FieldNum1
		}; // eFieldNum

		AnkiImportWidget *_aiwWidget;
        QSqlDatabase _qsdAnki;

		virtual const QString GetFilter() const;
		virtual const QStringList GetMarks() const;
		virtual const int GetRecordCount();
		virtual const QString GetRecordData(const int &pRecord, const QString &pMark);
		virtual const bool Open(const QString &pFile);
        virtual const void SetupUI(QGroupBox *pParent);
}; // ImpAnki

#endif // IMPANKI_H
