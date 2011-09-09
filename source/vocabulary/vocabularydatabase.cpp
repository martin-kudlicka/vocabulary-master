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
const QString COLUMN_BUILTIN = "builtin";
const QString COLUMN_CATEGORYID = "category_id";
const QString COLUMN_ENABLED = "enabled";
const QString COLUMN_FIELDID = "field_id";
const QString COLUMN_ID = "id";
const QString COLUMN_KEY = "key";
const QString COLUMN_LANGUAGE = "language";
const QString COLUMN_LEARNINGTEMPLATE = "learning_template";
const QString COLUMN_NAME = "name";
const QString COLUMN_PRIORITY = "priority";
const QString COLUMN_RECORDID = "record_id";
const QString COLUMN_SPEECH = "speech";
const QString COLUMN_TEMPLATENAME = "template_name";
const QString COLUMN_TEXT = "text";
const QString COLUMN_TRAYTEMPLATE = "tray_template";
//const QString COLUMN_TYPE = "type";
const QString COLUMN_VALUE = "value";
const QString COLUMN_VOICE = "voice";
#ifdef FREE
const QString FIELD_NOTE1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note1");
const QString FIELD_NOTE2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note2");
const QString FIELD_WORD1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word1");
const QString FIELD_WORD2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word2");
#endif
const QString KEY_LANGUAGE1 = "language1";
const QString KEY_LANGUAGE2 = "language2";
const QString KEY_LEARNINGTEMPLATE1 = "learningtemplate1";
const QString KEY_LEARNINGTEMPLATE2 = "learningtemplate2";
#ifndef FREE
const QString KEY_SPEECH1 = "speech1";
const QString KEY_SPEECH2 = "speech2";
const QString KEY_TRAYTEMPLATE1 = "traytemplate1";
const QString KEY_TRAYTEMPLATE2 = "traytemplate2";
const QString KEY_VOICE1 = "voice1";
const QString KEY_VOICE2 = "voice2";
#endif
#ifndef TRY
const QString KEY_VERSION = "version";
#endif
#ifdef FREE
const QString LEARNING_TEMPLATE1 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + FIELD_WORD1 + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + FIELD_NOTE1 + "</center>";
const QString LEARNING_TEMPLATE2 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + FIELD_WORD2 + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + FIELD_NOTE2 + "</center>";
#endif
const int PRIORITY_DEFAULT = 1;
const QString TABLE_CATEGORIES = "categories";
const QString TABLE_DATA = "data";
const QString TABLE_FIELDS = "fields";
const QString TABLE_LANGUAGES = "languages";
const QString TABLE_RECORDS = "records";
const QString TABLE_SETTINGS = "settings";

VocabularyDatabase::~VocabularyDatabase()
{
	CloseDatabase();
} // ~VocabularyDatabase

const int VocabularyDatabase::AddCategory(const QString &pName) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_PRIORITY +  ", " + COLUMN_ENABLED + ") VALUES ('" + pName + "', '" + QString::number(PRIORITY_DEFAULT) + "', '" + QString::number(true) + "')");
    return qsqQuery.lastInsertId().toInt();
} // AddCategory

#ifndef FREE
const void VocabularyDatabase::AddField() const
{
	int iNum = GetFieldCount() + 1;
	QString qsTemplate = tr("Field") + QString::number(iNum);
	QString qsName = tr("Name") + QString::number(iNum);

    AddField(qsTemplate, qsName, FieldAttributeShow, FieldBuiltInNone, FieldLanguageLeft);
} // AddField

const void VocabularyDatabase::AddField(const QString &pTemplate, const QString &pName, const FieldAttributes &pAttributes, const eFieldBuiltIn &pBuiltIn, const eFieldLanguage &pLanguage) const
{
    _qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " + COLUMN_NAME + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_BUILTIN + ", " + COLUMN_LANGUAGE + ") VALUES ('" + pTemplate + "', '" + pName + "', '" + QString::number(pAttributes) + "', '" + QString::number(pBuiltIn) + "', '" + QString::number(pLanguage) + "')");
} // AddField
#endif

const void VocabularyDatabase::AddLanguage(const QString &pName, const QString &pLearningTemplate
#ifndef FREE
    , const QString &pTrayTemplate, const TTSInterface::eTTSPlugin &pTTSPlugin, const QString &pVoice
#endif
    ) const
{
    _qsdDatabase.exec("INSERT INTO " + TABLE_LANGUAGES + " (" + COLUMN_NAME + ", " + COLUMN_LEARNINGTEMPLATE + ", " + COLUMN_TRAYTEMPLATE + ", " + COLUMN_SPEECH + ", " + COLUMN_VOICE + ") VALUES ('" + pName + "', '" + pLearningTemplate + "', '" +
#ifndef FREE
        pTrayTemplate +
#endif
        "', '" +
#ifdef FREE
        QString::number(TTSInterface::TTPluginNone)
#else
        QString::number(pTTSPlugin)
#endif
        + "', '" +
#ifndef FREE
        pVoice +
#endif
        "')");
} // AddLanguage

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

const bool VocabularyDatabase::GetCategoryEnabled(const int &pCategoryId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toBool();
} // GetCategoryEnabled

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
const int VocabularyDatabase::GetCategoryPriority(const int &pCategoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_PRIORITY + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
	qsqQuery.next();
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetCategoryPriority

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
const VocabularyDatabase::tLanguageIdList VocabularyDatabase::GetLanguageIds() const
{
    tLanguageIdList tlilIds;

    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_LANGUAGES);
    while (qsqQuery.next()) {
        tlilIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while
    tlilIds.append(FieldLanguageAll);

    return tlilIds;
} // GetLanguageIds
#endif

const QString VocabularyDatabase::GetLanguageLearningTemplate(const int &pLanguageId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_LEARNINGTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // GetLanguageLearningTemplate

const QString VocabularyDatabase::GetLanguageName(const int &pLanguageId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } else {
        return tr("All");
    } // if else
} // GetLanguageName

#ifndef FREE
const TTSInterface::eTTSPlugin VocabularyDatabase::GetLanguageSpeech(const int &pLanguageId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_SPEECH + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
    if (qsqQuery.next()) {
        return static_cast<const TTSInterface::eTTSPlugin>(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return TTSInterface::TTPluginNone;
} // GetLanguageSpeech

const QString VocabularyDatabase::GetLanguageTrayTemplate(const int &pLanguageId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TRAYTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // GetLanguageTrayTemplate

const QString VocabularyDatabase::GetLanguageVoice(const int &pLanguageId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_VOICE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return TTSInterface::TTPluginNone;
} // GetLanguageVoice
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
					  + COLUMN_PRIORITY + " INTEGER,"
					  + COLUMN_ENABLED + " INTEGER NOT NULL)");
    _qsdDatabase.exec("CREATE TABLE " + TABLE_LANGUAGES + " ("
                      + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                      + COLUMN_NAME + " TEXT NOT NULL,"
                      + COLUMN_LEARNINGTEMPLATE + " TEXT NOT NULL,"
                      + COLUMN_TRAYTEMPLATE + " TEXT NOT NULL,"
                      + COLUMN_SPEECH + " INTEGER NOT NULL,"
                      + COLUMN_VOICE + " TEXT NOT NULL)");
	_qsdDatabase.exec("CREATE TABLE " + TABLE_FIELDS + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_TEMPLATENAME + " TEXT NOT NULL,"
					  //+ COLUMN_TYPE + " INTEGER NOT NULL,"
					  + COLUMN_NAME + " TEXT NOT NULL,"
					  + COLUMN_ATTRIBUTES + " INTEGER NOT NULL,"
                      + COLUMN_BUILTIN + " INTEGER NOT NULL,"
					  + COLUMN_LANGUAGE + " INTEGER REFERENCES " + TABLE_LANGUAGES + " ON DELETE CASCADE)");
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
    AddLanguage(tr("Language1"),
#ifdef FREE
        LEARNING_TEMPLATE1
#else
        QString(), QString(), TTSInterface::TTPluginNone, QString()
#endif
        );
    AddLanguage(tr("Language2"),
#ifdef FREE
        LEARNING_TEMPLATE2
#else
        QString(), QString(), TTSInterface::TTPluginNone, QString()
#endif
        );
    AddField(tr("Enabled"), tr("Enabled"), FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInEnabled, FieldLanguageAll);
#ifdef FREE
    AddField(FIELD_WORD1,/*QString::number(FieldTypeTextEdit),*/ FIELD_WORD1, QString::number(FieldAttributeNone), FieldBuiltInNone, QString::number(FieldLanguageLeft));
    AddField(FIELD_NOTE1,/*QString::number(FieldTypeTextEdit),*/ FIELD_NOTE1, QString::number(FieldAttributeNone), FieldBuiltInNone, QString::number(FieldLanguageLeft));
    AddField(FIELD_WORD2,/*QString::number(FieldTypeTextEdit),*/ FIELD_WORD2, QString::number(FieldAttributeNone), FieldBuiltInNone, QString::number(FieldLanguageRight));
    AddField(FIELD_NOTE2,/*QString::number(FieldTypeTextEdit),*/ FIELD_NOTE2, QString::number(FieldAttributeNone), FieldBuiltInNone, QString::number(FieldLanguageRight));
#endif
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

    UpdateDatabase();
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

const void VocabularyDatabase::SetCategoryPriority(const int &pCategoryId, const int &pPriority) const
{
	_qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(pPriority) + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // SetCategoryPriority
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

const void VocabularyDatabase::SetLanguageLearningTemplate(const int &pLanguageId, const QString &pTemplate) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_LEARNINGTEMPLATE + " = '" + pTemplate + "' WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
} // SetLanguageLearningTemplateconst
#endif

const void VocabularyDatabase::SetLanguageName(const int &pLanguageId, const QString &pName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_NAME + " = '" + pName + "' WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
} // SetLanguageName

#ifndef FREE
const void VocabularyDatabase::SetLanguageSpeech(const int &pLanguageId, const TTSInterface::eTTSPlugin &pSpeech) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_SPEECH + " = '" + QString::number(pSpeech)+ "' WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
} // SetLanguageSpeech

const void VocabularyDatabase::SetLanguageTrayTemplate(const int &pLanguageId, const QString &pTemplate) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_TRAYTEMPLATE + " = '" + pTemplate + "' WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
} // SetLanguageTrayTemplate

const void VocabularyDatabase::SetLanguageVoice(const int &pLanguageId, const QString &pVoice) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_VOICE + " = '" + pVoice + "' WHERE " + COLUMN_ID + " = " + QString::number(pLanguageId));
} // SetLanguageVoice
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

#ifndef TRY
const void VocabularyDatabase::UpdateDatabase() const
{
    QString qsVersion = GetSettings(KEY_VERSION);
    eVersion evCurrent;
    if (qsVersion.isEmpty()) {
        evCurrent = Version1;
    } else {
        evCurrent = static_cast<eVersion>(qsVersion.toInt());
    } // if else

    if (evCurrent < Version2) {
        // add priority column to categories table
        _qsdDatabase.exec("ALTER TABLE " + TABLE_CATEGORIES + " ADD " + COLUMN_PRIORITY + " INTEGER");
        _qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(PRIORITY_DEFAULT));

        // add show attribute to fields
        tFieldIdList tfilFieldIds = GetFieldIds();
        foreach (int iFieldId, tfilFieldIds) {
            FieldAttributes faAttributes = GetFieldAttributes(iFieldId);
            faAttributes |= FieldAttributeShow;
            SetFieldAttributes(iFieldId, faAttributes);
        } // foreach

        // create language table
        _qsdDatabase.exec("CREATE TABLE " + TABLE_LANGUAGES + " ("
                          + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          + COLUMN_NAME + " TEXT NOT NULL,"
                          + COLUMN_LEARNINGTEMPLATE + " TEXT NOT NULL,"
                          + COLUMN_TRAYTEMPLATE + " TEXT NOT NULL,"
                          + COLUMN_SPEECH + " INTEGER NOT NULL,"
                          + COLUMN_VOICE + " TEXT NOT NULL)");
        // copy existing language settings values to language table
        AddLanguage(GetSettings(KEY_LANGUAGE1), GetSettings(KEY_LEARNINGTEMPLATE1)
#ifndef FREE
            , GetSettings(KEY_TRAYTEMPLATE1), static_cast<TTSInterface::eTTSPlugin>(GetSettings(KEY_SPEECH1).toInt()), GetSettings(KEY_VOICE1)
#endif
            );
        AddLanguage(GetSettings(KEY_LANGUAGE2), GetSettings(KEY_LEARNINGTEMPLATE2)
#ifndef FREE
            , GetSettings(KEY_TRAYTEMPLATE2), static_cast<TTSInterface::eTTSPlugin>(GetSettings(KEY_SPEECH2).toInt()), GetSettings(KEY_VOICE2)
#endif
            );
        // delete old language settings
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE1 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE1 + "'");
#ifndef FREE
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH1 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE1 + "'");
#endif
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE2 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE2 + "'");
#ifndef FREE
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH2 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE2 + "'");
#endif

        // add builtin column to fields table
        _qsdDatabase.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_BUILTIN + " INTEGER");
        _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_BUILTIN + " = " + QString::number(FieldBuiltInNone));

        // add enable/disable field
        AddField(tr("Enabled"), tr("Enabled"), FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInEnabled, FieldLanguageAll);

        SetSettings(KEY_VERSION, QString::number(Version2));
    } // if
} // UpdateDatabase
#endif

VocabularyDatabase::VocabularyDatabase(QObject *pParent /* NULL */) : QObject(pParent)
{
    // create empty database
    _qsdDatabase = QSqlDatabase::addDatabase("QSQLITE");
} // VocabularyDatabase