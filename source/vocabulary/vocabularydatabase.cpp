#include "vocabulary/vocabularydatabase.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#ifndef EDITION_FREE
# include <QtSql/QSqlField>
#else
# include "../plugins/common/tts-interface.h"
#endif

#ifdef EDITION_TRY
int VocabularyDatabase::_iMemoryVocabularies = 0;
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
const QString COLUMN_TYPE = "type";
const QString COLUMN_VALUE = "value";
const QString COLUMN_VOICE = "voice";
#ifdef EDITION_FREE
const char *FIELD_NOTE1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note1");
const char *FIELD_NOTE2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note2");
const char *FIELD_WORD1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word1");
const char *FIELD_WORD2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word2");
#endif
const QString KEY_LANGUAGE1 = "language1";
const QString KEY_LANGUAGE2 = "language2";
const QString KEY_LEARNINGTEMPLATE1 = "learningtemplate1";
const QString KEY_LEARNINGTEMPLATE2 = "learningtemplate2";
#ifndef EDITION_FREE
const QString KEY_SPEECH1 = "speech1";
const QString KEY_SPEECH2 = "speech2";
const QString KEY_TRAYTEMPLATE1 = "traytemplate1";
const QString KEY_TRAYTEMPLATE2 = "traytemplate2";
const QString KEY_VOICE1 = "voice1";
const QString KEY_VOICE2 = "voice2";
#endif
#ifndef EDITION_TRY
const QString KEY_VERSION = "version";
#endif
const QString TABLE_CATEGORIES = "categories";
const QString TABLE_DATA = "data";
const QString TABLE_FIELDS = "fields";
const QString TABLE_LANGUAGES = "languages";
const QString TABLE_RECORDS = "records";
const QString TABLE_SETTINGS = "settings";

VocabularyDatabase::VocabularyDatabase(QObject *pParent /* NULL */) : QObject(pParent)
{
	// create empty database
	_qsdDatabase = QSqlDatabase::addDatabase("QSQLITE", QString::number(qrand()));
} // VocabularyDatabase

VocabularyDatabase::~VocabularyDatabase()
{
	CloseDatabase();
} // ~VocabularyDatabase

int VocabularyDatabase::addCategory(const QString &pName) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_PRIORITY +  ", " + COLUMN_ENABLED + ") VALUES ('" + pName + "', '" + QString::number(PRIORITY_DEFAULT) + "', '" + QString::number(true) + "')");
    return qsqQuery.lastInsertId().toInt();
} // addCategory

#ifndef EDITION_FREE
int VocabularyDatabase::addField() const
{
	int iNum = fieldCount() + 1;
	QString qsTemplate = tr("Field") + QString::number(iNum);
	QString qsName = tr("Name") + QString::number(iNum);

    return addField(qsTemplate, qsName, FieldTypeLineEdit, FieldAttributeShow, FieldBuiltInNone, FieldLanguageLeft);
} // addField
#endif

int VocabularyDatabase::addField(const QString &pTemplate, const QString &pName, const FieldType &pType, const FieldAttributes &pAttributes, const FieldBuiltIn &pBuiltIn, const FieldLanguage &pLanguage) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " + COLUMN_NAME + ", " + COLUMN_TYPE + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_BUILTIN + ", " + COLUMN_LANGUAGE + ") VALUES ('" + pTemplate + "', '" + pName + "', '" + QString::number(pType) + "', '" + QString::number(pAttributes) + "', '" + QString::number(pBuiltIn) + "', '" + QString::number(pLanguage) + "')");
	return qsqQuery.lastInsertId().toInt();
} // addField

void VocabularyDatabase::AddLanguage(const QString &pName, const QString &pLearningTemplate
#ifndef EDITION_FREE
    , const QString &pTrayTemplate, const TTSInterface::TTSPlugin &pTTSPlugin, const QString &pVoice
#endif
    ) const
{
    _qsdDatabase.exec("INSERT INTO " + TABLE_LANGUAGES + " (" + COLUMN_NAME + ", " + COLUMN_LEARNINGTEMPLATE + ", " + COLUMN_TRAYTEMPLATE + ", " + COLUMN_SPEECH + ", " + COLUMN_VOICE + ") VALUES ('" + pName + "', '" + pLearningTemplate + "', '" +
#ifndef EDITION_FREE
        pTrayTemplate +
#endif
        "', '" +
#ifdef EDITION_FREE
        QString::number(TTSInterface::TTPluginNone)
#else
        QString::number(pTTSPlugin)
#endif
        + "', '" +
#ifndef EDITION_FREE
        pVoice +
#endif
        "')");
} // AddLanguage

int VocabularyDatabase::addRecord(const int &pCategoryId) const
{
	// create new record
	QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(pCategoryId) + "')");
	int iRecordId = qsqQuery.lastInsertId().toInt();

    return iRecordId;
} // addRecord

#ifndef EDITION_FREE
int VocabularyDatabase::addRecord(const int &pCategoryId, const QStringList &pData) const
{
    // create new record
    QSqlQuery qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(pCategoryId) + "')");
    int iRecordId = qsqQuery.lastInsertId().toInt();

    // create new data
    int iData = 0;
    foreach (int iFieldId, fieldIds()) {
        qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(iFieldId) + "', '" + QString::number(iRecordId) + "', '" + pData.at(iData) + "')");
        iData++;
    } // foreach

    return iRecordId;
} // addRecord
#endif

void VocabularyDatabase::beginEdit()
{
    _qsdDatabase.transaction();
} // beginEdit

void VocabularyDatabase::close()
{
	CloseDatabase();
} // close

void VocabularyDatabase::CloseDatabase()
{
	if (_qsdDatabase.isOpen()) {
		_qsdDatabase.close();
	} // if
} // CloseDatabase

void VocabularyDatabase::endEdit(const bool &pSave /* true */)
{
    if (pSave) {
        _qsdDatabase.commit();
    } else {
        _qsdDatabase.rollback();
    } // if else
} // endEdit

#ifndef EDITION_FREE
quint8 VocabularyDatabase::categoryCount() const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // categoryCount

bool VocabularyDatabase::categoryEnabled(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toBool();
} // categoryEnabled

quint8 VocabularyDatabase::categoryId(quint8 row) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    int iPosition = 0;
    while (qsqQuery.next()) {
		if (iPosition == row) {
            return qsqQuery.value(ColumnPosition1).toInt();
        } else {
            iPosition++;
        } // if else
    } // while

    return NOT_FOUND;
} // categoryId
#endif

VocabularyDatabase::CategoryIdList VocabularyDatabase::categoryIds() const
{
    CategoryIdList tcilCategories;

    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    while (qsqQuery.next()) {
        tcilCategories.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return tcilCategories;
} // categoryIds

QString VocabularyDatabase::categoryName(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toString();
} // categoryName

/*QString VocabularyDatabase::dataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const
{
    int iRecordId = GetRecordId(pCategoryId, pRow);
	return dataText(iRecordId, pFieldId);
} // dataText

QString VocabularyDatabase::dataText(const int &pRecordId, const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TEXT + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
	    return qsqQuery.value(ColumnPosition1).toString();
    } else {
        return QString();
    } // if else
} // dataText*/

VocabularyDatabase::RecordDataHash *VocabularyDatabase::dataText() const
{
    RecordDataHash *trdhRecordData = new RecordDataHash();

    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + " FROM " + TABLE_DATA);

	// set progress maximum
	if (qsqQuery.last()) {
		emit SetOpenProgressMax(qsqQuery.at() + 1);
	} // if

	if (qsqQuery.first()) {
		int iProgress = 0;
		do {
			DataHash *tdhData = &trdhRecordData->operator[](qsqQuery.value(ColumnPosition2).toInt());
			tdhData->insert(qsqQuery.value(ColumnPosition1).toInt(), qsqQuery.value(ColumnPosition3).toString());

            if (iProgress % OPENPROGRESS_REFRESHINTERVAL == 0) {
			    emit SetOpenProgressValue(iProgress);
            } // if
            iProgress++;
		} while (qsqQuery.next());
	} // if

    return trdhRecordData;
} // dataText

#ifndef EDITION_FREE
quint8 VocabularyDatabase::categoryPriority(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_PRIORITY + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
	qsqQuery.next();
	return qsqQuery.value(ColumnPosition1).toInt();
} // categoryPriority
#endif

VocabularyDatabase::FieldAttributes VocabularyDatabase::fieldAttributes(const int &pFieldId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ATTRIBUTES + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toInt();
    } // if

    return FieldAttributeNone;
} // fieldAttributes

#ifndef EDITION_FREE
VocabularyDatabase::FieldBuiltIn VocabularyDatabase::fieldBuiltIn(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_BUILTIN + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<FieldBuiltIn>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldBuiltInNone;
} // fieldBuiltIn

int VocabularyDatabase::fieldCount() const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // fieldCount
#endif

/*int VocabularyDatabase::GetFieldId(const int &pPosition) const
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
} // GetFieldId*/

VocabularyDatabase::FieldIdList VocabularyDatabase::fieldIds() const
{
	FieldIdList tfilIds;

	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	while (qsqQuery.next()) {
		tfilIds.append(qsqQuery.value(ColumnPosition1).toInt());
	} // while

	return tfilIds;
} // fieldIds

VocabularyDatabase::FieldLanguage VocabularyDatabase::fieldLanguage(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_LANGUAGE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<FieldLanguage>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldLanguageUnknown;
} // fieldLanguage

QString VocabularyDatabase::fieldName(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // fieldName

QString VocabularyDatabase::fieldTemplateName(const int &pFieldId) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TEMPLATENAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // fieldTemplateName

VocabularyDatabase::FieldType VocabularyDatabase::fieldType(const int &pFieldId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TYPE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
	if (qsqQuery.next()) {
		return static_cast<FieldType>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldTypeUnknown;
} // fieldType

bool VocabularyDatabase::isOpen() const
{
	return _qsdDatabase.isOpen();
} // isOpen

#ifndef EDITION_FREE
VocabularyDatabase::LanguageIdList VocabularyDatabase::languageIds(LanguageIds type) const
{
    LanguageIdList tlilIds;

	if (type & LanguageIdsUserDefined) {
		QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_LANGUAGES);
		while (qsqQuery.next()) {
			tlilIds.append(qsqQuery.value(ColumnPosition1).toInt());
		} // while
	} // if

	if (type & LanguageIdsAllOnly) {
		tlilIds.append(FieldLanguageAll);
	} // if

    return tlilIds;
} // languageIds
#endif

QString VocabularyDatabase::languageLearningTemplate(quint8 languageId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_LEARNINGTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // languageLearningTemplate

QString VocabularyDatabase::languageName(quint8 languageId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } else {
        return tr("All");
    } // if else
} // languageName

#ifndef EDITION_FREE
TTSInterface::TTSPlugin VocabularyDatabase::languageSpeech(quint8 languageId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_SPEECH + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return static_cast<const TTSInterface::TTSPlugin>(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return TTSInterface::TTPluginNone;
} // languageSpeech

QString VocabularyDatabase::languageTrayTemplate(quint8 languageId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_TRAYTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // languageTrayTemplate

QString VocabularyDatabase::languageVoice(quint8 languageId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_VOICE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return TTSInterface::TTPluginNone;
} // languageVoice
#endif

QString VocabularyDatabase::name() const
{
#ifdef EDITION_TRY
	return tr("memory %1").arg(_iVocabularyNumber);
#else
    return QFileInfo(_qsVocabularyFile).completeBaseName();
#endif
} // name

/*int VocabularyDatabase::GetRecordCategory(const int &pRecordId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
	qsqQuery.next();
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetRecordCategory

int VocabularyDatabase::GetRecordCount() const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

int VocabularyDatabase::GetRecordCount(const int &pCategoryId) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // GetRecordCount

int VocabularyDatabase::GetRecordCount(const bool &pEnabled) const
{
    QSqlQuery qsqQuery("SELECT " + TABLE_RECORDS + '.' + COLUMN_ID + " FROM " + TABLE_RECORDS + " JOIN " + TABLE_CATEGORIES + " ON " + TABLE_RECORDS + '.' + COLUMN_CATEGORYID + " = " + TABLE_CATEGORIES + '.' + COLUMN_ID + " WHERE " + TABLE_CATEGORIES + '.' + COLUMN_ENABLED + " = " + QString::number(pEnabled));
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // GetRecordCount

int VocabularyDatabase::GetRecordId(const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	qsqQuery.seek(pRow);
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetRecordId

int VocabularyDatabase::GetRecordId(const int &pCategoryId, const int &pRow) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	return qsqQuery.value(ColumnPosition1).toInt();
} // GetRecordId*/

#ifndef EDITION_TRY
VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds() const
{
    RecordIdList trilRecordIds;
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
    while (qsqQuery.next()) {
        trilRecordIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return trilRecordIds;
} // recordIds
#endif

VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds(const int &pCategoryId) const
{
    RecordIdList trilRecordIds;
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
    while (qsqQuery.next()) {
        trilRecordIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return trilRecordIds;
} // recordIds

quint32 VocabularyDatabase::row(quint32 recordId, quint8 categoryId) const
{
	int iRow = 0;

	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	while (qsqQuery.next()) {
		if (qsqQuery.value(ColumnPosition1).toInt() == recordId) {
			return iRow;
		} else {
			iRow++;
		} // if else
	} // while

	return NOT_FOUND;
} // row

void VocabularyDatabase::Initialize() const
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
					  + COLUMN_NAME + " TEXT NOT NULL,"
					  + COLUMN_TYPE + " INTEGER NOT NULL,"
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
					  + COLUMN_TEXT + " TEXT NOT NULL)");

    // fill default data
#ifdef EDITION_FREE
	const QString LEARNING_TEMPLATE1 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + tr(FIELD_WORD1) + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + tr(FIELD_NOTE1) + "</center>";
	const QString LEARNING_TEMPLATE2 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + tr(FIELD_WORD2) + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + tr(FIELD_NOTE2) + "</center>";
#endif
    AddLanguage(tr("Language1"),
#ifdef EDITION_FREE
        LEARNING_TEMPLATE1
#else
        QString(), QString(), TTSInterface::TTPluginNone, QString()
#endif
        );
    AddLanguage(tr("Language2"),
#ifdef EDITION_FREE
        LEARNING_TEMPLATE2
#else
        QString(), QString(), TTSInterface::TTPluginNone, QString()
#endif
        );
#ifdef EDITION_FREE
    addField(tr(FIELD_WORD1), tr(FIELD_WORD1), FieldTypeLineEdit, FieldAttributeNone, FieldBuiltInNone, FieldLanguageLeft);
    addField(tr(FIELD_NOTE1), tr(FIELD_NOTE1), FieldTypeLineEdit, FieldAttributeNone, FieldBuiltInNone, FieldLanguageLeft);
    addField(tr(FIELD_WORD2), tr(FIELD_WORD2), FieldTypeLineEdit, FieldAttributeNone, FieldBuiltInNone, FieldLanguageRight);
    addField(tr(FIELD_NOTE2), tr(FIELD_NOTE2), FieldTypeLineEdit, FieldAttributeNone, FieldBuiltInNone, FieldLanguageRight);
#endif
	addField(tr("Enabled"), "", FieldTypeCheckBox, FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInEnabled, FieldLanguageAll);
	addField(tr("Priority"), "", FieldTypeSpinBox, FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInPriority, FieldLanguageAll);

#ifndef EDITION_TRY
    setSettings(KEY_VERSION, QString::number(Version2));
#endif
} // Initialize

void VocabularyDatabase::new2(
#ifndef EDITION_TRY
	const QString &pFilePath
#endif
	)
{
#ifdef EDITION_TRY
	_iVocabularyNumber = ++_iMemoryVocabularies;
#else
	_qsVocabularyFile = pFilePath;

	if (QFile::exists(pFilePath)) {
		QFile::remove(pFilePath);
	} // if
#endif

	OpenDatabase();
	Initialize();
} // new

#ifndef EDITION_TRY
void VocabularyDatabase::open(const QString &pFilePath)
{
    if (!QFile::exists(pFilePath)) {
        return;
    } // if

    _qsVocabularyFile = pFilePath;
	emit SetVocabularyName(name());

    OpenDatabase();

    UpdateDatabase();
} // open
#endif

void VocabularyDatabase::OpenDatabase()
{
#ifdef EDITION_TRY
    _qsdDatabase.setDatabaseName(":memory:");
#else
	_qsdDatabase.setDatabaseName(_qsVocabularyFile);
#endif
	_qsdDatabase.open();
} // OpenDatabase

void VocabularyDatabase::removeCategory(const int &pCategoryId) const
{
	// record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	while (qsqQuery.next()) {
		int iRecord = qsqQuery.value(ColumnPosition1).toInt();
		removeRecord(iRecord);
	} // while

    _qsdDatabase.exec("DELETE FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(pCategoryId));
} // removeCategory

#ifndef EDITION_FREE
void VocabularyDatabase::removeField(const int &pFieldId) const
{
	_qsdDatabase.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
	_qsdDatabase.exec("DELETE FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // removeField
#endif

void VocabularyDatabase::removeRecord(const int &pCategoryId, const int &pRow) const
{
	// find record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecord = qsqQuery.value(ColumnPosition1).toInt();

	removeRecord(iRecord);
} // removeRecord

void VocabularyDatabase::removeRecord(const int &pRecordId) const
{
	// remove record's data
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId));
	while (qsqQuery.next()) {
		int iDataId = qsqQuery.value(ColumnPosition1).toInt();
		_qsdDatabase.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
	} // while

	_qsdDatabase.exec("DELETE FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
} // removeRecord

quint32 VocabularyDatabase::search(const QString &word, quint32 startId) const
{
	QString qsWordLike = '%' + word + '%';
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_RECORDID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_TEXT + " LIKE '" + qsWordLike + "' GROUP BY " + COLUMN_RECORDID);
	if (!qsqQuery.next()) {
		return NOT_FOUND;
	} // if

	do {
		if (qsqQuery.value(ColumnPosition1).toUInt() >= startId) {
			return qsqQuery.value(ColumnPosition1).toInt();
		} // if
	} while (qsqQuery.next());

	qsqQuery.seek(0);
	return qsqQuery.value(ColumnPosition1).toInt();
} // search

#ifndef EDITION_FREE
void VocabularyDatabase::setCategoryEnabled(quint8 categoryId, bool enabled) const
{
	_qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_ENABLED + " = " + QString::number(enabled) + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
} // setCategoryEnabled

void VocabularyDatabase::setCategoryPriority(quint8 categoryId, quint8 priority) const
{
	_qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(priority) + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
} // setCategoryPriority
#endif

/*void VocabularyDatabase::setDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData) const
{
	// find data record
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId));
	qsqQuery.seek(pRow);
	int iRecordId = qsqQuery.value(ColumnPosition1).toInt();

    setDataText(iRecordId, pFieldId, pData);
} // setDataText*/

void VocabularyDatabase::setDataText(const int &pRecordId, const int &pFieldId, const QString &pData) const
{
    QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(pRecordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(pFieldId));
    if (qsqQuery.next()) {
        int iDataId = qsqQuery.value(ColumnPosition1).toInt();
        _qsdDatabase.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_TEXT + " = '" + pData + "' WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
    } else {
        qsqQuery = _qsdDatabase.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(pFieldId) + "', '" + QString::number(pRecordId) + "', '" + pData + "')");
    } // if else
} // setDataText

void VocabularyDatabase::setFieldAttributes(const int &pFieldId, const FieldAttributes &pAttributes) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_ATTRIBUTES + " = '" + QString::number(pAttributes) + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // setFieldAttributes

#ifndef EDITION_FREE
void VocabularyDatabase::setFieldLanguage(const int &pFieldId, const FieldLanguage &pLanguage) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_LANGUAGE + " = '" + QString::number(pLanguage) + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // setFieldLanguage

void VocabularyDatabase::setFieldName(const int &pFieldId, const QString &pName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_NAME + " = '" + pName + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // setFieldName

void VocabularyDatabase::setFieldTemplateName(const int &pFieldId, const QString &pTemplateName) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TEMPLATENAME + " = '" + pTemplateName + "' WHERE " + COLUMN_ID + " = " + QString::number(pFieldId));
} // setFieldTemplateName

void VocabularyDatabase::setLanguageLearningTemplate(quint8 languageId, const QString &templateText) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_LEARNINGTEMPLATE + " = '" + templateText + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // SetLanguageLearningTemplateconst
#endif

void VocabularyDatabase::setLanguageName(quint8 languageId, const QString &name) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_NAME + " = '" + name + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageName

#ifndef EDITION_FREE
void VocabularyDatabase::setLanguageSpeech(quint8 languageId, TTSInterface::TTSPlugin speech) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_SPEECH + " = '" + QString::number(speech)+ "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageSpeech

void VocabularyDatabase::setLanguageTrayTemplate(quint8 languageId, const QString &templateText) const
{
    _qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_TRAYTEMPLATE + " = '" + templateText + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageTrayTemplate

void VocabularyDatabase::setLanguageVoice(quint8 languageId, const QString &voice) const
{
	_qsdDatabase.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_VOICE + " = '" + voice + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageVoice

void VocabularyDatabase::SetRecordCategory(const int &pRecordId, const int &pCategoryId) const
{
	_qsdDatabase.exec("UPDATE " + TABLE_RECORDS + " SET " + COLUMN_CATEGORYID + " = " + QString::number(pCategoryId) + " WHERE " + COLUMN_ID + " = " + QString::number(pRecordId));
} // SetRecordCategory
#endif

void VocabularyDatabase::setSettings(const QString &key, const QString &value) const
{
	if (settings(key).isNull()) {
		_qsdDatabase.exec("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + key + "', '" + value + "')");
	} else {
		_qsdDatabase.exec("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + value + "' WHERE " + COLUMN_KEY + " = '" + key + "'");
	} // if else
} // setSettings

QString VocabularyDatabase::settings(const QString &key) const
{
	QSqlQuery qsqQuery = _qsdDatabase.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + key + "'");
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} else {
		return QString();
	} // if else
} // settings

#ifndef EDITION_FREE
void VocabularyDatabase::swapFields(const int &pSourceId, const int &pDestinationId) const
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
} // swapFields

#ifndef EDITION_TRY
const QString &VocabularyDatabase::vocabularyFile() const
{
    return _qsVocabularyFile;
} // vocabularyFile
#endif

void VocabularyDatabase::Update(const QString &pTable, const int &pColumnId, const QSqlRecord &pRecord) const
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

#ifndef EDITION_TRY
void VocabularyDatabase::UpdateDatabase()
{
    QString qsVersion = settings(KEY_VERSION);
    eVersion evCurrent;
    if (qsVersion.isEmpty()) {
        evCurrent = Version1;
    } else {
        evCurrent = static_cast<eVersion>(qsVersion.toInt());
    } // if else

	beginEdit();

    if (evCurrent < Version2) {
        // add priority column to categories table
        _qsdDatabase.exec("ALTER TABLE " + TABLE_CATEGORIES + " ADD " + COLUMN_PRIORITY + " INTEGER");
        _qsdDatabase.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(PRIORITY_DEFAULT));

        // add show attribute to fields
        FieldIdList tfilFieldIds = fieldIds();
        foreach (int iFieldId, tfilFieldIds) {
            FieldAttributes qfaAttributes = fieldAttributes(iFieldId);
            qfaAttributes |= FieldAttributeShow;
            setFieldAttributes(iFieldId, qfaAttributes);
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
        AddLanguage(settings(KEY_LANGUAGE1), settings(KEY_LEARNINGTEMPLATE1)
#ifndef EDITION_FREE
            , settings(KEY_TRAYTEMPLATE1), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH1).toInt()), settings(KEY_VOICE1)
#endif
            );
        AddLanguage(settings(KEY_LANGUAGE2), settings(KEY_LEARNINGTEMPLATE2)
#ifndef EDITION_FREE
            , settings(KEY_TRAYTEMPLATE2), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH2).toInt()), settings(KEY_VOICE2)
#endif
            );
        // delete old language settings
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE1 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE1 + "'");
#ifndef EDITION_FREE
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH1 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE1 + "'");
#endif
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE2 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE2 + "'");
#ifndef EDITION_FREE
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH2 + "'");
        _qsdDatabase.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE2 + "'");
#endif

		// add type column to fields table
		_qsdDatabase.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_TYPE + " INTEGER");
		_qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TYPE + " = " + QString::number(FieldTypeLineEdit));

        // add builtin column to fields table
        _qsdDatabase.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_BUILTIN + " INTEGER");
        _qsdDatabase.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_BUILTIN + " = " + QString::number(FieldBuiltInNone));

        // add enable/disable field
        addField(tr("Enabled"), "", FieldTypeCheckBox, FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInEnabled, FieldLanguageAll);

		// add priority field
		addField(tr("Priority"), "", FieldTypeSpinBox, FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInPriority, FieldLanguageAll);

        setSettings(KEY_VERSION, QString::number(Version2));
    } // if

	endEdit(true);
} // UpdateDatabase
#endif