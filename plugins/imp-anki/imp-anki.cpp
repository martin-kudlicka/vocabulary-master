#include "imp-anki.h"

#include <QtWidgets/QBoxLayout>
#include <QtSql/QSqlQuery>

const QChar   DATA_TAIL           = ',';
const QString COLUMN_FACTID       = "factId";
const QString COLUMN_FIELDMODELID = "fieldModelId";
const QString COLUMN_ID           = "id";
const QString COLUMN_VALUE        = "value";
const QString TABLE_FIELDS        = "fields";

ImpAnki::ImpAnki() : ImpInterface(), _database(QSqlDatabase::addDatabase("QSQLITE", "Anki"))
{
}

ImpAnki::~ImpAnki()
{
}

void ImpAnki::close()
{
    if (_database.isOpen())
	{
        _database.close();
    }
}

QString ImpAnki::filter() const
{
	return "Anki (*.anki)";
}

QStringList ImpAnki::marks() const
{
	return _widget->marks();
}

bool ImpAnki::open(const QString &fileName)
{
	close();
	_database.setDatabaseName(fileName);
	return _database.open();
}

quint16 ImpAnki::recordCount() const
{
	const qlonglong fieldId = _widget->fieldId(FieldNum1);
	QSqlQuery query         = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FIELDMODELID + " = " + QString::number(fieldId));
	if (query.last())
	{
		return query.at() + 1;
	}
	else
	{
		return 0;
	}
}

QString ImpAnki::recordData(quint16 recordId, const QString &mark)
{
	// query records by first field ID to get always same sequence
	const qlonglong fieldId = _widget->fieldId(FieldNum1);
	QSqlQuery query         = _database.exec("SELECT " + COLUMN_FACTID + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FIELDMODELID + " = " + QString::number(fieldId));
	query.seek(recordId);
	const qlonglong factId = query.value(ColumnPosition1).toLongLong();

	// get mark ID
	const QStringList marks = _widget->marks();
	const quint8 markIndex  = marks.indexOf(mark);
	const qlonglong markId  = _widget->fieldId(markIndex);

	// get data
	query = _database.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FACTID + " = " + QString::number(factId) + " AND " + COLUMN_FIELDMODELID + " = " + QString::number(markId));
	query.next();
    QString data = query.value(ColumnPosition1).toString();
    if (data.endsWith(DATA_TAIL))
	{
        data.chop(QString(DATA_TAIL).size());
    }

	return data;
}

void ImpAnki::setupUI(QGroupBox *parent)
{
    _widget = new AnkiImportWidget(&_database, parent);
    QBoxLayout *boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
    boxLayout->insertWidget(WIDGET_POSITION, _widget);
}