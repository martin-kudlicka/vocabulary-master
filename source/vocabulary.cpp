#include "vocabulary.h"

#include "settings.h"
#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"

Vocabulary::Vocabulary(const Settings *settings) : _cacheEnabled(false), _recordData(Q_NULLPTR), _settings(settings)
{
}

Vocabulary::~Vocabulary()
{
}

quintptr Vocabulary::addCategory(const QString &name)
{
	const quint8 categoryId = VocabularyDatabase::addCategory(name);

	if (_cacheEnabled)
	{
		_categoryRecords.insert(categoryId, RecordIdList());
	}

	return categoryId;
}

#ifndef EDITION_FREE
void Vocabulary::addField()
{
    const quint8 fieldId = VocabularyDatabase::addField();

	if (_cacheEnabled)
	{
		const FieldData fieldData2 = fieldData(fieldId);
		_fieldData.insert(fieldId, fieldData2);
	}
}
#endif

void Vocabulary::addRecord(quintptr categoryId)
{
	const quint32 recordId = VocabularyDatabase::addRecord(categoryId);

	if (_cacheEnabled)
	{
		_categoryRecords[categoryId].append(recordId);
	}
}

#ifndef EDITION_FREE
void Vocabulary::addRecord(quintptr categoryId, const QStringList &data)
{
	const quint32 recordId = VocabularyDatabase::addRecord(categoryId, data);
	/*_categoryRecords[categoryId].append(recordId);

    // insert data to cache
    quint8 dataIndex = 0;
    FieldDataHash *fieldData = &_recordData->operator[](recordId);
    for (quint8 fieldId : fieldIds())
	{
        fieldData->insert(fieldId, data.at(dataIndex));
        dataIndex++;
    }*/
}
#endif

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
        _fieldData   = _fieldDataBackup;
        *_recordData = _recordDataBackup;
    }

    VocabularyDatabase::endEdit(save);
}

bool Vocabulary::fieldHasAttribute(quintptr fieldId, FieldAttribute attribute) const
{
	const FieldAttributes attributes = fieldAttributes(fieldId);
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
		const quint32 recordId = _categoryRecords.value(categoryId).at(row);
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

#ifndef EDITION_FREE
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
#endif

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
		quint8 pos = 0;
		for (FieldDataHash::const_iterator fieldId = _fieldData.constBegin(); fieldId != _fieldData.constEnd(); fieldId++)
		{
			if (pos == position)
			{
				return fieldId.key();
			}
			else
			{
				pos++;
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

#ifndef EDITION_FREE
QStringList Vocabulary::record(quintptr recordId) const
{
    QStringList data;

    const FieldIdList fieldIdList = fieldIds();
    for (quint8 fieldId : fieldIdList)
	{
        data.append(dataText(recordId, fieldId));
    }

    return data;
}
#endif

quintptr Vocabulary::recordCategory(quintptr recordId) const
{
	if (_cacheEnabled)
	{
		for (CategoryRecordsHash::const_iterator category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); category++)
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
		quint32 recordCount = 0;
		const QList<RecordIdList> allRecordIds = _categoryRecords.values();
		for (RecordIdList recordIds : allRecordIds)
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

#ifndef EDITION_FREE
quintptr Vocabulary::recordCount(quintptr categoryId, bool enabled) const
{
	if (_cacheEnabled)
	{
		quint32 recordCount = 0;

		const RecordIdList recordIds = _categoryRecords.value(categoryId);
		for (quint32 recordId : recordIds)
		{
			if (recordEnabled(recordId))
			{
				recordCount++;
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
		quint32 records = 0;

		const CategoryIdList categoryIdList = categoryIds();
		for (quint8 categoryId : categoryIdList)
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
	for (quint8 fieldId : fieldIds())
	{
		if (fieldHasAttribute(fieldId, FieldAttribute::BuiltIn))
		{
			const FieldBuiltIn builtIn = fieldBuiltIn(fieldId);
			switch (builtIn)
			{
				case VocabularyDatabase::FieldBuiltIn::Enabled:
					const QString data = dataText(recordId, fieldId);
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
#endif

quintptr Vocabulary::recordId(quintptr row) const
{
	if (_cacheEnabled)
	{
		quint32 recordsTotal = 0;
		for (CategoryRecordsHash::const_iterator category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); category++)
		{
			const quint32 records = recordsTotal + category->size();
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

void Vocabulary::new2(
#ifndef EDITION_TRY
	const QString &filePath
#endif
	)
{
	VocabularyDatabase::new2(
#ifndef EDITION_TRY
		filePath
#endif
		);
	initCache();
}

#ifndef EDITION_TRY
void Vocabulary::open(const QString &filePath)
{
	VocabularyDatabase::open(filePath);
	initCache();
}
#else

void Vocabulary::openMemory()
{
    initCache();
}
#endif

void Vocabulary::removeCategory(quintptr categoryId)
{
	if (_cacheEnabled)
	{
		const RecordIdList records = _categoryRecords.value(categoryId);
		for (quint32 recordId : records)
		{
			_recordData->remove(recordId);
		}
		_categoryRecords.remove(categoryId);
	}

	VocabularyDatabase::removeCategory(categoryId);
}

#ifndef EDITION_FREE
void Vocabulary::removeField(quintptr fieldId)
{
	if (_cacheEnabled)
	{
		for (RecordDataHash::iterator fieldData = _recordData->begin(); fieldData != _recordData->end(); fieldData++)
		{
			fieldData->remove(fieldId);
		}
	}

    VocabularyDatabase::removeField(fieldId);
}
#endif

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
		const quint32 recordId = _categoryRecords.value(categoryId).at(row);
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

#ifndef EDITION_FREE
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
		const quint32 recordId                   = _categoryRecords[oldCategoryId].takeAt(recordRow);
		RecordIdList *trilRecordIds              = &_categoryRecords[newCategoryId];
		const RecordIdList::iterator iLowerBound = qLowerBound(trilRecordIds->begin(), trilRecordIds->end(), recordId);
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
		const FieldData fieldTemp = _fieldData.value(sourceId);
		_fieldData[sourceId]      = _fieldData.value(destinationId);
		_fieldData[destinationId] = fieldTemp;

		// swap in data table
		for (RecordDataHash::iterator record = _recordData->begin(); record != _recordData->end(); record++)
		{
	        DataHash *data = record.operator->();

			QString dataTemp                = data->value(sourceId);
			data->operator[](sourceId)      = data->value(destinationId);
			data->operator[](destinationId) = dataTemp;
		}
	}

    VocabularyDatabase::swapFields(sourceId, destinationId);
}
#endif

void Vocabulary::clearCache()
{
	if (_cacheEnabled)
	{
		_categoryRecords.clear();
		_fieldData.clear();

		// clear large cache in background
		if (_recordData)
		{
			ClearCacheWorker *clearCacheWorker = new ClearCacheWorker(_recordData);
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
#ifndef EDITION_FREE
    fieldData.builtIn      = VocabularyDatabase::fieldBuiltIn(fieldId);
#endif
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
        const FieldIdList fieldIdList = VocabularyDatabase::fieldIds();
        for (quint8 fieldId : fieldIdList)
		{
            const FieldData fieldData2 = fieldData(fieldId);
            _fieldData.insert(fieldId, fieldData2);
        }

		// categories
		CategoryIdList categoryIdList = VocabularyDatabase::categoryIds();
		for (quint8 categoryId : categoryIdList)
		{
            const RecordIdList recordIdList = VocabularyDatabase::recordIds(categoryId);
			_categoryRecords.insert(categoryId, recordIdList);
		}

        // records
        _recordData = new RecordDataHash();
        _recordData = VocabularyDatabase::dataText();

		_cacheEnabled = true;
	}
}