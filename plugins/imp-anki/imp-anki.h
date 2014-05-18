#ifndef IMPANKI_H
#define IMPANKI_H

#include "../common/imp-interface.h"
#include "ankiimportwidget.h"

class ImpAnki : public QObject, private ImpInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID IID_IMPINTERFACE FILE "imp-anki.json")
	Q_INTERFACES(ImpInterface)

    public:
        ImpAnki();

	private:
		enum ColumnPosition
		{
			ColumnPosition1
		}; // ColumnPosition
		enum FieldNum
		{
			FieldNum1
		}; // FieldNum

		AnkiImportWidget *_widget;
        QSqlDatabase      _database;

		virtual ~ImpAnki();

        virtual const void        close      ();
		virtual const QString     filter     () const;
		virtual const QStringList marks      () const;
		virtual const bool        open       (const QString &fileName);
		virtual const quint16     recordCount() const;
		virtual const QString     recordData (const quint16 &recordId, const QString &mark);
        virtual const void        setupUI    (QGroupBox *parent);
}; // ImpAnki

#endif // IMPANKI_H
