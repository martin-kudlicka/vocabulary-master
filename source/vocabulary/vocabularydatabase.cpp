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
quint8 VocabularyDatabase::_memoryVocabularies = 0;
#endif

#ifdef EDITION_FREE
const char *FIELD_NOTE1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note1");
const char *FIELD_NOTE2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Note2");
const char *FIELD_WORD1 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word1");
const char *FIELD_WORD2 = QT_TRANSLATE_NOOP("VocabularyDatabase", "Word2");
#endif

const QString COLUMN_ATTRIBUTES       = "attributes";
const QString COLUMN_BUILTIN          = "builtin";
const QString COLUMN_CATEGORYID       = "category_id";
const QString COLUMN_ENABLED          = "enabled";
const QString COLUMN_FIELDID          = "field_id";
const QString COLUMN_ID               = "id";
const QString COLUMN_KEY              = "key";
const QString COLUMN_LANGUAGE         = "language";
const QString COLUMN_LEARNINGTEMPLATE = "learning_template";
const QString COLUMN_NAME             = "name";
const QString COLUMN_PRIORITY         = "priority";
const QString COLUMN_RECORDID         = "record_id";
const QString COLUMN_SPEECH           = "speech";
const QString COLUMN_TEMPLATENAME     = "template_name";
const QString COLUMN_TEXT             = "text";
const QString COLUMN_TRAYTEMPLATE     = "tray_template";
const QString COLUMN_TYPE             = "type";
const QString COLUMN_VALUE            = "value";
const QString COLUMN_VOICE            = "voice";
const QString KEY_LANGUAGE1           = "language1";
const QString KEY_LANGUAGE2           = "language2";
const QString KEY_LEARNINGTEMPLATE1   = "learningtemplate1";
const QString KEY_LEARNINGTEMPLATE2   = "learningtemplate2";
#ifndef EDITION_FREE
const QString KEY_SPEECH1             = "speech1";
const QString KEY_SPEECH2             = "speech2";
const QString KEY_TRAYTEMPLATE1       = "traytemplate1";
const QString KEY_TRAYTEMPLATE2       = "traytemplate2";
const QString KEY_VOICE1              = "voice1";
const QString KEY_VOICE2              = "voice2";
#endif
#ifndef EDITION_TRY
const QString KEY_VERSION             = "version";
#endif
const QString TABLE_CATEGORIES        = "categories";
const QString TABLE_DATA              = "data";
const QString TABLE_FIELDS            = "fields";
const QString TABLE_LANGUAGES         = "languages";
const QString TABLE_RECORDS           = "records";
const QString TABLE_SETTINGS          = "settings";

VocabularyDatabase::VocabularyDatabase(QObject *parent /* NULL */) : QObject(parent)
{
	// create empty database
	_database = QSqlDatabase::addDatabase("QSQLITE", QString::number(qrand()));
} // VocabularyDatabase

VocabularyDatabase::~VocabularyDatabase()
{
	closeDatabase();
} // ~VocabularyDatabase

#ifndef EDITION_FREE
quint8 VocabularyDatabase::categoryCount() const
{
    QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    if (query.last())
	{
        return query.at() + 1;
    }
	else
	{
        return 0;
    } // if else
} // categoryCount

bool VocabularyDatabase::categoryEnabled(quint8 categoryId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
    query.next();
    return query.value(ColumnPosition1).toBool();
} // categoryEnabled

quint8 VocabularyDatabase::categoryId(quint8 row) const
{
    QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    quint8 position = 0;
    while (query.next())
	{
		if (position == row)
		{
            return query.value(ColumnPosition1).toUInt();
        }
		else
		{
            position++;
        } // if else
    } // while

    return NOT_FOUND;
} // categoryId
#endif

QString VocabularyDatabase::categoryName(quint8 categoryId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
    query.next();
    return query.value(ColumnPosition1).toString();
} // categoryName

#ifndef EDITION_FREE
quint8 VocabularyDatabase::categoryPriority(quint8 categoryId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_PRIORITY + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
	query.next();
	return query.value(ColumnPosition1).toUInt();
} // categoryPriority

bool VocabularyDatabase::isOpen() const
{
	return _database.isOpen();
} // isOpen

VocabularyDatabase::LanguageIdList VocabularyDatabase::languageIds(LanguageIds type) const
{
    LanguageIdList languageIdList;

	if (type & LanguageIdsUserDefined)
	{
		QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_LANGUAGES);
		while (query.next())
		{
			languageIdList.append(query.value(ColumnPosition1).toUInt());
		} // while
	} // if

	if (type & LanguageIdsAllOnly)
	{
		languageIdList.append(FieldLanguageAll);
	} // if

    return languageIdList;
} // languageIds
#endif

QString VocabularyDatabase::languageLearningTemplate(quint8 languageId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_LEARNINGTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (query.next())
	{
        return query.value(ColumnPosition1).toString();
    } // while

    return QString();
} // languageLearningTemplate

QString VocabularyDatabase::languageName(quint8 languageId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (query.next())
	{
        return query.value(ColumnPosition1).toString();
    }
	else
	{
        return tr("All");
    } // if else
} // languageName

#ifndef EDITION_FREE
TTSInterface::TTSPlugin VocabularyDatabase::languageSpeech(quint8 languageId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_SPEECH + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (query.next())
	{
        return static_cast<const TTSInterface::TTSPlugin>(query.value(ColumnPosition1).toUInt());
    } // while

    return TTSInterface::TTPluginNone;
} // languageSpeech

QString VocabularyDatabase::languageTrayTemplate(quint8 languageId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_TRAYTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
	if (query.next())
	{
		return query.value(ColumnPosition1).toString();
	} // while

	return QString();
} // languageTrayTemplate

QString VocabularyDatabase::languageVoice(quint8 languageId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_VOICE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(languageId));
    if (query.next())
	{
        return query.value(ColumnPosition1).toString();
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

quint32 VocabularyDatabase::row(quint32 recordId, quint8 categoryId) const
{
	quint32 row = 0;

	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	while (query.next())
	{
		if (query.value(ColumnPosition1).toUInt() == recordId)
		{
			return row;
		}
		else
		{
			row++;
		} // if else
	} // while

	return NOT_FOUND;
} // row

quint32 VocabularyDatabase::search(const QString &word, quint32 startId) const
{
	const QString wordLike = '%' + word + '%';
	QSqlQuery query = _database.exec("SELECT " + COLUMN_RECORDID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_TEXT + " LIKE '" + wordLike + "' GROUP BY " + COLUMN_RECORDID);
	if (!query.next())
	{
		return NOT_FOUND;
	} // if

	do
	{
		if (query.value(ColumnPosition1).toUInt() >= startId)
		{
			return query.value(ColumnPosition1).toUInt();
		} // if
	} while (query.next());

	query.seek(0);
	return query.value(ColumnPosition1).toUInt();
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
#endif

void VocabularyDatabase::setSettings(const QString &key, const QString &value) const
{
	if (settings(key).isNull())
	{
		_database.exec("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + key + "', '" + value + "')");
	}
	else
	{
		_database.exec("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + value + "' WHERE " + COLUMN_KEY + " = '" + key + "'");
	} // if else
} // setSettings

QString VocabularyDatabase::settings(const QString &key) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + key + "'");
	if (query.next())
	{
		return query.value(ColumnPosition1).toString();
	}
	else
	{
		return QString();
	} // if else
} // settings

#ifndef EDITION_TRY
const QString &VocabularyDatabase::vocabularyFile() const
{
    return _vocabularyFile;
} // vocabularyFile
#endif

quint8 VocabularyDatabase::addCategory(const QString &name) const
{
	const QSqlQuery query = _database.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_PRIORITY +  ", " + COLUMN_ENABLED + ") VALUES ('" + name + "', '" + QString::number(PRIORITY_DEFAULT) + "', '" + QString::number(true) + "')");
    return query.lastInsertId().toUInt();
} // addCategory

#ifndef EDITION_FREE
quint8 VocabularyDatabase::addField() const
{
	const quint8 num           = fieldCount() + 1;
	const QString templateText = tr("Field") + QString::number(num);
	const QString name         = tr("Name") + QString::number(num);

    return addField(templateText, name, FieldTypeLineEdit, FieldAttributeShow, FieldBuiltInNone, FieldLanguageLeft);
} // addField
#endif

quint32 VocabularyDatabase::addRecord(quint8 categoryId) const
{
	// create new record
	const QSqlQuery query  = _database.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(categoryId) + "')");
	const quint32 recordId = query.lastInsertId().toUInt();

    return recordId;
} // addRecord

#ifndef EDITION_FREE
quint32 VocabularyDatabase::addRecord(quint8 categoryId, const QStringList &data) const
{
    // create new record
    QSqlQuery query        = _database.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(categoryId) + "')");
    const quint32 recordId = query.lastInsertId().toUInt();

    // create new data
    quint8 dataIndex = 0;
    foreach (quint8 fieldId, fieldIds())
	{
		query = _database.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(fieldId) + "', '" + QString::number(recordId) + "', '" + data.at(dataIndex) + "')");
		dataIndex++;
    } // foreach

    return recordId;
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

void VocabularyDatabase::endEdit(bool save /* true */)
{
    if (save)
	{
        _database.commit();
    }
	else
	{
        _database.rollback();
    } // if else
} // endEdit

VocabularyDatabase::CategoryIdList VocabularyDatabase::categoryIds() const
{
    CategoryIdList categories;

    QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
    while (query.next())
	{
        categories.append(query.value(ColumnPosition1).toUInt());
    } // while

    return categories;
} // categoryIds

QString VocabularyDatabase::dataText(quint8 categoryId, quint32 row, quint8 fieldId) const
{
    const quint32 dataRecordId = recordId(categoryId, row);
	return dataText(dataRecordId, fieldId);
} // dataText

QString VocabularyDatabase::dataText(quint32 recordId, quint8 fieldId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_TEXT + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(fieldId));
    if (query.next())
	{
	    return query.value(ColumnPosition1).toString();
    }
	else
	{
        return QString();
    } // if else
} // dataText

VocabularyDatabase::RecordDataHash *VocabularyDatabase::dataText() const
{
    RecordDataHash *recordData = new RecordDataHash();

    QSqlQuery query = _database.exec("SELECT " + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + " FROM " + TABLE_DATA);

	// set progress maximum
	if (query.last())
	{
		emit setOpenProgressMax(query.at() + 1);
	} // if

	if (query.first())
	{
		quint8 progress = 0;
		do
		{
			DataHash *data = &recordData->operator[](query.value(ColumnPosition2).toUInt());
			data->insert(query.value(ColumnPosition1).toUInt(), query.value(ColumnPosition3).toString());

            if (progress % OPENPROGRESS_REFRESHINTERVAL == 0)
			{
			    emit setOpenProgressValue(progress);
            } // if
            progress++;
		}
		while (query.next());
	} // if

    return recordData;
} // dataText

VocabularyDatabase::FieldAttributes VocabularyDatabase::fieldAttributes(quint8 fieldId) const
{
    QSqlQuery query = _database.exec("SELECT " + COLUMN_ATTRIBUTES + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
    if (query.next())
	{
        return query.value(ColumnPosition1).toUInt();
    } // if

    return FieldAttributeNone;
} // fieldAttributes

#ifndef EDITION_FREE
VocabularyDatabase::FieldBuiltIn VocabularyDatabase::fieldBuiltIn(quint8 fieldId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_BUILTIN + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (query.next())
	{
		return static_cast<FieldBuiltIn>(query.value(ColumnPosition1).toUInt());
	} // if

	return FieldBuiltInNone;
} // fieldBuiltIn

quint8 VocabularyDatabase::fieldCount() const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	if (query.last())
	{
		return query.at() + 1;
	}
	else
	{
		return 0;
	} // if else
} // fieldCount
#endif

quint8 VocabularyDatabase::fieldId(quint8 position) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	quint8 currentPosition = 0;
	while (query.next())
	{
		if (currentPosition == position)
		{
			return query.value(ColumnPosition1).toUInt();
		}
		else
		{
			currentPosition++;
		} // if else
	} // while

	return NOT_FOUND;
} // getFieldId

VocabularyDatabase::FieldIdList VocabularyDatabase::fieldIds() const
{
	FieldIdList fieldIdList;

	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
	while (query.next())
	{
		fieldIdList.append(query.value(ColumnPosition1).toUInt());
	} // while

	return fieldIdList;
} // fieldIds

VocabularyDatabase::FieldLanguage VocabularyDatabase::fieldLanguage(quint8 fieldId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_LANGUAGE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (query.next())
	{
		return static_cast<FieldLanguage>(query.value(ColumnPosition1).toUInt());
	} // if

	return FieldLanguageUnknown;
} // fieldLanguage

QString VocabularyDatabase::fieldName(quint8 fieldId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (query.next())
	{
		return query.value(ColumnPosition1).toString();
	} // while

	return QString();
} // fieldName

QString VocabularyDatabase::fieldTemplateName(quint8 fieldId) const
{
    QSqlQuery query = _database.exec("SELECT " + COLUMN_TEMPLATENAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
    if (query.next())
	{
        return query.value(ColumnPosition1).toString();
    } // while

    return QString();
} // fieldTemplateName

VocabularyDatabase::FieldType VocabularyDatabase::fieldType(quint8 fieldId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_TYPE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
	if (query.next())
	{
		return static_cast<FieldType>(query.value(ColumnPosition1).toUInt());
	} // if

	return FieldTypeUnknown;
} // fieldType

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

	if (QFile::exists(filePath))
	{
		QFile::remove(filePath);
	} // if
#endif

	openDatabase();
	initialize();
} // new

quint8 VocabularyDatabase::recordCategory(quint32 recordId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
	query.next();
	return query.value(ColumnPosition1).toUInt();
} // recordCategory

quint32 VocabularyDatabase::recordCount() const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	if (query.last())
	{
		return query.at() + 1;
	}
	else
	{
		return 0;
	} // if else
} // recordCount

quint32 VocabularyDatabase::recordCount(quint8 categoryId) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	if (query.last())
	{
		return query.at() + 1;
	}
	else
	{
		return 0;
	} // if else
} // recordCount

#ifndef EDITION_FREE
quint32 VocabularyDatabase::recordCount(quint8 categoryId, bool enabled) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId) + " AND " + COLUMN_ENABLED + " = " + QString::number(enabled));
    if (query.last())
	{
        return query.at() + 1;
    }
	else
	{
        return 0;
    } // if else
} // recordCount
#endif

quint32 VocabularyDatabase::recordCount(bool enabled) const
{
    QSqlQuery query = _database.exec("SELECT " + TABLE_RECORDS + '.' + COLUMN_ID + " FROM " + TABLE_RECORDS + " JOIN " + TABLE_CATEGORIES + " ON " + TABLE_RECORDS + '.' + COLUMN_CATEGORYID + " = " + TABLE_CATEGORIES + '.' + COLUMN_ID + " WHERE " + TABLE_CATEGORIES + '.' + COLUMN_ENABLED + " = " + QString::number(enabled));
    if (query.last())
	{
        return query.at() + 1;
    }
	else
	{
        return 0;
    } // if else
} // recordCount

quint32 VocabularyDatabase::recordId(quint32 row) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
	query.seek(row);
	return query.value(ColumnPosition1).toUInt();
} // recordId

quint32 VocabularyDatabase::recordId(quint8 categoryId, quint32 row) const
{
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	query.seek(row);
	return query.value(ColumnPosition1).toUInt();
} // recordId

VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds(quint8 categoryId) const
{
    RecordIdList recordIdList;
    QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
    while (query.next())
	{
        recordIdList.append(query.value(ColumnPosition1).toUInt());
    } // while

    return recordIdList;
} // recordIds

#ifndef EDITION_TRY
void VocabularyDatabase::open(const QString &filePath)
{
    if (!QFile::exists(filePath))
	{
        return;
    } // if

    _vocabularyFile = filePath;
	emit setVocabularyName(name());

    openDatabase();

    updateDatabase();
} // open
#endif

void VocabularyDatabase::removeCategory(quint8 categoryId) const
{
	// record
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	while (query.next())
	{
		quint32 record = query.value(ColumnPosition1).toUInt();
		removeRecord(record);
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
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	query.seek(row);
	quint32 record = query.value(ColumnPosition1).toUInt();

	removeRecord(record);
} // removeRecord

void VocabularyDatabase::setDataText(quint8 categoryId, quint32 row, quint8 fieldId, const QString &data) const
{
	// find data record
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
	query.seek(row);
	quint32 recordId = query.value(ColumnPosition1).toUInt();

    setDataText(recordId, fieldId, data);
} // setDataText

void VocabularyDatabase::setDataText(quint32 recordId, quint8 fieldId, const QString &data) const
{
    QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(fieldId));
    if (query.next())
	{
        quint32 dataId = query.value(ColumnPosition1).toUInt();
        _database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_TEXT + " = '" + data + "' WHERE " + COLUMN_ID + " = " + QString::number(dataId));
    }
	else
	{
        _database.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(fieldId) + "', '" + QString::number(recordId) + "', '" + data + "')");
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

void VocabularyDatabase::setRecordByRowCategory(quint8 oldCategoryId, quint32 recordRow, quint8 newCategoryId) const
{
	quint32 record = recordId(oldCategoryId, recordRow);
	SetRecordCategory(record, newCategoryId);
} // setRecordByRowCategory

void VocabularyDatabase::SetRecordCategory(quint32 recordId, quint8 categoryId) const
{
	_database.exec("UPDATE " + TABLE_RECORDS + " SET " + COLUMN_CATEGORYID + " = " + QString::number(categoryId) + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
} // SetRecordCategory

void VocabularyDatabase::swapFields(quint8 sourceId, quint8 destinationId) const
{
	QSqlQuery source      = _database.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(sourceId));
	QSqlQuery destination = _database.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(destinationId));

	source.next();
	destination.next();

	update(TABLE_FIELDS, destinationId, source.record());
	update(TABLE_FIELDS, sourceId, destination.record());

	_database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = 0 WHERE " + COLUMN_FIELDID + " = " + QString::number(sourceId));
	_database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(sourceId) + " WHERE " + COLUMN_FIELDID + " = " + QString::number(destinationId));
	_database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(destinationId) + " WHERE " + COLUMN_FIELDID + " = 0");
} // swapFields

quint8 VocabularyDatabase::addField(const QString &templateText, const QString &name, const FieldType &type, FieldAttributes attributes, FieldBuiltIn builtIn, FieldLanguage language) const
{
    const QSqlQuery query = _database.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " + COLUMN_NAME + ", " + COLUMN_TYPE + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_BUILTIN + ", " + COLUMN_LANGUAGE + ") VALUES ('" + templateText + "', '" + name + "', '" + QString::number(type) + "', '" + QString::number(attributes) + "', '" + QString::number(builtIn) + "', '" + QString::number(language) + "')");
	return query.lastInsertId().toUInt();
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

void VocabularyDatabase::closeDatabase()
{
	if (_database.isOpen())
	{
		_database.close();
	} // if
} // closeDatabase

#ifndef EDITION_TRY
VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds() const
{
    RecordIdList recordIdList;
    QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
    while (query.next())
	{
        recordIdList.append(query.value(ColumnPosition1).toUInt());
    } // while

    return recordIdList;
} // recordIds
#endif

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

void VocabularyDatabase::openDatabase()
{
#ifdef EDITION_TRY
    _database.setDatabaseName(":memory:");
#else
	_database.setDatabaseName(_vocabularyFile);
#endif
	_database.open();
} // openDatabase

void VocabularyDatabase::removeRecord(quint32 recordId) const
{
	// remove record's data
	QSqlQuery query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId));
	while (query.next())
	{
		quint32 dataId = query.value(ColumnPosition1).toUInt();
		_database.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_ID + " = " + QString::number(dataId));
	} // while

	_database.exec("DELETE FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
} // removeRecord

void VocabularyDatabase::update(const QString &table, quint8 columnId, const QSqlRecord &record) const
{
	QString query = "UPDATE " + table + " SET ";

	quint8 setIndex = 0;
	for (quint8 column = 0; column < record.count(); column++)
	{
		const QSqlField field = record.field(column);
		if (field.name() != COLUMN_ID)
		{
			if (setIndex > 0)
			{
				query += ", ";
			} // if

			query += field.name() + " = '" + field.value().toString() + "'";
			setIndex++;
		} // if
	} // for

	query += " WHERE " + COLUMN_ID + " = " + QString::number(columnId);

	_database.exec(query);
} // update
#endif

#ifndef EDITION_TRY
void VocabularyDatabase::updateDatabase()
{
    const QString version = settings(KEY_VERSION);
    Version current;
    if (version.isEmpty())
	{
        current = Version1;
    }
	else
	{
        current = static_cast<Version>(version.toUInt());
    } // if else

	beginEdit();

    if (current < Version2)
	{
        // add priority column to categories table
        _database.exec("ALTER TABLE " + TABLE_CATEGORIES + " ADD " + COLUMN_PRIORITY + " INTEGER");
        _database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(PRIORITY_DEFAULT));

        // add show attribute to fields
        FieldIdList fieldIdList = fieldIds();
        foreach (quint8 fieldId, fieldIdList)
		{
            FieldAttributes attributes = fieldAttributes(fieldId);
            attributes                |= FieldAttributeShow;
            setFieldAttributes(fieldId, attributes);
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
            , settings(KEY_TRAYTEMPLATE1), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH1).toUInt()), settings(KEY_VOICE1)
#endif
            );
        addLanguage(settings(KEY_LANGUAGE2), settings(KEY_LEARNINGTEMPLATE2)
#ifndef EDITION_FREE
            , settings(KEY_TRAYTEMPLATE2), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH2).toUInt()), settings(KEY_VOICE2)
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