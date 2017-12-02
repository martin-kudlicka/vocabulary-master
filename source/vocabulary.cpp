#include "vocabulary.h"

#include "settings.h"
#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"

Vocabulary::Vocabulary(const Settings *settings) : _cacheEnabled(false), _recordData(Q_NULLPTR), _settings(settings)
{
}

quintptr Vocabulary::addCategory(const QString &name)
{
  const auto categoryId = VocabularyDatabase::addCategory(name);

  if (_cacheEnabled)
  {
    _categoryRecords.insert(categoryId, RecordIdList());
  }

  return categoryId;
}

void Vocabulary::addField()
{
  const auto fieldId = VocabularyDatabase::addField();

  if (_cacheEnabled)
  {
    const auto fieldData2 = fieldData(fieldId);
    _fieldData.insert(fieldId, fieldData2);
  }
}

void Vocabulary::addRecord(quintptr categoryId)
{
  const auto recordId = VocabularyDatabase::addRecord(categoryId);

  if (_cacheEnabled)
  {
    _categoryRecords[categoryId].append(recordId);
  }
}

void Vocabulary::addRecord(quintptr categoryId, const QStringList &data)
{
  const auto recordId = VocabularyDatabase::addRecord(categoryId, data);
  /*_categoryRecords[categoryId].append(recordId);

    // insert data to cache
    auto dataIndex = 0;
    auto fieldData = &_recordData->operator[](recordId);
    for (auto fieldId : fieldIds())
  {
        fieldData->insert(fieldId, data.at(dataIndex));
        ++dataIndex;
    }*/
}

void Vocabulary::beginEdit()
{
  if (_cacheEnabled)
  {
    _fieldDataBackup  = _fieldData;
    _recordDataBackup = *_recordData;
  }

  VocabularyDatabase::beginEdit();
}

void Vocabulary::close()
{
  clearCache();
  VocabularyDatabase::close();
}

void Vocabulary::endEdit(bool save /* true */)
{
  if (!save && _cacheEnabled)
  {
    _fieldData = _fieldDataBackup;
    *_recordData = _recordDataBackup;
  }

  VocabularyDatabase::endEdit(save);
}

bool Vocabulary::fieldHasAttribute(quintptr fieldId, FieldAttribute attribute) const
{
  const auto attributes = fieldAttributes(fieldId);
  return attributes & attribute;
}

VocabularyDatabase::CategoryIdList Vocabulary::categoryIds() const
{
  if (_cacheEnabled)
  {
    return _categoryRecords.keys();
  }
  else
  {
    return VocabularyDatabase::categoryIds();
  }
}

QString Vocabulary::dataText(quintptr categoryId, quintptr row, quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    const auto recordId = _categoryRecords.value(categoryId).at(row);
    return dataText(recordId, fieldId);
  }
  else
  {
    return VocabularyDatabase::dataText(categoryId, row, fieldId);
  }
}

QString Vocabulary::dataText(quintptr recordId, quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _recordData->value(recordId).value(fieldId);
  }
  else
  {
    return VocabularyDatabase::dataText(recordId, fieldId);
  }
}

VocabularyDatabase::FieldAttributes Vocabulary::fieldAttributes(quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _fieldData.value(fieldId).attributes;
  }
  else
  {
    return VocabularyDatabase::fieldAttributes(fieldId);
  }
}

VocabularyDatabase::FieldBuiltIn Vocabulary::fieldBuiltIn(quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _fieldData.value(fieldId).builtIn;
  }
  else
  {
    return VocabularyDatabase::fieldBuiltIn(fieldId);
  }
}

quintptr Vocabulary::fieldCount() const
{
  if (_cacheEnabled)
  {
    return _fieldData.size();
  }
  else
  {
    return VocabularyDatabase::fieldCount();
  }
}

quintptr Vocabulary::fieldId(quintptr position) const
{
  if (_cacheEnabled)
  {
    auto pos = 0;
    for (auto fieldId = _fieldData.constBegin(); fieldId != _fieldData.constEnd(); ++fieldId)
    {
      if (pos == position)
      {
        return fieldId.key();
      }
      else
      {
        ++pos;
      }
    }

    return NOT_FOUND;
  }
  else
  {
    return VocabularyDatabase::fieldId(position);
  }
}

VocabularyDatabase::FieldIdList Vocabulary::fieldIds() const
{
  if (_cacheEnabled)
  {
    return _fieldData.keys();
  }
  else
  {
    return VocabularyDatabase::fieldIds();
  }
}

VocabularyDatabase::FieldLanguage Vocabulary::fieldLanguage(quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _fieldData.value(fieldId).language;
  }
  else
  {
    return VocabularyDatabase::fieldLanguage(fieldId);
  }
}

QString Vocabulary::fieldName(quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _fieldData.value(fieldId).name;
  }
  else
  {
    return VocabularyDatabase::fieldName(fieldId);
  }
}

QString Vocabulary::fieldTemplateName(quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _fieldData.value(fieldId).templateName;
  }
  else
  {
    return VocabularyDatabase::fieldTemplateName(fieldId);
  }
}

VocabularyDatabase::FieldType Vocabulary::fieldType(quintptr fieldId) const
{
  if (_cacheEnabled)
  {
    return _fieldData.value(fieldId).type;
  }
  else
  {
    return VocabularyDatabase::fieldType(fieldId);
  }
}

QStringList Vocabulary::record(quintptr recordId) const
{
  QStringList data;

  const auto fieldIdList = fieldIds();
  for (auto fieldId : fieldIdList)
  {
    data.append(dataText(recordId, fieldId));
  }

  return data;
}

quintptr Vocabulary::recordCategory(quintptr recordId) const
{
  if (_cacheEnabled)
  {
    for (auto category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); ++category)
    {
      if (category->contains(recordId))
      {
        return category.key();
      }
    }

    return NOT_FOUND;
  }
  else
  {
    return VocabularyDatabase::recordCategory(recordId);
  }
}

quintptr Vocabulary::recordCount() const
{
  if (_cacheEnabled)
  {
    auto recordCount = 0;
    const auto allRecordIds = _categoryRecords.values();
    for (const auto &recordIds : allRecordIds)
    {
      recordCount += recordIds.size();
    }

    return recordCount;
  }
  else
  {
    return VocabularyDatabase::recordCount();
  }
}

quintptr Vocabulary::recordCount(quintptr categoryId) const
{
  if (_cacheEnabled)
  {
    return _categoryRecords.value(categoryId).size();
  }
  else
  {
    return VocabularyDatabase::recordCount(categoryId);
  }
}

quintptr Vocabulary::recordCount(quintptr categoryId, bool enabled) const
{
  if (_cacheEnabled)
  {
    auto recordCount = 0;

    const auto recordIds = _categoryRecords.value(categoryId);
    for (auto recordId : recordIds)
    {
      if (recordEnabled(recordId))
      {
        ++recordCount;
      }
    }

    return recordCount;
  }
  else
  {
    return VocabularyDatabase::recordCount(categoryId, enabled);
  }
}

quintptr Vocabulary::recordCount(bool enabled) const
{
  if (_cacheEnabled)
  {
    auto records = 0;

    const auto categoryIdList = categoryIds();
    for (auto categoryId : categoryIdList)
    {
      if (categoryEnabled(categoryId))
      {
        records += recordCount(categoryId, enabled);
      }
    }

    return records;
  }
  else
  {
    return VocabularyDatabase::recordCount(enabled);
  }
}

bool Vocabulary::recordEnabled(quintptr recordId) const
{
  for (auto fieldId : fieldIds())
  {
    if (fieldHasAttribute(fieldId, FieldAttribute::BuiltIn))
    {
      const auto builtIn = fieldBuiltIn(fieldId);
      switch (builtIn)
      {
        case VocabularyDatabase::FieldBuiltIn::Enabled:
          const auto data = dataText(recordId, fieldId);
          if (data.isNull())
          {
            return true;
          }
          else
          {
            return data.toUInt();
          }
      }
    }
  }

  return true;
}

quintptr Vocabulary::recordId(quintptr row) const
{
  if (_cacheEnabled)
  {
    auto recordsTotal = 0;
    for (auto category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); ++category)
    {
      const auto records = recordsTotal + category->size();
      if (row < records)
      {
        return category->at(row - recordsTotal);
      }
      else
      {
        recordsTotal = records;
      }
    }

    return NOT_FOUND;
  }
  else
  {
    return VocabularyDatabase::recordId(row);
  }
}

quintptr Vocabulary::recordId(quintptr categoryId, quintptr row) const
{
  if (_cacheEnabled)
  {
    if (row == NOT_FOUND)
    {
      return NOT_FOUND;
    }
    else
    {
      return _categoryRecords.value(categoryId).at(row);
    }
  }
  else
  {
    return VocabularyDatabase::recordId(categoryId, row);
  }
}

VocabularyDatabase::RecordIdList Vocabulary::recordIds(quintptr categoryId) const
{
  if (_cacheEnabled)
  {
    return _categoryRecords.value(categoryId);
  }
  else
  {
    return VocabularyDatabase::recordIds(categoryId);
  }
}

void Vocabulary::new2(const QString &filePath)
{
  VocabularyDatabase::new2(filePath);
  initCache();
}

void Vocabulary::open(const QString &filePath)
{
  VocabularyDatabase::open(filePath);
  initCache();
}

void Vocabulary::removeCategory(quintptr categoryId)
{
  if (_cacheEnabled)
  {
    const auto records = _categoryRecords.value(categoryId);
    for (auto recordId : records)
    {
      _recordData->remove(recordId);
    }
    _categoryRecords.remove(categoryId);
  }

  VocabularyDatabase::removeCategory(categoryId);
}

void Vocabulary::removeField(quintptr fieldId)
{
  if (_cacheEnabled)
  {
    for (auto fieldData = _recordData->begin(); fieldData != _recordData->end(); ++fieldData)
    {
      fieldData->remove(fieldId);
    }
  }

  VocabularyDatabase::removeField(fieldId);
}

void Vocabulary::removeRecord(quintptr categoryId, quintptr row)
{
  if (_cacheEnabled)
  {
    _recordData->remove(_categoryRecords.value(categoryId).at(row));
    _categoryRecords[categoryId].removeAt(row);
  }

  VocabularyDatabase::removeRecord(categoryId, row);
}

void Vocabulary::setDataText(quintptr categoryId, quintptr row, quintptr fieldId, const QString &data)
{
  if (_cacheEnabled)
  {
    const auto recordId = _categoryRecords.value(categoryId).at(row);
    setDataText(recordId, fieldId, data);
  }
  else
  {
    VocabularyDatabase::setDataText(categoryId, row, fieldId, data);
  }
}

void Vocabulary::setDataText(quintptr recordId, quintptr fieldId, const QString &data)
{
  if (_cacheEnabled)
  {
    _recordData->operator[](recordId).operator[](fieldId) = data;
  }

  VocabularyDatabase::setDataText(recordId, fieldId, data);
}

void Vocabulary::setFieldAttributes(quintptr fieldId, VocabularyDatabase::FieldAttributes attributes)
{
  if (_cacheEnabled)
  {
    _fieldData[fieldId].attributes = attributes;
  }

  VocabularyDatabase::setFieldAttributes(fieldId, attributes);
}

void Vocabulary::setFieldLanguage(quintptr fieldId, VocabularyDatabase::FieldLanguage language)
{
  if (_cacheEnabled)
  {
    _fieldData[fieldId].language = language;
  }

  VocabularyDatabase::setFieldLanguage(fieldId, language);
}

void Vocabulary::setFieldName(quintptr fieldId, const QString &name)
{
  if (_cacheEnabled)
  {
    _fieldData[fieldId].name = name;
  }

  VocabularyDatabase::setFieldName(fieldId, name);
}

void Vocabulary::setFieldTemplateName(quintptr fieldId, const QString &templateName)
{
  if (_cacheEnabled)
  {
    _fieldData[fieldId].templateName = templateName;
  }

  VocabularyDatabase::setFieldTemplateName(fieldId, templateName);
}

void Vocabulary::setRecordByRowCategory(quintptr oldCategoryId, quintptr recordRow, quintptr newCategoryId)
{
  if (_cacheEnabled)
  {
    const auto recordId    = _categoryRecords[oldCategoryId].takeAt(recordRow);
    auto trilRecordIds     = &_categoryRecords[newCategoryId];
    const auto iLowerBound = qLowerBound(trilRecordIds->begin(), trilRecordIds->end(), recordId);
    trilRecordIds->insert(iLowerBound, recordId);

    VocabularyDatabase::SetRecordCategory(recordId, newCategoryId);
  }
  else
  {
    VocabularyDatabase::setRecordByRowCategory(oldCategoryId, recordRow, newCategoryId);
  }
}

void Vocabulary::swapFields(quintptr sourceId, quintptr destinationId)
{
  if (_cacheEnabled)
  {
    // swap in fields table
    const auto fieldTemp      = _fieldData.value(sourceId);
    _fieldData[sourceId]      = _fieldData.value(destinationId);
    _fieldData[destinationId] = fieldTemp;

    // swap in data table
    for (auto record = _recordData->begin(); record != _recordData->end(); ++record)
    {
      auto data = record.operator->();

      auto dataTemp = data->value(sourceId);
      data->operator[](sourceId) = data->value(destinationId);
      data->operator[](destinationId) = dataTemp;
    }
  }

  VocabularyDatabase::swapFields(sourceId, destinationId);
}

void Vocabulary::clearCache()
{
  if (_cacheEnabled)
  {
    _categoryRecords.clear();
    _fieldData.clear();

    // clear large cache in background
    if (_recordData)
    {
      auto clearCacheWorker = new ClearCacheWorker(_recordData);
      clearCacheWorker->start(QThread::LowPriority);
      _recordData = Q_NULLPTR;
    }

    _cacheEnabled = false;
  }
}

Vocabulary::FieldData Vocabulary::fieldData(quintptr fieldId) const
{
  FieldData fieldData;

  fieldData.templateName = VocabularyDatabase::fieldTemplateName(fieldId);
  fieldData.name         = VocabularyDatabase::fieldName(fieldId);
  fieldData.type         = VocabularyDatabase::fieldType(fieldId);
  fieldData.attributes   = VocabularyDatabase::fieldAttributes(fieldId);
  fieldData.builtIn      = VocabularyDatabase::fieldBuiltIn(fieldId);
  fieldData.language     = VocabularyDatabase::fieldLanguage(fieldId);

  return fieldData;
}

void Vocabulary::initCache()
{
  if (isOpen() && _settings->cacheVocabulary())
  {
    if (VocabularyDatabase::recordCount() < _settings->recordsToCache())
    {
      return;
    }

    // fields
    const auto fieldIdList = VocabularyDatabase::fieldIds();
    for (auto fieldId : fieldIdList)
    {
      const auto fieldData2 = fieldData(fieldId);
      _fieldData.insert(fieldId, fieldData2);
    }

    // categories
    auto categoryIdList = VocabularyDatabase::categoryIds();
    for (auto categoryId : categoryIdList)
    {
      const auto recordIdList = VocabularyDatabase::recordIds(categoryId);
      _categoryRecords.insert(categoryId, recordIdList);
    }

    // records
    _recordData = new RecordDataHash();
    _recordData = VocabularyDatabase::dataText();

    _cacheEnabled = true;
  }
}