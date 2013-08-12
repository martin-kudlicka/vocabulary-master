#include "imp-anki.h"

#include <QtWidgets/QBoxLayout>
#include <QtSql/QSqlQuery>

const QChar DATA_TAIL = ',';
const QString COLUMN_FACTID = "factId";
const QString COLUMN_FIELDMODELID = "fieldModelId";
const QString COLUMN_ID = "id";
const QString COLUMN_VALUE = "value";
const QString TABLE_FIELDS = "fields";

const void ImpAnki::Close()
{
    if (_qsdAnki.isOpen()) {
        _qsdAnki.close();
    } // if
} // Close

const QString ImpAnki::GetFilter() const
{
	return "Anki (*.anki)";
} // GetFilter

const QStringList ImpAnki::GetMarks() const
{
	return _aiwWidget->GetMarks();
} // GetMarks

const int ImpAnki::GetRecordCount() const
{
	qlonglong qllFieldId = _aiwWidget->GetFieldId(FieldNum1);
	QSqlQuery qsqQuery = _qsdAnki.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FIELDMODELID + " = " + QString::number(qllFieldId));
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

const QString ImpAnki::GetRecordData(const int &pRecord, const QString &pMark)
{
	// query records by first field ID to get always same sequence
	qlonglong qllFieldId = _aiwWidget->GetFieldId(FieldNum1);
	QSqlQuery qsqQuery = _qsdAnki.exec("SELECT " + COLUMN_FACTID + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FIELDMODELID + " = " + QString::number(qllFieldId));
	qsqQuery.seek(pRecord);
	qlonglong qllFactId = qsqQuery.value(ColumnPosition1).toLongLong();

	// get mark ID
	QStringList qslMarks = _aiwWidget->GetMarks();
	int iIndex = qslMarks.indexOf(pMark);
	qlonglong qllMarkId = _aiwWidget->GetFieldId(iIndex);

	// get data
	qsqQuery = _qsdAnki.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_FACTID + " = " + QString::number(qllFactId) + " AND " + COLUMN_FIELDMODELID + " = " + QString::number(qllMarkId));
	qsqQuery.next();
    QString qsData = qsqQuery.value(ColumnPosition1).toString();
    if (qsData.endsWith(DATA_TAIL)) {
        qsData.chop(QString(DATA_TAIL).size());
    } // if

	return qsData;
} // GetRecordData

ImpAnki::ImpAnki() : ImpInterface()
{
    _qsdAnki = QSqlDatabase::addDatabase("QSQLITE", "Anki");
} // ImpAnki

const bool ImpAnki::Open(const QString &pFile)
{
    Close();
    _qsdAnki.setDatabaseName(pFile);
    return _qsdAnki.open();
} // Open

const void ImpAnki::SetupUI(QGroupBox *pParent)
{
    _aiwWidget = new AnkiImportWidget(&_qsdAnki, pParent);
    QBoxLayout *pLayout = qobject_cast<QBoxLayout *>(pParent->layout());
    pLayout->insertWidget(WIDGET_POSITION, _aiwWidget);
} // SetupUI