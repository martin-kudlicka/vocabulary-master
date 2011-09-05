#include "vocabulary/vocabularydatabase.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#ifndef FREE
# include <QtSql/QSqlField>
#endif

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
const QString FIELD_NOTE1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note1");
const QString FIELD_NOTE2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note2");
const QString FIELD_WORD1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word1");
const QString FIELD_WORD2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word2");
const QString LEARNING_TEMPLATE1 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + FIELD_WORD1 + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + FIELD_NOTE1 + "</center>";
const QString LEARNING_TEMPLATE2 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + FIELD_WORD2 + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + FIELD_NOTE2 + "</center>";
#endif
const QString TABLE_CATEGORIES = "categories";
const QString TABLE_DATA = "data";
const QString TABLE_FIELDS = "fields";
const QString TABLE_RECORDS = "records";
const QString TABLE_SETTINGS = "settings";

VocabularyDatabase::~VocabularyDatabase()
{
	CloseDatabase();
} // ~VocabularyDatabase

const int VocabularyDatabase::AddCategory(const QString &pName) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_ENABLED + ") VALUES ('" + pName + "', '" + QString::number(true) + "')");
    return qsqQuery.lastInsertId().toInt();
} // AddCategory

#ifndef FREE
const void VocabularyDatabase::AddField() const
{
	int iNum = GetFieldCount() + 1;
	QString qsTemplate = tr("Field") + QString::number(iNum);
	QString qsName = tr("Name") + QString::number(iNum);

    _qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " + COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + qsTemplate + "', '" + qsName + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageLeft) + "')");
} // AddField
#endif

const int VocabularyDatabase::AddRecord(const int &pCategoryId) const
{
	// create new record
	QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(pCategoryId) + "')");
	int iRecordId = qsqQuery.lastInsertId().toInt();

    return iRecordId;
} // AddRecord

#ifndef FREE
const int VocabularyDatabase::AddRecord(const int &pCategoryId, const QStringList &pData) const
{
    // create new record
    QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(pCategoryId) + "')");
    int iRecordId = qsqQuery.lastInsertId().toInt();

    // create new data
    int iData = 0;
    foreach (int iFieldId, GetFieldIds()) {
        qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(iFieldId) + "', '" + QString::number(iRecordId) + "', '" + pData.at(iData) + "')");
        iData++;
    } // foreach

    return iRecordId;
} // AddRecord
#endif

const void VocabularyDatabase::BeginEdit()
{
    _qsdDatabase.transaction();
} // BeginEdit

const void VocabularyDatabase::CloseDatabase()
{
	if (_qsdDatabase.isOpen()) {
		_qsdDatabase.close();
	} // if
} // CloseDatabase

const void VocabularyDatabase::EndEdit(const bool &pSave /* true */)
{
    if (pSave) {
        _qsdDatabase.commit();
    } else {
        _qsdDatabase.rollback();
    } // if else
} // EndEdit

#ifndef FREE
const int VocabularyDatabase::GetCategoryCount() const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // GetCategoryCount
#endif

const bool VocabularyDatabase::GetCategoryEnabled(const int &pCategoryId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toBool();
} // GetCategoryEnabled

#ifndef FREE
const int VocabularyDatabase::GetCategoryId(const int &pRow) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    int iPosition = 0;
    while (qsqQuery.next()) {
        if (iPosition == pRow) {
            return qsqQuery.value(ColumnPosition1).toInt();
        } else {
            iPosition++;
        } // if else
    } // while

    return NOT_FOUND;
} // GetCategoryId
#endif

const VocabularyDatabase::tCategoryIdList VocabularyDatabase::GetCategoryIds() const
{
    tCategoryIdList tcilCategories;

    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    while (qsqQuery.next()) {
        tcilCategories.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return tcilCategories;
} // GetCategoryIds

const QString VocabularyDatabase::GetCategoryName(const int &pCategoryId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toString();
} // GetCategoryName

/*const QString VocabularyDatabase::GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const
{
    int iRecordId = GetRecordId(pCategoryId, pRow);
	return GetDataText(iRecordId, pFieldId);
} // GetDataText

const QString VocabularyDatabase::GetDataText(const int &pRecordId, const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TEXT + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
	    return qsqQuery.value(ColumnPosition1).toString();
    } else {
        return QString();
    } // if else
} // GetDataText*/

VocabularyDatabase::tRecordDataHash *VocabularyDatabase::GetDataText() const
{
    tRecordDataHash *trdhRecordData = new tRecordDataHash();

    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + " FROM " + TABLE_DATA);

	// set progress maximum
	if (qsqQuery.last()) {
		emit SetOpenProgressMax(qsqQuery.at() + 1);
	} // if

	if (qsqQuery.first()) {
		int iProgress = 0;
		do {
			tFieldDataHash *tfdhFieldData = &trdhRecordData->operator[](qsqQuery.value(ColumnPosition2).toInt());
			tfdhFieldData->insert(qsqQuery.value(ColumnPosition1).toInt(), qsqQuery.value(ColumnPosition3).toString());

            if (iProgress % OPENPROGRESS_REFRESHINTERVAL == 0) {
			    emit SetOpenProgressValue(iProgress);
            } // if
            iProgress++;
		} while (qsqQuery.next());
	} // if

    return trdhRecordData;
} // GetDataText

#ifndef FREE
const VocabularyDatabase::FieldAttributes VocabularyDatabase::GetFieldAttributes(const int &pFieldId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ATTRIBUTES + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toInt();
    } // if

    return FieldAttributeNone;
} // GetFieldAttributes
#endif

const int VocabularyDatabase::GetFieldCount() const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetFieldCount

const int VocabularyDatabase::GetFieldId(const int &pPosition) const
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

const VocabularyDatabase::tFieldIdList VocabularyDatabase::GetFieldIds() const
{
	tFieldIdList tfilIds;

	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	while (qsqQuery.next()) {
		tfilIds.append(qsqQuery.value(ColumnPosition1).toInt());
	} // while

	return tfilIds;
} // GetFieldIds

const VocabularyDatabase::eFieldLanguage VocabularyDatabase::GetFieldLanguage(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_LANGUAGE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<eFieldLanguage>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldLanguageUnknown;
} // GetFieldLanguage

const QString VocabularyDatabase::GetFieldName(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // GetFieldName

const QString VocabularyDatabase::GetFieldTemplateName(const int &pFieldId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TEMPLATENAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // GetFieldTemplateName

/*const VocabularyDatabase::eFieldType VocabularyDatabase::GetFieldType(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TYPE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<eFieldType>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldTypeUnknown;
} // GetFieldType*/

#ifndef FREE
const QString VocabularyDatabase::GetLanguageName(const eFieldLanguage &pLanguage) const
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

const QString VocabularyDatabase::GetName() const
{
    return QFileInfo(_qsVocabularyFile).completeBaseName();
} // GetName

const int VocabularyDatabase::GetRecordCategory(const int &pRecordId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
	qsqQuery.next();
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetRecordCategory

/*const int VocabularyDatabase::GetRecordCount() const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

const int VocabularyDatabase::GetRecordCount(const int &pCategoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

const int VocabularyDatabase::GetRecordCount(const bool &pEnabled) const
{
    QSqlQuery qsqQuery("SELECT " + TABLE_RECORDS + '.' + COLUMN_ID + " FROM " + TABLE_RECORDS + " JOIN " + TABLE_CATEGORIES + " ON " + TABLE_RECORDS + '.' + COLUMN_CATEGORYID + " = " + TABLE_CATEGORIES + '.' + COLUMN_ID + " WHERE " + TABLE_CATEGORIES + '.' + COLUMN_ENABLED + " = " + QString::number(pEnabled));
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // GetRecordCount*/

const int VocabularyDatabase::GetRecordId(const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	qsqQuery.seek(pRow);
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetRecordId

const int VocabularyDatabase::GetRecordId(const int &pCategoryId, const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetRecordId

/*#ifndef FREE
const VocabularyDatabase::tRecordIdList VocabularyDatabase::GetRecordIds() const
{
    tRecordIdList trilRecordIds;
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
    while (qsqQuery.next()) {
        trilRecordIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return trilRecordIds;
} // GetRecordIds
#endif*/

const VocabularyDatabase::tRecordIdList VocabularyDatabase::GetRecordIds(const int &pCategoryId) const
{
    tRecordIdList trilRecordIds;
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    while (qsqQuery.next()) {
        trilRecordIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return trilRecordIds;
} // GetRecordIds

const int VocabularyDatabase::GetRow(const int &pRecordId, const int &pCategoryId) const
{
	int iRow = 0;

	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	while (qsqQuery.next()) {
		if (qsqQuery.value(ColumnPosition1).toInt() == pRecordId) {
			return iRow;
		} else {
			iRow++;
		} // if else
	} // while

	return NOT_FOUND;
} // GetRow

const QString VocabularyDatabase::GetSettings(const QString &pKey) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + pKey + "'");
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} else {
		return QString();
	} // if else
} // GetSettings

const QString &VocabularyDatabase::GetVocabularyFile() const
{
    return _qsVocabularyFile;
} // GetVocabularyFile

const void VocabularyDatabase::Initialize() const
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

    // fill default data
#ifdef FREE
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_WORD1 + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_WORD1 + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageLeft) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_NOTE1 + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_NOTE1 + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageLeft) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_WORD2 + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_WORD2 + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageRight) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " /*+ COLUMN_TYPE + ", " */+ COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_LANGUAGE + ") VALUES ('" + FIELD_NOTE2 + "', '" /*+ QString::number(FieldTypeTextEdit) + "', '" */+ FIELD_NOTE2 + "', '" + QString::number(FieldAttributeNone) + "', '" + QString::number(FieldLanguageRight) + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " VALUES ('" + KEY_LEARNINGTEMPLATE1 + "', '" + LEARNING_TEMPLATE1 + "')");
	_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " VALUES ('" + KEY_LEARNINGTEMPLATE2 + "', '" + LEARNING_TEMPLATE2 + "')");
#endif
    SetSettings(KEY_LANGUAGE1, tr("Language1"));
    SetSettings(KEY_LANGUAGE2, tr("Language2"));
} // Initialize

const bool VocabularyDatabase::IsOpen() const
{
    return _qsdDatabase.isOpen();
} // IsOpen

const void VocabularyDatabase::New(const QString &pFilePath)
{
	_qsVocabularyFile = pFilePath;

	CloseDatabase();

	if (QFile::exists(pFilePath)) {
		QFile::remove(pFilePath);
	} // if

	OpenDatabase();
	Initialize();
} // New

#ifndef TRY
const void VocabularyDatabase::Open(const QString &pFilePath)
{
    if (!QFile::exists(pFilePath)) {
        return;
    } // if

	emit SetVocabularyName(QFileInfo(pFilePath).completeBaseName());
    _qsVocabularyFile = pFilePath;

    CloseDatabase();
    OpenDatabase();
} // Open
#endif

const void VocabularyDatabase::OpenDatabase()
{
#ifdef TRY
    _qsdDatabase.setDatabaseName(":memory:");
#else
	_qsdDatabase.setDatabaseName(_qsVocabularyFile);
#endif
	_qsdDatabase.open();
} // OpenDatabase

const void VocabularyDatabase::RemoveCategory(const int &pCategoryId) const
{
	// record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	while (qsqQuery.next()) {
		int iRecord = qsqQuery.value(ColumnPosition1).toInt();
		RemoveRecord(iRecord);
	} // while

    _qsdDatabase.exec("DELETE FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // RemoveCategory

#ifndef FREE
const void VocabularyDatabase::RemoveField(const int &pFieldId) const
{
	_qsdDatabase.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
	_qsdDatabase.exec("DELETE FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // RemoveField
#endif

const void VocabularyDatabase::RemoveRecord(const int &pCategoryId, const int &pRow) const
{
	// find record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecord = qsqQuery.value(ColumnPosition1).toInt();

	RemoveRecord(iRecord);
} // RemoveRecord

const void VocabularyDatabase::RemoveRecord(const int &pRecordId) const
{
	// remove record's data
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId));
	while (qsqQuery.next()) {
		int iDataId = qsqQuery.value(ColumnPosition1).toInt();
		_qsdDatabase.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
	} // while

	_qsdDatabase.exec("DELETE FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
} // RemoveRecord

const int VocabularyDatabase::Search(const QString &pWord, const int &pStartId) const
{
	QString qsWordLike = '%' + pWord + '%';
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_RECORDID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_TEXT + " LIKE '" + qsWordLike + "' GROUP BY " + COLUMN_RECORDID);
	if (!qsqQuery.next()) {
		return NOT_FOUND;
	} // if

	do {
		if (qsqQuery.value(ColumnPosition1).toInt() >= pStartId) {
			return qsqQuery.value(ColumnPosition1).toInt();
		} // if
	} while (qsqQuery.next());

	qsqQuery.seek(0);
	return qsqQuery.value(ColumnPosition1).toInt();
} // Search

#ifndef FREE
const void VocabularyDatabase::SetCategoryEnabled(const int &pCategoryId, const bool &pEnabled) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_ENABLED + " = " + QString::number(pEnabled) + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // SetCategoryEnabled
#endif

/*const void VocabularyDatabase::SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const
{
	// find data record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecordId = qsqQuery.value(ColumnPosition1).toInt();

    SetDataText(iRecordId, pFieldId, pData);
} // SetDataText*/

const void VocabularyDatabase::SetDataText(const int &pRecordId, const int &pFieldId, const QString &pData) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        int iDataId = qsqQuery.value(ColumnPosition1).toInt();
        _qsdDatabase.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_TEXT + " = '" + pData + "' WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
    } else {
        qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(pFieldId) + "', '" + QString::number(pRecordId) + "', '" + pData + "')");
    } // if else
} // SetDataText

#ifndef FREE
const void VocabularyDatabase::SetFieldAttributes(const int &pFieldId, const FieldAttributes &pAttributes) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_ATTRIBUTES + " = '" + QString::number(pAttributes) + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldAttributes

const void VocabularyDatabase::SetFieldLanguage(const int &pFieldId, const eFieldLanguage &pLanguage) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_LANGUAGE + " = '" + QString::number(pLanguage) + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldLanguage

const void VocabularyDatabase::SetFieldName(const int &pFieldId, const QString &pName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_NAME + " = '" + pName + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldName

const void VocabularyDatabase::SetFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TEMPLATENAME + " = '" + pTemplateName + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // SetFieldTemplateName
#endif

const void VocabularyDatabase::SetSettings(const QString &pKey, const QString &pValue) const
{
	if (GetSettings(pKey).isNull()) {
		_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + pKey + "', '" + pValue + "')");
	} else {
		_qsdDatabase.exec("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + pValue + "' WHERE " + COLUMN_KEY + " = '" + pKey + "'");
	} // if else
} // SetSettings

#ifndef FREE
const void VocabularyDatabase::SwapFields(const int &pSourceId, const int &pDestinationId) const
{
	QSqlQuery qsqSource = _qsdDatabase.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pSourceId));
	QSqlQuery qsqDestination = _qsdDatabase.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pDestinationId));

	qsqSource.next();
	qsqDestination.next();

	Update(TABLE_FIELDS, pDestinationId, qsqSource.record());
	Update(TABLE_FIELDS, pSourceId, qsqDestination.record());

	_qsdDatabase.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = 0 WHERE " + COLUMN_FIELDID + " = " + QString::number(pSourceId));
	_qsdDatabase.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(pSourceId) + " WHERE " + COLUMN_FIELDID + " = " + QString::number(pDestinationId));
	_qsdDatabase.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(pDestinationId) + " WHERE " + COLUMN_FIELDID + " = 0");
} // SwapFields

const void VocabularyDatabase::Update(const QString &pTable, const int &pColumnId, const QSqlRecord &pRecord) const
{
	QString qsQuery = "UPDATE " + pTable + " SET ";

	int iSet = 0;
	for (int iColumn = 0; iColumn < pRecord.count(); iColumn++) {
		QSqlField qsfField = pRecord.field(iColumn);
		if (qsfField.name() != COLUMN_ID) {
			if (iSet > 0) {
				qsQuery += ", ";
			} // if

			qsQuery += qsfField.name() + " = '" + qsfField.value().toString() + "'";
			iSet++;
		} // if
	} // for

	qsQuery += " WHERE " + COLUMN_ID + " = " + QString::number(pColumnId);

	_qsdDatabase.exec(qsQuery);
} // Update
#endif

VocabularyDatabase::VocabularyDatabase(QObject *pParent /* NULL */) : QObject(pParent)
{
    // create empty database
    _qsdDatabase = QSqlDatabase::addDatabase("QSQLITE");
} // VocabularyDatabase