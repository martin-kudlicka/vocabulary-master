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

		virtual ~ImpAnki() override;

        virtual void        close      ()                                      override;
		virtual QString     filter     () const                                override;
		virtual QStringList marks      () const                                override;
		virtual bool        open       (const QString &fileName)               override;
		virtual quint16     recordCount() const                                override;
		virtual QString     recordData (quint16 recordId, const QString &mark) override;
        virtual void        setupUI    (QGroupBox *parent)                     override;
}; // ImpAnki

#endif // IMPANKI_H
