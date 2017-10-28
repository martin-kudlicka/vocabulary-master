#include "imp-anki.h"

#include <QtWidgets/QGroupBox>
#include "ankiimportwidget.h"
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

quintptr ImpAnki::recordCount() const
{
  const auto fieldId = _widget->fieldId(static_cast<int>(FieldNum::N1));
  auto query         = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FIELDMODELID + " = " + QString::number(fieldId));
  if (query.last())
  {
    return query.at() + 1;
  }
  else
  {
    return 0;
  }
}

QString ImpAnki::recordData(quintptr recordId, const QString &mark)
{
  // query records by first field ID to get always same sequence
  const auto fieldId = _widget->fieldId(static_cast<int>(FieldNum::N1));
  auto query         = _database.exec("SELECT " + COLUMN_FACTID + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FIELDMODELID + " = " + QString::number(fieldId));
  query.seek(recordId);
  const auto factId = query.value(static_cast<int>(ColumnPosition::N1)).toLongLong();

  // get mark ID
  const auto marks     = _widget->marks();
  const auto markIndex = marks.indexOf(mark);
  const auto markId    = _widget->fieldId(markIndex);

  // get data
  query = _database.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FACTID + " = " + QString::number(factId) + " AND " + COLUMN_FIELDMODELID + " = " + QString::number(markId));
  query.next();
  auto data = query.value(static_cast<int>(ColumnPosition::N1)).toString();
  if (data.endsWith(DATA_TAIL))
  {
    data.chop(QString(DATA_TAIL).size());
  }

  return data;
}

void ImpAnki::setupUI(QGroupBox *parent)
{
  _widget        = new AnkiImportWidget(&_database, parent);
  auto boxLayout = qobject_cast<QBoxLayout *>(parent->layout());
  boxLayout->insertWidget(WIDGET_POSITION, _widget);
}