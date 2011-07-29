#include "vocabulary.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

const QString COLUMN_ATTRIBUTES = "attributes";
const QString COLUMN_CATEGORYID = "category_id";
const QString COLUMN_ENABLED = "enabled";
const QString COLUMN_FIELDID = "field_id";
const QString COLUMN_ID = "id";
const QString COLUMN_KEY = "key";
const QString COLUMN_LANGUAGE = "language";
const QString COLUMN_NAME = "name";
//const QString COLUMN_PRIORITY = "priority";
const QString COLUMN_RECORDID = "record_id";
const QString COLUMN_TEMPLATENAME = "template_name";
const QString COLUMN_TEXT = "text";
//const QString COLUMN_TYPE = "type";
const QString COLUMN_VALUE = "value";
#ifdef FREE
const QString FIELD_NOTE = QT_TRANSLATE_NOOP("Vocabulary", "Note");
const QString FIELD_WORD = QT_TRANSLATE_NOOP("Vocabulary", "Word");
const QString LEARNING_TEMPLATE = "<center style=\"font-size:20px\">" + VARIABLE_MARK + FIELD_WORD + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + FIELD_NOTE + "</center>";
#endif
const QString TABLE_CATEGORIES = "categories";
const QString TABLE_DATA = "data";
const QString TABLE_FIELDS = "fields";
const QString TABLE_RECORDS = "records";
const QString TABLE_SETTINGS = "settings";

Vocabulary::~Vocabulary()
{
	CloseDatabase();
} // ~Vocabulary

const int Vocabulary::AddCategory(const QString &pName) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_ENABLED + ") VALUES ('" + pName + "', '" + QString::number(true) + "')");
    return qsqQuery.lastInsertId().toInt();
} // AddCategory

const void Vocabulary::AddRecord(const int &pCategoryId) const
{
	// create new record
	QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(pCategoryId) + "')");
	int iRecord = qsqQuery.lastInsertId().toInt();

	// create new empty data
	QList<int> qlDataList;
	foreach (int iFieldId, GetFieldIds()) {
		qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(iFieldId) + "', '" + QString::number(iRecord) + "', '')");
		qlDataList.append(qsqQuery.lastInsertId().toInt());
	} // foreach
} // AddRecord

const void Vocabulary::BeginEdit()
{
    _qsdDatabase.transaction();
} // BeginEdit

const void Vocabulary::CloseDatabase()
{
	if (_qsdDatabase.isOpen()) {
		_qsdDatabase.close();
	} // if
} // CloseDatabase

const void Vocabulary::EndEdit(const bool &pSave /* true */)
{
    if (pSave) {
        _qsdDatabase.commit();
    } else {
        _qsdDatabase.rollback();
    } // if else
} // EndEdit

#ifndef FREE
const bool Vocabulary::GetCategoryEnabled(const int &pCategoryId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    qsqQuery.next();
    return qsqQuery.value(0).toBool();
} // GetCategoryEnabled
#endif

const Vocabulary::tCategoryIdList Vocabulary::GetCategoryIds() const
{
    tCategoryIdList tcilCategories;

    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    while (qsqQuery.next()) {
        tcilCategories.append(qsqQuery.value(0).toInt());
    } // while

    return tcilCategories;
} // GetCategoryIds

const QString Vocabulary::GetCategoryName(const int &pCategoryId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    qsqQuery.next();
    return qsqQuery.value(0).toString();
} // GetCategoryName

const QString Vocabulary::GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecordId = qsqQuery.value(0).toInt();

	return GetDataText(iRecordId, pFieldId);
} // GetDataText

const QString Vocabulary::GetDataText(const int &pRecordId, const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TEXT + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
	qsqQuery.next();
	return qsqQuery.value(0).toString();
} // GetDataText

const int Vocabulary::GetFieldCount() const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetFieldCount

const int Vocabulary::GetFieldId(const int &pPosition) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	int iPosition = 0;
	while (qsqQuery.next()) {
		if (iPosition == pPosition) {
			return qsqQuery.value(ColumnPosition1).toInt();
		} else {
			iPosition++;
		} // if else
	} // while

	return NOT_FOUND;
} // GetFieldId

const Vocabulary::tFieldIdList Vocabulary::GetFieldIds() const
{
	tFieldIdList tfilIds;

	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	while (qsqQuery.next()) {
		tfilIds.append(qsqQuery.value(ColumnPosition1).toInt());
	} // while

	return tfilIds;
} // GetFieldIds

const Vocabulary::eFieldLanguage Vocabulary::GetFieldLanguage(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_LANGUAGE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<eFieldLanguage>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldLanguageUknown;
} // GetFieldLanguage

const QString Vocabulary::GetFieldName(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // GetFieldName

const QString Vocabulary::GetFieldTemplateName(const int &pFieldId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TEMPLATENAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // GetFieldTemplateName

/*const Vocabulary::eFieldType Vocabulary::GetFieldType(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TYPE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<eFieldType>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldTypeUnknown;
} // GetFieldType*/

#ifndef FREE
const QString Vocabulary::GetLanguageName(const eFieldLanguage &pLanguage) const
{
    switch (pLanguage) {
        case FieldLanguageLeft:
            return GetSettings(KEY_LANGUAGE1);
        case FieldLanguageRight:
            return GetSettings(KEY_LANGUAGE2);
        default:
            return QString();
    } // switch
} // GetLanguageName
#endif

const QString Vocabulary::GetName() const
{
    return QFileInfo(_qsVocabularyFile).completeBaseName();
} // GetName

const int Vocabulary::GetRecordCategory(const int &pRecordId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
	qsqQuery.next();
	return qsqQuery.value(0).toInt();
} // GetRecordCategory

const int Vocabulary::GetRecordCount() const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

const int Vocabulary::GetRecordCount(const int &pCategoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

const int Vocabulary::GetRecordCount(const bool &pEnabled) const
{
    QSqlQuery qsqQuery("SELECT " + TABLE_RECORDS + '.' + COLUMN_ID + " FROM " + TABLE_RECORDS + " JOIN " + TABLE_CATEGORIES + " ON " + TABLE_RECORDS + '.' + COLUMN_CATEGORYID + " = " + TABLE_CATEGORIES + '.' + COLUMN_ID + " WHERE " + TABLE_CATEGORIES + '.' + COLUMN_ENABLED + " = " + QString::number(pEnabled));
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // GetRecordCount

const int Vocabulary::GetRecordId(const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	qsqQuery.seek(pRow);
	return qsqQuery.value(0).toInt();
} // GetRecordId

const int Vocabulary::GetRecordId(const int &pCategoryId, const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	return qsqQuery.value(0).toInt();
} // GetRecordId

const int Vocabulary::GetRow(const int &pRecordId, const int &pCategoryId) const
{
	int iRow = 0;

	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	while (qsqQuery.next()) {
		if (qsqQuery.value(0).toInt() == pRecordId) {
			return iRow;
		} else {
			iRow++;
		} // if else
	} // while

	return NOT_FOUND;
} // GetRow

const QString Vocabulary::GetSettings(const QString &pKey) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + pKey + "'");
	if (qsqQuery.next()) {
		return qsqQuery.value(0).toString();
	} else {
		return QString();
	} // if else
} // GetSettings

const QString &Vocabulary::GetVocabularyFile() const
{
    return _qsVocabularyFile;
} // GetVocabularyFile

const void Vocabulary::Initialize() const
{
    _qsdDatabase.exec("CREATE TABLE " + TABLE_SETTINGS + " ("
					  + COLUMN_KEY + " TEXT NOT NULL,"
					  + COLUMN_VALUE + " TEXT NOT NULL)");
    _qsdDatabase.exec("CREATE TABLE " + TABLE_CATEGORIES + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_NAME + " TEXT NOT NULL,"
					  //+ COLUMN_PRIORITY + " INTEGER,"
					  + COLUMN_ENABLED + " INTEGER NOT NULL)");
	_qsdDatabase.exec("CREATE TABLE " + TABLE_FIELDS + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_TEMPLATENAME + " TEXT NOT NULL,"
					  //+ COLUMN_TYPE + " INTEGER NOT NULL,"
					  + COLUMN_NAME + " TEXT NOT NULL,"
					  + COLUMN_ATTRIBUTES + " INTEGER NOT NULL,"
					  + COLUMN_LANGUAGE + " INTEGER NOT NULL)");
	_qsdDatabase.exec("CREATE TABLE " + TABLE_RECORDS + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_CATEGORYID + " INTEGER REFERENCES " + TABLE_CATEGORIES + " ON DELETE CASCADE)");
    _qsdDatabase.exec("CREATE TABLE " + TABLE_DATA + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_FIELDID + " INTEGER REFERENCES " + TABLE_FIELDS + " ON DELETE CASCADE,"
					  + COLUMN_RECORDID + " INTEGER REFERENCES " + TABLE_RECORDS + " ON DELETE CASCADE,"
					  + COLUMN_TEXT + " TEXT NOT NULL)"
					  /*+ COLUMN_PRIORITY + " INTEGER,"
					  + COLUMN_ENABLED + " INTEGER)"*/);

#ifdef FREE
	// fill default data
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_WORD + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_WORD + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageLeft) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_NOTE + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_NOTE + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageLeft) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_WORD + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_WORD + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageRight) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_NOTE + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_NOTE + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageRight) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " VALUES ('" + KEY_LEARNINGTEMPLATE1 + "', '" + LEARNING_TEMPLATE + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " VALUES ('" + KEY_LEARNINGTEMPLATE2 + "', '" + LEARNING_TEMPLATE + "')");
#endif
} // Initialize

const bool Vocabulary::IsOpen() const
{
    return _qsdDatabase.isOpen();
} // IsOpen

const void Vocabulary::New(const QString &pFilePath)
{
	_qsVocabularyFile = pFilePath;

	CloseDatabase();

	if (QFile::exists(pFilePath)) {
		QFile::remove(pFilePath);
	} // if

	OpenDatabase();
	Initialize();
} // New

const void Vocabulary::Open(const QString &pFilePath)
{
    if (!QFile::exists(pFilePath)) {
        return;
    } // if

    _qsVocabularyFile = pFilePath;

    CloseDatabase();
    OpenDatabase();
} // Open

const void Vocabulary::OpenDatabase()
{
	_qsdDatabase.setDatabaseName(_qsVocabularyFile);
	_qsdDatabase.open();
} // OpenDatabase

const void Vocabulary::RemoveCategory(const int &pCategoryId) const
{
	// record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	while (qsqQuery.next()) {
		int iRecord = qsqQuery.value(0).toInt();
		RemoveRecord(iRecord);
	} // while

    _qsdDatabase.exec("DELETE FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // RemoveCategory

#ifndef FREE
const void Vocabulary::RemoveField(const int &pFieldId) const
{
	_qsdDatabase.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
	_qsdDatabase.exec("DELETE FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // RemoveField
#endif

const void Vocabulary::RemoveRecord(const int &pCategoryId, const int &pRow) const
{
	// find record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecord = qsqQuery.value(0).toInt();

	RemoveRecord(iRecord);
} // RemoveRecord

const void Vocabulary::RemoveRecord(const int &pRecordId) const
{
	// remove record's data
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId));
	while (qsqQuery.next()) {
		int iDataId = qsqQuery.value(0).toInt();
		_qsdDatabase.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
	} // while

	_qsdDatabase.exec("DELETE FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
} // RemoveRecord

const int Vocabulary::Search(const QString &pWord, const int &pStartId) const
{
	QString qsWordLike = '%' + pWord + '%';
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_RECORDID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_TEXT + " LIKE '" + qsWordLike + "' GROUP BY " + COLUMN_RECORDID);
	if (!qsqQuery.next()) {
		return NOT_FOUND;
	} // if

	do {
		if (qsqQuery.value(0).toInt() >= pStartId) {
			return qsqQuery.value(0).toInt();
		} // if
	} while (qsqQuery.next());

	qsqQuery.seek(0);
	return qsqQuery.value(0).toInt();
} // Search

#ifndef FREE
const void Vocabulary::SetCategoryEnabled(const int &pCategoryId, const bool &pEnabled) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_ENABLED + " = " + QString::number(pEnabled) + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // SetCategoryEnabled
#endif

const void Vocabulary::SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const
{
	// find data record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecordId = qsqQuery.value(0).toInt();

	qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(iRecordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
	qsqQuery.next();
	int iDataId = qsqQuery.value(0).toInt();

	_qsdDatabase.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_TEXT + " = '" + pData + "' WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
} // SetDataText

#ifndef FREE
const void Vocabulary::SetFieldLanguage(const int &pFieldId, const eFieldLanguage &pLanguage) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_LANGUAGE + " = '" + QString::number(pLanguage) + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldLanguage

const void Vocabulary::SetFieldName(const int &pFieldId, const QString &pName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_NAME + " = '" + pName + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldName

const void Vocabulary::SetFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TEMPLATENAME + " = '" + pTemplateName + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldTemplateName
#endif

const void Vocabulary::SetSettings(const QString &pKey, const QString &pValue) const
{
	if (GetSettings(pKey).isEmpty()) {
		_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + pKey + "', '" + pValue + "')");
	} else {
		_qsdDatabase.exec("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + pValue + "' WHERE " + COLUMN_KEY + " = '" + pKey + "'");
	} // if else
} // SetSettings

Vocabulary::Vocabulary()
{
    // create empty database
    _qsdDatabase = QSqlDatabase::addDatabase("QSQLITE");
} // Vocabulary