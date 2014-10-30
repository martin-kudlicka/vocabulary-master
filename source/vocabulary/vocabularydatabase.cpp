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
int VocabularyDatabase::_memoryVocabularies = 0;
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
	_database = QSqlDatabase::addDatabase("QSQLITE", QString::number(qrand()));
} // VocabularyDatabase

VocabularyDatabase::~VocabularyDatabase()
{
	closeDatabase();
} // ~VocabularyDatabase

quint8 VocabularyDatabase::addCategory(const QString &name) const
{
	QSqlQuery qsqQuery = _database.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_PRIORITY +  ", " + COLUMN_ENABLED + ") VALUES ('" + name + "', '" + QString::number(PRIORITY_DEFAULT) + "', '" + QString::number(true) + "')");
    return qsqQuery.lastInsertId().toInt();
} // addCategory

#ifndef EDITION_FREE
quint8 VocabularyDatabase::addField() const
{
	int iNum = fieldCount() + 1;
	QString qsTemplate = tr("Field") + QString::number(iNum);
	QString qsName = tr("Name") + QString::number(iNum);

    return addField(qsTemplate, qsName, FieldTypeLineEdit, FieldAttributeShow, FieldBuiltInNone, FieldLanguageLeft);
} // addField
#endif

quint8 VocabularyDatabase::addField(const QString &templateText, const QString &name, const FieldType &type, FieldAttributes attributes, FieldBuiltIn builtIn, FieldLanguage language) const
{
    QSqlQuery qsqQuery = _database.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " + COLUMN_NAME + ", " + COLUMN_TYPE + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_BUILTIN + ", " + COLUMN_LANGUAGE + ") VALUES ('" + templateText + "', '" + name + "', '" + QString::number(type) + "', '" + QString::number(attributes) + "', '" + QString::number(builtIn) + "', '" + QString::number(language) + "')");
	return qsqQuery.lastInsertId().toInt();
} // addField

void VocabularyDatabase::addLanguage(const QString &name, const QString &learningTemplate
#ifndef EDITION_FREE
    , const QString &trayTemplate, TTSInterface::TTSPlugin ttsPlugin, const QString &voice
#endif
    ) const
{
    _database.exec("INSERT INTO " + TABLE_LANGUAGES + " (" + COLUMN_NAME + ", " + COLUMN_LEARNINGTEMPLATE + ", " + COLUMN_TRAYTEMPLATE + ", " + COLUMN_SPEECH + ", " + COLUMN_VOICE + ") VALUES ('" + name + "', '" + learningTemplate + "', '" +
#ifndef EDITION_FREE
        trayTemplate +
#endif
        "', '" +
#ifdef EDITION_FREE
        QString::number(TTSInterface::TTPluginNone)
#else
        QString::number(ttsPlugin)
#endif
        + "', '" +
#ifndef EDITION_FREE
        voice +
#endif
        "')");
} // addLanguage

quint32 VocabularyDatabase::addRecord(quint8 categoryId) const
{
	// create new record
	QSqlQuery qsqQuery = _database.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(categoryId) + "')");
	int iRecordId = qsqQuery.lastInsertId().toInt();

    return iRecordId;
} // addRecord

#ifndef EDITION_FREE
quint32 VocabularyDatabase::addRecord(quint8 categoryId, const QStringList &data) const
{
    // create new record
    QSqlQuery qsqQuery = _database.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(categoryId) + "')");
    int iRecordId = qsqQuery.lastInsertId().toInt();

    // create new data
    int iData = 0;
    foreach (int iFieldId, fieldIds()) {
        qsqQuery = _database.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(iFieldId) + "', '" + QString::number(iRecordId) + "', '" + data.at(iData) + "')");
        iData++;
    } // foreach

    return iRecordId;
} // addRecord
#endif

void VocabularyDatabase::beginEdit()
{
    _database.transaction();
} // beginEdit

void VocabularyDatabase::close()
{
	closeDatabase();
} // close

void VocabularyDatabase::closeDatabase()
{
	if (_database.isOpen()) {
		_database.close();
	} // if
} // closeDatabase

void VocabularyDatabase::endEdit(bool save /* true */)
{
    if (save) {
        _database.commit();
    } else {
        _database.rollback();
    } // if else
} // endEdit

#ifndef EDITION_FREE
quint8 VocabularyDatabase::categoryCount() const
{
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // categoryCount

bool VocabularyDatabase::categoryEnabled(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toBool();
} // categoryEnabled

quint8 VocabularyDatabase::categoryId(quint8 row) const
{
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
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

    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    while (qsqQuery.next()) {
        tcilCategories.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return tcilCategories;
} // categoryIds

QString VocabularyDatabase::categoryName(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
    qsqQuery.next();
    return qsqQuery.value(ColumnPosition1).toString();
} // categoryName

/*QString VocabularyDatabase::dataText(quint8 categoryId, quint32 row, quint8 fieldId) const
{
    int iRecordId = GetRecordId(categoryId, row);
	return dataText(iRecordId, fieldId);
} // dataText

QString VocabularyDatabase::dataText(quint32 recordId, quint8 fieldId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_TEXT + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(fieldId));
    if (qsqQuery.next()) {
	    return qsqQuery.value(ColumnPosition1).toString();
    } else {
        return QString();
    } // if else
} // dataText*/

VocabularyDatabase::RecordDataHash *VocabularyDatabase::dataText() const
{
    RecordDataHash *trdhRecordData = new RecordDataHash();

    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + " FROM " + TABLE_DATA);

	// set progress maximum
	if (qsqQuery.last()) {
		emit setOpenProgressMax(qsqQuery.at() + 1);
	} // if

	if (qsqQuery.first()) {
		int iProgress = 0;
		do {
			DataHash *tdhData = &trdhRecordData->operator[](qsqQuery.value(ColumnPosition2).toInt());
			tdhData->insert(qsqQuery.value(ColumnPosition1).toInt(), qsqQuery.value(ColumnPosition3).toString());

            if (iProgress % OPENPROGRESS_REFRESHINTERVAL == 0) {
			    emit setOpenProgressValue(iProgress);
            } // if
            iProgress++;
		} while (qsqQuery.next());
	} // if

    return trdhRecordData;
} // dataText

#ifndef EDITION_FREE
quint8 VocabularyDatabase::categoryPriority(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_PRIORITY + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
	qsqQuery.next();
	return qsqQuery.value(ColumnPosition1).toInt();
} // categoryPriority
#endif

VocabularyDatabase::FieldAttributes VocabularyDatabase::fieldAttributes(quint8 fieldId) const
{
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ATTRIBUTES + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toInt();
    } // if

    return FieldAttributeNone;
} // fieldAttributes

#ifndef EDITION_FREE
VocabularyDatabase::FieldBuiltIn VocabularyDatabase::fieldBuiltIn(quint8 fieldId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_BUILTIN + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (qsqQuery.next()) {
		return static_cast<FieldBuiltIn>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldBuiltInNone;
} // fieldBuiltIn

quint8 VocabularyDatabase::fieldCount() const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // fieldCount
#endif

/*quint8 VocabularyDatabase::getFieldId(quint8 position) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	int iPosition = 0;
	while (qsqQuery.next()) {
		if (iPosition == position) {
			return qsqQuery.value(ColumnPosition1).toInt();
		} else {
			iPosition++;
		} // if else
	} // while

	return NOT_FOUND;
} // getFieldId*/

VocabularyDatabase::FieldIdList VocabularyDatabase::fieldIds() const
{
	FieldIdList tfilIds;

	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	while (qsqQuery.next()) {
		tfilIds.append(qsqQuery.value(ColumnPosition1).toInt());
	} // while

	return tfilIds;
} // fieldIds

VocabularyDatabase::FieldLanguage VocabularyDatabase::fieldLanguage(quint8 fieldId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_LANGUAGE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (qsqQuery.next()) {
		return static_cast<FieldLanguage>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldLanguageUnknown;
} // fieldLanguage

QString VocabularyDatabase::fieldName(quint8 fieldId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // fieldName

QString VocabularyDatabase::fieldTemplateName(quint8 fieldId) const
{
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_TEMPLATENAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // fieldTemplateName

VocabularyDatabase::FieldType VocabularyDatabase::fieldType(quint8 fieldId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_TYPE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (qsqQuery.next()) {
		return static_cast<FieldType>(qsqQuery.value(ColumnPosition1).toInt());
	} // if

	return FieldTypeUnknown;
} // fieldType

bool VocabularyDatabase::isOpen() const
{
	return _database.isOpen();
} // isOpen

#ifndef EDITION_FREE
VocabularyDatabase::LanguageIdList VocabularyDatabase::languageIds(LanguageIds type) const
{
    LanguageIdList tlilIds;

	if (type & LanguageIdsUserDefined) {
		QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_LANGUAGES);
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
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_LEARNINGTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return QString();
} // languageLearningTemplate

QString VocabularyDatabase::languageName(quint8 languageId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } else {
        return tr("All");
    } // if else
} // languageName

#ifndef EDITION_FREE
TTSInterface::TTSPlugin VocabularyDatabase::languageSpeech(quint8 languageId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_SPEECH + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return static_cast<const TTSInterface::TTSPlugin>(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return TTSInterface::TTPluginNone;
} // languageSpeech

QString VocabularyDatabase::languageTrayTemplate(quint8 languageId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_TRAYTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} // while

	return QString();
} // languageTrayTemplate

QString VocabularyDatabase::languageVoice(quint8 languageId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_VOICE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (qsqQuery.next()) {
        return qsqQuery.value(ColumnPosition1).toString();
    } // while

    return TTSInterface::TTPluginNone;
} // languageVoice
#endif

QString VocabularyDatabase::name() const
{
#ifdef EDITION_TRY
	return tr("memory %1").arg(_vocabularyNumber);
#else
    return QFileInfo(_vocabularyFile).completeBaseName();
#endif
} // name

/*quint8 VocabularyDatabase::recordCategory(quint32 recordId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
	qsqQuery.next();
	return qsqQuery.value(ColumnPosition1).toInt();
} // recordCategory

quint32 VocabularyDatabase::recordCount() const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // recordCount

quint32 VocabularyDatabase::recordCount(quint8 categoryId) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	if (qsqQuery.last()) {
		return qsqQuery.at() + 1;
	} else {
		return 0;
	} // if else
} // recordCount

quint32 VocabularyDatabase::recordCount(bool enabled) const
{
    QSqlQuery qsqQuery("SELECT " + TABLE_RECORDS + '.' + COLUMN_ID + " FROM " + TABLE_RECORDS + " JOIN " + TABLE_CATEGORIES + " ON " + TABLE_RECORDS + '.' + COLUMN_CATEGORYID + " = " + TABLE_CATEGORIES + '.' + COLUMN_ID + " WHERE " + TABLE_CATEGORIES + '.' + COLUMN_ENABLED + " = " + QString::number(enabled));
    if (qsqQuery.last()) {
        return qsqQuery.at() + 1;
    } else {
        return 0;
    } // if else
} // recordCount

quint32 VocabularyDatabase::recordId(quint32 row) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	qsqQuery.seek(row);
	return qsqQuery.value(ColumnPosition1).toInt();
} // recordId

quint32 VocabularyDatabase::recordId(quint8 categoryId, quint32 row) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	qsqQuery.seek(row);
	return qsqQuery.value(ColumnPosition1).toInt();
} // recordId*/

#ifndef EDITION_TRY
VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds() const
{
    RecordIdList trilRecordIds;
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
    while (qsqQuery.next()) {
        trilRecordIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return trilRecordIds;
} // recordIds
#endif

VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds(quint8 categoryId) const
{
    RecordIdList trilRecordIds;
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
    while (qsqQuery.next()) {
        trilRecordIds.append(qsqQuery.value(ColumnPosition1).toInt());
    } // while

    return trilRecordIds;
} // recordIds

quint32 VocabularyDatabase::row(quint32 recordId, quint8 categoryId) const
{
	int iRow = 0;

	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	while (qsqQuery.next()) {
		if (qsqQuery.value(ColumnPosition1).toInt() == recordId) {
			return iRow;
		} else {
			iRow++;
		} // if else
	} // while

	return NOT_FOUND;
} // row

void VocabularyDatabase::initialize() const
{
    _database.exec("CREATE TABLE " + TABLE_SETTINGS + " ("
					  + COLUMN_KEY + " TEXT NOT NULL,"
					  + COLUMN_VALUE + " TEXT NOT NULL)");
    _database.exec("CREATE TABLE " + TABLE_CATEGORIES + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_NAME + " TEXT NOT NULL,"
					  + COLUMN_PRIORITY + " INTEGER,"
					  + COLUMN_ENABLED + " INTEGER NOT NULL)");
    _database.exec("CREATE TABLE " + TABLE_LANGUAGES + " ("
                      + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                      + COLUMN_NAME + " TEXT NOT NULL,"
                      + COLUMN_LEARNINGTEMPLATE + " TEXT NOT NULL,"
                      + COLUMN_TRAYTEMPLATE + " TEXT NOT NULL,"
                      + COLUMN_SPEECH + " INTEGER NOT NULL,"
                      + COLUMN_VOICE + " TEXT NOT NULL)");
	_database.exec("CREATE TABLE " + TABLE_FIELDS + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_TEMPLATENAME + " TEXT NOT NULL,"
					  + COLUMN_NAME + " TEXT NOT NULL,"
					  + COLUMN_TYPE + " INTEGER NOT NULL,"
					  + COLUMN_ATTRIBUTES + " INTEGER NOT NULL,"
                      + COLUMN_BUILTIN + " INTEGER NOT NULL,"
					  + COLUMN_LANGUAGE + " INTEGER REFERENCES " + TABLE_LANGUAGES + " ON DELETE CASCADE)");
	_database.exec("CREATE TABLE " + TABLE_RECORDS + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_CATEGORYID + " INTEGER REFERENCES " + TABLE_CATEGORIES + " ON DELETE CASCADE)");
    _database.exec("CREATE TABLE " + TABLE_DATA + " ("
					  + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
					  + COLUMN_FIELDID + " INTEGER REFERENCES " + TABLE_FIELDS + " ON DELETE CASCADE,"
					  + COLUMN_RECORDID + " INTEGER REFERENCES " + TABLE_RECORDS + " ON DELETE CASCADE,"
					  + COLUMN_TEXT + " TEXT NOT NULL)");

    // fill default data
#ifdef EDITION_FREE
	const QString LEARNING_TEMPLATE1 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + tr(FIELD_WORD1) + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + tr(FIELD_NOTE1) + "</center>";
	const QString LEARNING_TEMPLATE2 = "<center style=\"font-size:20px\">" + VARIABLE_MARK + tr(FIELD_WORD2) + "</center><center style=\"font-size:10px\">" + VARIABLE_MARK + tr(FIELD_NOTE2) + "</center>";
#endif
    addLanguage(tr("Language1"),
#ifdef EDITION_FREE
        LEARNING_TEMPLATE1
#else
        QString(), QString(), TTSInterface::TTPluginNone, QString()
#endif
        );
    addLanguage(tr("Language2"),
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
} // initialize

void VocabularyDatabase::new2(
#ifndef EDITION_TRY
	const QString &filePath
#endif
	)
{
#ifdef EDITION_TRY
	_vocabularyNumber = ++_memoryVocabularies;
#else
	_vocabularyFile = filePath;

	if (QFile::exists(filePath)) {
		QFile::remove(filePath);
	} // if
#endif

	openDatabase();
	initialize();
} // new

#ifndef EDITION_TRY
void VocabularyDatabase::open(const QString &filePath)
{
    if (!QFile::exists(filePath)) {
        return;
    } // if

    _vocabularyFile = filePath;
	emit setVocabularyName(name());

    openDatabase();

    updateDatabase();
} // open
#endif

void VocabularyDatabase::openDatabase()
{
#ifdef EDITION_TRY
    _database.setDatabaseName(":memory:");
#else
	_database.setDatabaseName(_vocabularyFile);
#endif
	_database.open();
} // openDatabase

void VocabularyDatabase::removeCategory(quint8 categoryId) const
{
	// record
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	while (qsqQuery.next()) {
		int iRecord = qsqQuery.value(ColumnPosition1).toInt();
		removeRecord(iRecord);
	} // while

    _database.exec("DELETE FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
} // removeCategory

#ifndef EDITION_FREE
void VocabularyDatabase::removeField(quint8 fieldId) const
{
	_database.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_FIELDID + " = " + QString::number(fieldId));
	_database.exec("DELETE FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
} // removeField
#endif

void VocabularyDatabase::removeRecord(quint8 categoryId, quint32 row) const
{
	// find record
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	qsqQuery.seek(row);
	int iRecord = qsqQuery.value(ColumnPosition1).toInt();

	removeRecord(iRecord);
} // removeRecord

void VocabularyDatabase::removeRecord(quint32 recordId) const
{
	// remove record's data
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId));
	while (qsqQuery.next()) {
		int iDataId = qsqQuery.value(ColumnPosition1).toInt();
		_database.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
	} // while

	_database.exec("DELETE FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
} // removeRecord

quint32 VocabularyDatabase::search(const QString &word, quint32 startId) const
{
	QString qsWordLike = '%' + word + '%';
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_RECORDID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_TEXT + " LIKE '" + qsWordLike + "' GROUP BY " + COLUMN_RECORDID);
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
	_database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_ENABLED + " = " + QString::number(enabled) + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
} // setCategoryEnabled

void VocabularyDatabase::setCategoryPriority(quint8 categoryId, quint8 priority) const
{
	_database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(priority) + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
} // setCategoryPriority
#endif

/*void VocabularyDatabase::setDataText(quint8 categoryId, quint32 row, quint8 fieldId, const QString &data) const
{
	// find data record
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	qsqQuery.seek(row);
	int iRecordId = qsqQuery.value(ColumnPosition1).toInt();

    setDataText(iRecordId, fieldId, data);
} // setDataText*/

void VocabularyDatabase::setDataText(quint32 recordId, quint8 fieldId, const QString &data) const
{
    QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(fieldId));
    if (qsqQuery.next()) {
        int iDataId = qsqQuery.value(ColumnPosition1).toInt();
        _database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_TEXT + " = '" + data + "' WHERE " + COLUMN_ID + " = " + QString::number(iDataId));
    } else {
        qsqQuery = _database.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(fieldId) + "', '" + QString::number(recordId) + "', '" + data + "')");
    } // if else
} // setDataText

void VocabularyDatabase::setFieldAttributes(quint8 fieldId, FieldAttributes attributes) const
{
    _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_ATTRIBUTES + " = '" + QString::number(attributes) + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
} // setFieldAttributes

#ifndef EDITION_FREE
void VocabularyDatabase::setFieldLanguage(quint8 fieldId, FieldLanguage language) const
{
    _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_LANGUAGE + " = '" + QString::number(language) + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
} // setFieldLanguage

void VocabularyDatabase::setFieldName(quint8 fieldId, const QString &name) const
{
    _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_NAME + " = '" + name + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
} // setFieldName

void VocabularyDatabase::setFieldTemplateName(quint8 fieldId, const QString &templateName) const
{
    _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TEMPLATENAME + " = '" + templateName + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
} // setFieldTemplateName

void VocabularyDatabase::setLanguageLearningTemplate(quint8 languageId, const QString &templateText) const
{
    _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_LEARNINGTEMPLATE + " = '" + templateText + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // SetLanguageLearningTemplateconst
#endif

void VocabularyDatabase::setLanguageName(quint8 languageId, const QString &name) const
{
    _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_NAME + " = '" + name + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageName

#ifndef EDITION_FREE
void VocabularyDatabase::setLanguageSpeech(quint8 languageId, TTSInterface::TTSPlugin speech) const
{
    _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_SPEECH + " = '" + QString::number(speech)+ "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageSpeech

void VocabularyDatabase::setLanguageTrayTemplate(quint8 languageId, const QString &templateText) const
{
    _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_TRAYTEMPLATE + " = '" + templateText + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageTrayTemplate

void VocabularyDatabase::setLanguageVoice(quint8 languageId, const QString &voice) const
{
	_database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_VOICE + " = '" + voice + "' WHERE " + COLUMN_ID + " = " + QString::number(languageId));
} // setLanguageVoice

void VocabularyDatabase::SetRecordCategory(quint32 recordId, quint8 categoryId) const
{
	_database.exec("UPDATE " + TABLE_RECORDS + " SET " + COLUMN_CATEGORYID + " = " + QString::number(categoryId) + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
} // SetRecordCategory
#endif

void VocabularyDatabase::setSettings(const QString &key, const QString &value) const
{
	if (settings(key).isNull()) {
		_database.exec("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + key + "', '" + value + "')");
	} else {
		_database.exec("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + value + "' WHERE " + COLUMN_KEY + " = '" + key + "'");
	} // if else
} // setSettings

QString VocabularyDatabase::settings(const QString &key) const
{
	QSqlQuery qsqQuery = _database.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + key + "'");
	if (qsqQuery.next()) {
		return qsqQuery.value(ColumnPosition1).toString();
	} else {
		return QString();
	} // if else
} // settings

#ifndef EDITION_FREE
void VocabularyDatabase::swapFields(quint8 sourceId, quint8 destinationId) const
{
	QSqlQuery qsqSource = _database.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(sourceId));
	QSqlQuery qsqDestination = _database.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(destinationId));

	qsqSource.next();
	qsqDestination.next();

	update(TABLE_FIELDS, destinationId, qsqSource.record());
	update(TABLE_FIELDS, sourceId, qsqDestination.record());

	_database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = 0 WHERE " + COLUMN_FIELDID + " = " + QString::number(sourceId));
	_database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(sourceId) + " WHERE " + COLUMN_FIELDID + " = " + QString::number(destinationId));
	_database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(destinationId) + " WHERE " + COLUMN_FIELDID + " = 0");
} // swapFields

#ifndef EDITION_TRY
const QString &VocabularyDatabase::vocabularyFile() const
{
    return _vocabularyFile;
} // vocabularyFile
#endif

void VocabularyDatabase::update(const QString &table, quint8 columnId, const QSqlRecord &record) const
{
	QString qsQuery = "UPDATE " + table + " SET ";

	int iSet = 0;
	for (int iColumn = 0; iColumn < record.count(); iColumn++) {
		QSqlField qsfField = record.field(iColumn);
		if (qsfField.name() != COLUMN_ID) {
			if (iSet > 0) {
				qsQuery += ", ";
			} // if

			qsQuery += qsfField.name() + " = '" + qsfField.value().toString() + "'";
			iSet++;
		} // if
	} // for

	qsQuery += " WHERE " + COLUMN_ID + " = " + QString::number(columnId);

	_database.exec(qsQuery);
} // update
#endif

#ifndef EDITION_TRY
void VocabularyDatabase::updateDatabase()
{
    QString qsVersion = settings(KEY_VERSION);
    Version evCurrent;
    if (qsVersion.isEmpty()) {
        evCurrent = Version1;
    } else {
        evCurrent = static_cast<Version>(qsVersion.toInt());
    } // if else

	beginEdit();

    if (evCurrent < Version2) {
        // add priority column to categories table
        _database.exec("ALTER TABLE " + TABLE_CATEGORIES + " ADD " + COLUMN_PRIORITY + " INTEGER");
        _database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(PRIORITY_DEFAULT));

        // add show attribute to fields
        FieldIdList tfilFieldIds = fieldIds();
        foreach (int iFieldId, tfilFieldIds) {
            FieldAttributes qfaAttributes = fieldAttributes(iFieldId);
            qfaAttributes |= FieldAttributeShow;
            setFieldAttributes(iFieldId, qfaAttributes);
        } // foreach

        // create language table
        _database.exec("CREATE TABLE " + TABLE_LANGUAGES + " ("
                          + COLUMN_ID + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                          + COLUMN_NAME + " TEXT NOT NULL,"
                          + COLUMN_LEARNINGTEMPLATE + " TEXT NOT NULL,"
                          + COLUMN_TRAYTEMPLATE + " TEXT NOT NULL,"
                          + COLUMN_SPEECH + " INTEGER NOT NULL,"
                          + COLUMN_VOICE + " TEXT NOT NULL)");
        // copy existing language settings values to language table
        addLanguage(settings(KEY_LANGUAGE1), settings(KEY_LEARNINGTEMPLATE1)
#ifndef EDITION_FREE
            , settings(KEY_TRAYTEMPLATE1), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH1).toInt()), settings(KEY_VOICE1)
#endif
            );
        addLanguage(settings(KEY_LANGUAGE2), settings(KEY_LEARNINGTEMPLATE2)
#ifndef EDITION_FREE
            , settings(KEY_TRAYTEMPLATE2), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH2).toInt()), settings(KEY_VOICE2)
#endif
            );
        // delete old language settings
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE1 + "'");
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE1 + "'");
#ifndef EDITION_FREE
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH1 + "'");
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE1 + "'");
#endif
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE2 + "'");
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE2 + "'");
#ifndef EDITION_FREE
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH2 + "'");
        _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE2 + "'");
#endif

		// add type column to fields table
		_database.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_TYPE + " INTEGER");
		_database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TYPE + " = " + QString::number(FieldTypeLineEdit));

        // add builtin column to fields table
        _database.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_BUILTIN + " INTEGER");
        _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_BUILTIN + " = " + QString::number(FieldBuiltInNone));

        // add enable/disable field
        addField(tr("Enabled"), "", FieldTypeCheckBox, FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInEnabled, FieldLanguageAll);

		// add priority field
		addField(tr("Priority"), "", FieldTypeSpinBox, FieldAttributeShow | FieldAttributeBuiltIn, FieldBuiltInPriority, FieldLanguageAll);

        setSettings(KEY_VERSION, QString::number(Version2));
    } // if

	endEdit(true);
} // updateDatabase
#endif