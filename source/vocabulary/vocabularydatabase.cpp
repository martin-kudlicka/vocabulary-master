#include "pch.h"
#include "vocabulary/vocabularydatabase.h"

const auto OPENPROGRESS_REFRESHINTERVAL = 100;
const auto PRIORITY_DEFAULT             = 1;

const auto    KEY_LANGUAGE1           = "language1";
const auto    KEY_LANGUAGE2           = "language2";
const auto    KEY_LEARNINGTEMPLATE1   = "learningtemplate1";
const auto    KEY_LEARNINGTEMPLATE2   = "learningtemplate2";
const auto    KEY_SPEECH1             = "speech1";
const auto    KEY_SPEECH2             = "speech2";
const auto    KEY_TRAYTEMPLATE1       = "traytemplate1";
const auto    KEY_TRAYTEMPLATE2       = "traytemplate2";
const auto    KEY_VOICE1              = "voice1";
const auto    KEY_VOICE2              = "voice2";
const auto    KEY_VERSION             = "version";
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
const QString TABLE_CATEGORIES        = "categories";
const QString TABLE_DATA              = "data";
const QString TABLE_FIELDS            = "fields";
const QString TABLE_LANGUAGES         = "languages";
const QString TABLE_RECORDS           = "records";
const QString TABLE_SETTINGS          = "settings";

VocabularyDatabase::VocabularyDatabase() : _database(QSqlDatabase::addDatabase("QSQLITE", QString::number(qrand())))
{
}

VocabularyDatabase::~VocabularyDatabase()
{
  closeDatabase();
}

quintptr VocabularyDatabase::categoryCount() const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
  if (query.last())
  {
    return query.at() + 1;
  }

  return 0;
}

bool VocabularyDatabase::categoryEnabled(quintptr categoryId) const
{
  auto query = _database.exec("SELECT " + COLUMN_ENABLED + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
  query.next();
  return query.value(static_cast<int>(ColumnPosition::N1)).toBool();
}

quintptr VocabularyDatabase::categoryId(quintptr row) const
{
  auto query             = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
  decltype(row) position = 0;
  while (query.next())
  {
    if (position == row)
    {
      return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
    }

    ++position;
  }

  return NOT_FOUND;
}

QString VocabularyDatabase::categoryName(quintptr categoryId) const
{
  auto query = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
  query.next();
  return query.value(static_cast<int>(ColumnPosition::N1)).toString();
}

quintptr VocabularyDatabase::categoryPriority(quintptr categoryId) const
{
  auto query = _database.exec("SELECT " + COLUMN_PRIORITY + " FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
  query.next();
  return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
}

bool VocabularyDatabase::isOpen() const
{
  return _database.isOpen();
}

VocabularyDatabase::LanguageIdList VocabularyDatabase::languageIds(LanguageIds type) const
{
  LanguageIdList languageIdList;

  if (type & LanguageId::UserDefined)
  {
    auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_LANGUAGES);
    while (query.next())
    {
      languageIdList.append(static_cast<FieldLanguage>(query.value(static_cast<int>(ColumnPosition::N1)).toUInt()));
    }
  }

  if (type & LanguageId::AllOnly)
  {
    languageIdList.append(FieldLanguage::All);
  }

  return languageIdList;
}

QString VocabularyDatabase::languageLearningTemplate(FieldLanguage languageId) const
{
  auto query = _database.exec("SELECT " + COLUMN_LEARNINGTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return QString();
}

QString VocabularyDatabase::languageName(FieldLanguage languageId) const
{
  auto query = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return tr("All");
}

TTSInterface::TTSPlugin VocabularyDatabase::languageSpeech(FieldLanguage languageId) const
{
  auto query = _database.exec("SELECT " + COLUMN_SPEECH + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
  if (query.next())
  {
    return static_cast<const TTSInterface::TTSPlugin>(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return TTSInterface::TTSPlugin::None;
}

QString VocabularyDatabase::languageTrayTemplate(FieldLanguage languageId) const
{
  auto query = _database.exec("SELECT " + COLUMN_TRAYTEMPLATE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return QString();
}

QString VocabularyDatabase::languageVoice(FieldLanguage languageId) const
{
  auto query = _database.exec("SELECT " + COLUMN_VOICE + " FROM " + TABLE_LANGUAGES + " WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return static_cast<quintptr>(TTSInterface::TTSPlugin::None);
}

QString VocabularyDatabase::name() const
{
  return QFileInfo(_vocabularyFile).completeBaseName();
}

quintptr VocabularyDatabase::row(quintptr recordId, quintptr categoryId) const
{
  auto row = 0;

  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  while (query.next())
  {
    if (query.value(static_cast<int>(ColumnPosition::N1)).toUInt() == recordId)
    {
      return row;
    }

    ++row;
  }

  return NOT_FOUND;
}

quintptr VocabularyDatabase::search(const QString &word, quintptr startId) const
{
  const auto wordLike = '%' + word + '%';
  auto query          = _database.exec("SELECT " + COLUMN_RECORDID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_TEXT + " LIKE '" + wordLike + "' GROUP BY " + COLUMN_RECORDID);
  if (!query.next())
  {
    return NOT_FOUND;
  }

  do
  {
    if (query.value(static_cast<int>(ColumnPosition::N1)).toUInt() >= startId)
    {
      return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
    }
  }
  while (query.next());

  query.seek(0);
  return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
}

void VocabularyDatabase::setCategoryEnabled(quintptr categoryId, bool enabled) const
{
  _database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_ENABLED + " = " + QString::number(enabled) + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
}

void VocabularyDatabase::setCategoryPriority(quintptr categoryId, quintptr priority) const
{
  _database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(priority) + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
}

void VocabularyDatabase::setLanguageLearningTemplate(FieldLanguage languageId, const QString &templateText) const
{
  _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_LEARNINGTEMPLATE + " = '" + templateText + "' WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
}

void VocabularyDatabase::setLanguageName(FieldLanguage languageId, const QString &name) const
{
  _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_NAME + " = '" + name + "' WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
}

void VocabularyDatabase::setLanguageSpeech(FieldLanguage languageId, TTSInterface::TTSPlugin speech) const
{
  _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_SPEECH + " = '" + QString::number(static_cast<quintptr>(speech)) + "' WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
}

void VocabularyDatabase::setLanguageTrayTemplate(FieldLanguage languageId, const QString &templateText) const
{
  _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_TRAYTEMPLATE + " = '" + templateText + "' WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
}

void VocabularyDatabase::setLanguageVoice(FieldLanguage languageId, const QString &voice) const
{
  _database.exec("UPDATE " + TABLE_LANGUAGES + " SET " + COLUMN_VOICE + " = '" + voice + "' WHERE " + COLUMN_ID + " = " + QString::number(static_cast<quintptr>(languageId)));
}

void VocabularyDatabase::setSettings(const QString &key, const QString &value) const
{
  if (settings(key).isNull())
  {
    _database.exec("INSERT INTO " + TABLE_SETTINGS + " (" + COLUMN_KEY + ", " + COLUMN_VALUE + ") VALUES ('" + key + "', '" + value + "')");
  }
  else
  {
    _database.exec("UPDATE " + TABLE_SETTINGS + " SET " + COLUMN_VALUE + " = '" + value + "' WHERE " + COLUMN_KEY + " = '" + key + "'");
  }
}

QString VocabularyDatabase::settings(const QString &key) const
{
  auto query = _database.exec("SELECT " + COLUMN_VALUE + " FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + key + "'");
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }
  else
  {
    return QString();
  }
}

const QString &VocabularyDatabase::vocabularyFile() const
{
  return _vocabularyFile;
}

quintptr VocabularyDatabase::addCategory(const QString &name) const
{
  const auto query = _database.exec("INSERT INTO " + TABLE_CATEGORIES + " (" + COLUMN_NAME + ", " + COLUMN_PRIORITY + ", " + COLUMN_ENABLED + ") VALUES ('" + name + "', '" + QString::number(PRIORITY_DEFAULT) + "', '" + QString::number(true) + "')");
  return query.lastInsertId().toUInt();
}

quintptr VocabularyDatabase::addField() const
{
  const auto num          = fieldCount() + 1;
  const auto templateText = tr("Field") + QString::number(num);
  const auto name         = tr("Name")  + QString::number(num);

  return addField(templateText, name, FieldType::LineEdit, FieldAttribute::Show, FieldBuiltIn::None, FieldLanguage::Left);
}

quintptr VocabularyDatabase::addRecord(quintptr categoryId) const
{
  // create new record
  const auto query    = _database.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(categoryId) + "')");
  const auto recordId = query.lastInsertId().toUInt();

  return recordId;
}

quintptr VocabularyDatabase::addRecord(quintptr categoryId, const QStringList &data) const
{
  // create new record
  auto query          = _database.exec("INSERT INTO " + TABLE_RECORDS + " (" + COLUMN_CATEGORYID + ") VALUES ('" + QString::number(categoryId) + "')");
  const auto recordId = query.lastInsertId().toUInt();

  // create new data
  auto dataIndex = 0;
  for (auto fieldId : fieldIds())
  {
    _database.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(fieldId) + "', '" + QString::number(recordId) + "', '" + data.at(dataIndex) + "')");
    ++dataIndex;
  }

  return recordId;
}

void VocabularyDatabase::beginEdit()
{
  _database.transaction();
}

void VocabularyDatabase::close()
{
  closeDatabase();
}

void VocabularyDatabase::endEdit(bool save /* true */)
{
  if (save)
  {
    _database.commit();
  }
  else
  {
    _database.rollback();
  }
}

VocabularyDatabase::CategoryIdList VocabularyDatabase::categoryIds() const
{
  CategoryIdList categories;

  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_CATEGORIES);
  while (query.next())
  {
    categories.append(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return categories;
}

QString VocabularyDatabase::dataText(quintptr categoryId, quintptr row, quintptr fieldId) const
{
  const auto dataRecordId = recordId(categoryId, row);
  return dataText(dataRecordId, fieldId);
}

QString VocabularyDatabase::dataText(quintptr recordId, quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_TEXT + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return QString();
}

VocabularyDatabase::RecordDataHash *VocabularyDatabase::dataText() const
{
  auto *recordData = new RecordDataHash();

  auto query = _database.exec("SELECT " + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + " FROM " + TABLE_DATA);

  // set progress maximum
  if (query.last())
  {
    emit setOpenProgressMax(query.at() + 1);
  }

  if (query.first())
  {
    auto progress = 0;
    do
    {
      auto *data = &recordData->operator[](query.value(static_cast<int>(ColumnPosition::N2)).toUInt());
      data->insert(query.value(static_cast<int>(ColumnPosition::N1)).toUInt(), query.value(static_cast<int>(ColumnPosition::N3)).toString());

      if (progress % OPENPROGRESS_REFRESHINTERVAL == 0)
      {
        emit setOpenProgressValue(progress);
      }
      ++progress;
    }
    while (query.next());
  }

  return recordData;
}

VocabularyDatabase::FieldAttributes VocabularyDatabase::fieldAttributes(quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_ATTRIBUTES + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
  }

  return FieldAttribute::None;
}

VocabularyDatabase::FieldBuiltIn VocabularyDatabase::fieldBuiltIn(quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_BUILTIN + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return static_cast<FieldBuiltIn>(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return FieldBuiltIn::None;
}

quintptr VocabularyDatabase::fieldCount() const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
  if (query.last())
  {
    return query.at() + 1;
  }

  return 0;
}

quintptr VocabularyDatabase::fieldId(quintptr position) const
{
  auto query                         = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
  decltype(position) currentPosition = 0;
  while (query.next())
  {
    if (currentPosition == position)
    {
      return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
    }

    ++currentPosition;
  }

  return NOT_FOUND;
}

VocabularyDatabase::FieldIdList VocabularyDatabase::fieldIds() const
{
  FieldIdList fieldIdList;

  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_FIELDS);
  while (query.next())
  {
    fieldIdList.append(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return fieldIdList;
}

VocabularyDatabase::FieldLanguage VocabularyDatabase::fieldLanguage(quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_LANGUAGE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return static_cast<FieldLanguage>(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return FieldLanguage::Unknown;
}

QString VocabularyDatabase::fieldName(quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_NAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return QString();
}

QString VocabularyDatabase::fieldTemplateName(quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_TEMPLATENAME + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return query.value(static_cast<int>(ColumnPosition::N1)).toString();
  }

  return QString();
}

VocabularyDatabase::FieldType VocabularyDatabase::fieldType(quintptr fieldId) const
{
  auto query = _database.exec("SELECT " + COLUMN_TYPE + " FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
  if (query.next())
  {
    return static_cast<FieldType>(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return FieldType::Unknown;
}

void VocabularyDatabase::new2(const QString &filePath)
{
  _vocabularyFile = filePath;

  if (QFile::exists(filePath))
  {
    QFile::remove(filePath);
  }

  openDatabase();
  initialize();
}

quintptr VocabularyDatabase::recordCategory(quintptr recordId) const
{
  auto query = _database.exec("SELECT " + COLUMN_CATEGORYID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
  query.next();
  return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
}

quintptr VocabularyDatabase::recordCount() const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
  if (query.last())
  {
    return query.at() + 1;
  }

  return 0;
}

quintptr VocabularyDatabase::recordCount(quintptr categoryId) const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  if (query.last())
  {
    return query.at() + 1;
  }

  return 0;
}

quintptr VocabularyDatabase::recordCount(quintptr categoryId, bool enabled) const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId) + " AND " + COLUMN_ENABLED + " = " + QString::number(enabled));
  if (query.last())
  {
    return query.at() + 1;
  }

  return 0;
}

quintptr VocabularyDatabase::recordCount(bool enabled) const
{
  auto query = _database.exec("SELECT " + TABLE_RECORDS + '.' + COLUMN_ID + " FROM " + TABLE_RECORDS + " JOIN " + TABLE_CATEGORIES + " ON " + TABLE_RECORDS + '.' + COLUMN_CATEGORYID + " = " + TABLE_CATEGORIES + '.' + COLUMN_ID + " WHERE " + TABLE_CATEGORIES + '.' + COLUMN_ENABLED + " = " + QString::number(enabled));
  if (query.last())
  {
    return query.at() + 1;
  }

  return 0;
}

quintptr VocabularyDatabase::recordId(quintptr row) const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
  query.seek(row);
  return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
}

quintptr VocabularyDatabase::recordId(quintptr categoryId, quintptr row) const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  query.seek(row);
  return query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
}

VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds(quintptr categoryId) const
{
  RecordIdList recordIdList;
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  while (query.next())
  {
    recordIdList.append(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return recordIdList;
}

void VocabularyDatabase::open(const QString &filePath)
{
  if (!QFile::exists(filePath))
  {
    return;
  }

  _vocabularyFile = filePath;
  emit setVocabularyName(name());

  openDatabase();

  updateDatabase();
}

void VocabularyDatabase::removeCategory(quintptr categoryId) const
{
  // record
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  while (query.next())
  {
    auto record = query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
    removeRecord(record);
  }

  _database.exec("DELETE FROM " + TABLE_CATEGORIES + " WHERE " + COLUMN_ID + " = " + QString::number(categoryId));
}

void VocabularyDatabase::removeField(quintptr fieldId) const
{
  _database.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_FIELDID + " = " + QString::number(fieldId));
  _database.exec("DELETE FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
}

void VocabularyDatabase::removeRecord(quintptr categoryId, quintptr row) const
{
  // find record
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  query.seek(row);
  auto record = query.value(static_cast<int>(ColumnPosition::N1)).toUInt();

  removeRecord(record);
}

void VocabularyDatabase::setDataText(quintptr categoryId, quintptr row, quintptr fieldId, const QString &data) const
{
  // find data record
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS + " WHERE " + COLUMN_CATEGORYID + " = " + QString::number(categoryId));
  query.seek(row);
  auto recordId = query.value(static_cast<int>(ColumnPosition::N1)).toUInt();

  setDataText(recordId, fieldId, data);
}

void VocabularyDatabase::setDataText(quintptr recordId, quintptr fieldId, const QString &data) const
{
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId) + " AND " + COLUMN_FIELDID + " = " + QString::number(fieldId));
  if (query.next())
  {
    auto dataId = query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
    _database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_TEXT + " = '" + data + "' WHERE " + COLUMN_ID + " = " + QString::number(dataId));
  }
  else
  {
    _database.exec("INSERT INTO " + TABLE_DATA + " (" + COLUMN_FIELDID + ", " + COLUMN_RECORDID + ", " + COLUMN_TEXT + ") VALUES ('" + QString::number(fieldId) + "', '" + QString::number(recordId) + "', '" + data + "')");
  }
}

void VocabularyDatabase::setFieldAttributes(quintptr fieldId, FieldAttributes attributes) const
{
  _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_ATTRIBUTES + " = '" + QString::number(attributes) + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
}

void VocabularyDatabase::setFieldLanguage(quintptr fieldId, FieldLanguage language) const
{
  _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_LANGUAGE + " = '" + QString::number(static_cast<quintptr>(language)) + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
}

void VocabularyDatabase::setFieldName(quintptr fieldId, const QString &name) const
{
  _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_NAME + " = '" + name + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
}

void VocabularyDatabase::setFieldTemplateName(quintptr fieldId, const QString &templateName) const
{
  _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TEMPLATENAME + " = '" + templateName + "' WHERE " + COLUMN_ID + " = " + QString::number(fieldId));
}

void VocabularyDatabase::setRecordByRowCategory(quintptr oldCategoryId, quintptr recordRow, quintptr newCategoryId) const
{
  auto record = recordId(oldCategoryId, recordRow);
  SetRecordCategory(record, newCategoryId);
}

void VocabularyDatabase::SetRecordCategory(quintptr recordId, quintptr categoryId) const
{
  _database.exec("UPDATE " + TABLE_RECORDS + " SET " + COLUMN_CATEGORYID + " = " + QString::number(categoryId) + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
}

void VocabularyDatabase::swapFields(quintptr sourceId, quintptr destinationId) const
{
  auto source      = _database.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(sourceId));
  auto destination = _database.exec("SELECT * FROM " + TABLE_FIELDS + " WHERE " + COLUMN_ID + " = " + QString::number(destinationId));

  source.next();
  destination.next();

  update(TABLE_FIELDS, destinationId, source.record());
  update(TABLE_FIELDS, sourceId, destination.record());

  _database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = 0 WHERE " + COLUMN_FIELDID + " = " + QString::number(sourceId));
  _database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(sourceId) + " WHERE " + COLUMN_FIELDID + " = " + QString::number(destinationId));
  _database.exec("UPDATE " + TABLE_DATA + " SET " + COLUMN_FIELDID + " = " + QString::number(destinationId) + " WHERE " + COLUMN_FIELDID + " = 0");
}

quintptr VocabularyDatabase::addField(const QString &templateText, const QString &name, const FieldType &type, FieldAttributes attributes, FieldBuiltIn builtIn, FieldLanguage language) const
{
  const auto query = _database.exec("INSERT INTO " + TABLE_FIELDS + " (" + COLUMN_TEMPLATENAME + ", " + COLUMN_NAME + ", " + COLUMN_TYPE + ", " + COLUMN_ATTRIBUTES + ", " + COLUMN_BUILTIN + ", " + COLUMN_LANGUAGE + ") VALUES ('" + templateText + "', '" + name + "', '" + QString::number(static_cast<quintptr>(type)) + "', '" + QString::number(attributes) + "', '" + QString::number(static_cast<quintptr>(builtIn)) + "', '" + QString::number(static_cast<quintptr>(language)) + "')");
  return query.lastInsertId().toUInt();
}

void VocabularyDatabase::addLanguage(const QString &name, const QString &learningTemplate, const QString &trayTemplate, TTSInterface::TTSPlugin ttsPlugin, const QString &voice) const
{
  _database.exec("INSERT INTO " + TABLE_LANGUAGES + " (" + COLUMN_NAME + ", " + COLUMN_LEARNINGTEMPLATE + ", " + COLUMN_TRAYTEMPLATE + ", " + COLUMN_SPEECH + ", " + COLUMN_VOICE + ") VALUES ('" + name + "', '" + learningTemplate + "', '" + trayTemplate + "', '" + QString::number(static_cast<quintptr>(ttsPlugin)) + "', '" + voice + "')");
}

void VocabularyDatabase::closeDatabase()
{
  if (_database.isOpen())
  {
    _database.close();
  }
}

VocabularyDatabase::RecordIdList VocabularyDatabase::recordIds() const
{
  RecordIdList recordIdList;
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_RECORDS);
  while (query.next())
  {
    recordIdList.append(query.value(static_cast<int>(ColumnPosition::N1)).toUInt());
  }

  return recordIdList;
}

void VocabularyDatabase::initialize() const
{
  _database.exec("CREATE TABLE " + TABLE_SETTINGS + " ("
                 + COLUMN_KEY   + " TEXT NOT NULL,"
                 + COLUMN_VALUE + " TEXT NOT NULL)");
  _database.exec("CREATE TABLE " + TABLE_CATEGORIES + " ("
                 + COLUMN_ID       + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                 + COLUMN_NAME     + " TEXT NOT NULL,"
                 + COLUMN_PRIORITY + " INTEGER,"
                 + COLUMN_ENABLED  + " INTEGER NOT NULL)");
  _database.exec("CREATE TABLE " + TABLE_LANGUAGES + " ("
                 + COLUMN_ID               + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                 + COLUMN_NAME             + " TEXT NOT NULL,"
                 + COLUMN_LEARNINGTEMPLATE + " TEXT NOT NULL,"
                 + COLUMN_TRAYTEMPLATE     + " TEXT NOT NULL,"
                 + COLUMN_SPEECH           + " INTEGER NOT NULL,"
                 + COLUMN_VOICE            + " TEXT NOT NULL)");
  _database.exec("CREATE TABLE " + TABLE_FIELDS + " ("
                 + COLUMN_ID           + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                 + COLUMN_TEMPLATENAME + " TEXT NOT NULL,"
                 + COLUMN_NAME         + " TEXT NOT NULL,"
                 + COLUMN_TYPE         + " INTEGER NOT NULL,"
                 + COLUMN_ATTRIBUTES   + " INTEGER NOT NULL,"
                 + COLUMN_BUILTIN      + " INTEGER NOT NULL,"
                 + COLUMN_LANGUAGE     + " INTEGER REFERENCES " + TABLE_LANGUAGES + " ON DELETE CASCADE)");
  _database.exec("CREATE TABLE " + TABLE_RECORDS + " ("
                 + COLUMN_ID         + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                 + COLUMN_CATEGORYID + " INTEGER REFERENCES " + TABLE_CATEGORIES + " ON DELETE CASCADE)");
  _database.exec("CREATE TABLE " + TABLE_DATA + " ("
                 + COLUMN_ID       + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                 + COLUMN_FIELDID  + " INTEGER REFERENCES " + TABLE_FIELDS + " ON DELETE CASCADE,"
                 + COLUMN_RECORDID + " INTEGER REFERENCES " + TABLE_RECORDS + " ON DELETE CASCADE,"
                 + COLUMN_TEXT     + " TEXT NOT NULL)");

  // fill default data
  addLanguage(tr("Language1"), QString(), QString(), TTSInterface::TTSPlugin::None, QString());
  addLanguage(tr("Language2"), QString(), QString(), TTSInterface::TTSPlugin::None, QString());
  addField(tr("Enabled"),  "", FieldType::CheckBox, FieldAttribute::Show | FieldAttribute::BuiltIn, FieldBuiltIn::Enabled,  FieldLanguage::All);
  addField(tr("Priority"), "", FieldType::SpinBox,  FieldAttribute::Show | FieldAttribute::BuiltIn, FieldBuiltIn::Priority, FieldLanguage::All);

  setSettings(KEY_VERSION, QString::number(static_cast<quintptr>(Version::N2)));
}

void VocabularyDatabase::openDatabase()
{
  _database.setDatabaseName(_vocabularyFile);
  _database.open();
}

void VocabularyDatabase::removeRecord(quintptr recordId) const
{
  // remove record's data
  auto query = _database.exec("SELECT " + COLUMN_ID + " FROM " + TABLE_DATA + " WHERE " + COLUMN_RECORDID + " = " + QString::number(recordId));
  while (query.next())
  {
    auto dataId = query.value(static_cast<int>(ColumnPosition::N1)).toUInt();
    _database.exec("DELETE FROM " + TABLE_DATA + " WHERE " + COLUMN_ID + " = " + QString::number(dataId));
  }

  _database.exec("DELETE FROM " + TABLE_RECORDS + " WHERE " + COLUMN_ID + " = " + QString::number(recordId));
}

void VocabularyDatabase::update(const QString &table, quintptr columnId, const QSqlRecord &record) const
{
  auto query = "UPDATE " + table + " SET ";

  auto setIndex = 0;
  for (auto column = 0; column < record.count(); ++column)
  {
    const auto field = record.field(column);
    if (field.name() != COLUMN_ID)
    {
      if (setIndex > 0)
      {
        query += ", ";
      }

      query += field.name() + " = '" + field.value().toString() + "'";
      ++setIndex;
    }
  }

  query += " WHERE " + COLUMN_ID + " = " + QString::number(columnId);

  _database.exec(query);
}

void VocabularyDatabase::updateDatabase()
{
  const auto version = settings(KEY_VERSION);
  Version current;
  if (version.isEmpty())
  {
    current = Version::N1;
  }
  else
  {
    current = static_cast<Version>(version.toUInt());
  }

  beginEdit();

  if (current < Version::N2)
  {
    // add priority column to categories table
    _database.exec("ALTER TABLE " + TABLE_CATEGORIES + " ADD " + COLUMN_PRIORITY + " INTEGER");
    _database.exec("UPDATE " + TABLE_CATEGORIES + " SET " + COLUMN_PRIORITY + " = " + QString::number(PRIORITY_DEFAULT));

    // add show attribute to fields
    auto fieldIdList = fieldIds();
    for (auto fieldId : fieldIdList)
    {
      auto attributes = fieldAttributes(fieldId);
      attributes     |= FieldAttribute::Show;
      setFieldAttributes(fieldId, attributes);
    }

    // create language table
    _database.exec("CREATE TABLE " + TABLE_LANGUAGES + " ("
                   + COLUMN_ID               + " INTEGER PRIMARY KEY AUTOINCREMENT,"
                   + COLUMN_NAME             + " TEXT NOT NULL,"
                   + COLUMN_LEARNINGTEMPLATE + " TEXT NOT NULL,"
                   + COLUMN_TRAYTEMPLATE     + " TEXT NOT NULL,"
                   + COLUMN_SPEECH           + " INTEGER NOT NULL,"
                   + COLUMN_VOICE            + " TEXT NOT NULL)");
    // copy existing language settings values to language table
    addLanguage(settings(KEY_LANGUAGE1), settings(KEY_LEARNINGTEMPLATE1), settings(KEY_TRAYTEMPLATE1), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH1).toUInt()), settings(KEY_VOICE1));
    addLanguage(settings(KEY_LANGUAGE2), settings(KEY_LEARNINGTEMPLATE2), settings(KEY_TRAYTEMPLATE2), static_cast<TTSInterface::TTSPlugin>(settings(KEY_SPEECH2).toUInt()), settings(KEY_VOICE2));
    // delete old language settings
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE1 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE1 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH1 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE1 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LANGUAGE2 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_LEARNINGTEMPLATE2 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_SPEECH2 + "'");
    _database.exec("DELETE FROM " + TABLE_SETTINGS + " WHERE " + COLUMN_KEY + " = '" + KEY_VOICE2 + "'");

    // add type column to fields table
    _database.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_TYPE + " INTEGER");
    _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_TYPE + " = " + QString::number(static_cast<quintptr>(FieldType::LineEdit)));

    // add builtin column to fields table
    _database.exec("ALTER TABLE " + TABLE_FIELDS + " ADD " + COLUMN_BUILTIN + " INTEGER");
    _database.exec("UPDATE " + TABLE_FIELDS + " SET " + COLUMN_BUILTIN + " = " + QString::number(static_cast<quintptr>(FieldBuiltIn::None)));

    // add enable/disable field
    addField(tr("Enabled"), "", FieldType::CheckBox, FieldAttribute::Show | FieldAttribute::BuiltIn, FieldBuiltIn::Enabled, FieldLanguage::All);

    // add priority field
    addField(tr("Priority"), "", FieldType::SpinBox, FieldAttribute::Show | FieldAttribute::BuiltIn, FieldBuiltIn::Priority, FieldLanguage::All);

    setSettings(KEY_VERSION, QString::number(static_cast<quintptr>(Version::N2)));
  }

  endEdit(true);
}