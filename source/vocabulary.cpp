#include "vocabulary.h"

#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"

Vocabulary::Vocabulary(const Settings *settings) : _cacheEnabled(false), _recordData(NULL), _settings(settings)
{
} // Vocabulary

Vocabulary::~Vocabulary()
{
} // ~Vocabulary

quint8 Vocabulary::addCategory(const QString &name)
{
	const quint8 categoryId = VocabularyDatabase::addCategory(name);

	if (_cacheEnabled)
	{
		_categoryRecords.insert(categoryId, RecordIdList());
	} // if

	return categoryId;
} // addCategory

#ifndef EDITION_FREE
void Vocabulary::addField()
{
    const quint8 fieldId = VocabularyDatabase::addField();

	if (_cacheEnabled)
	{
		const FieldData fieldData = getFieldData(fieldId);
		_fieldData.insert(fieldId, fieldData);
	} // if
} // addField
#endif

void Vocabulary::addRecord(quint8 categoryId)
{
	const quint32 recordId = VocabularyDatabase::addRecord(categoryId);

	if (_cacheEnabled)
	{
		_categoryRecords[categoryId].append(recordId);
	} // if
} // addRecord

#ifndef EDITION_FREE
void Vocabulary::addRecord(quint8 categoryId, const QStringList &data)
{
	const quint32 recordId = VocabularyDatabase::addRecord(categoryId, data);
	/*_categoryRecords[categoryId].append(recordId);

    // insert data to cache
    quint8 dataIndex = 0;
    FieldDataHash *fieldData = &_recordData->operator[](recordId);
    foreach (quint8 fieldId, fieldIds())
	{
        fieldData->insert(fieldId, data.at(dataIndex));
        dataIndex++;
    } // foreach*/
} // addRecord
#endif

void Vocabulary::beginEdit()
{
	if (_cacheEnabled)
	{
		_fieldDataBackup  = _fieldData;
		_recordDataBackup = *_recordData;
		}

    VocabularyDatabase::beginEdit();
} // beginEdit

void Vocabulary::close()
{
	clearCache();
	VocabularyDatabase::close();
} // close

void Vocabulary::endEdit(bool save /* true */)
{
    if (!save && _cacheEnabled)
	{
        _fieldData   = _fieldDataBackup;
        *_recordData = _recordDataBackup;
    } // if

    VocabularyDatabase::endEdit(save);
} // endEdit

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
			_recordData = NULL;
		} // if

		_cacheEnabled = false;
	} // if
} // clearCache

bool Vocabulary::fieldHasAttribute(quint8 fieldId, FieldAttribute attribute) const
{
	const FieldAttributes attributes = fieldAttributes(fieldId);
	return attributes & attribute;
} // fieldHasAttribute

VocabularyDatabase::CategoryIdList Vocabulary::categoryIds() const
{
	if (_cacheEnabled)
	{
		return _categoryRecords.keys();
	}
	else
	{
		return VocabularyDatabase::categoryIds();
	} // if else
} // categoryIds

QString Vocabulary::dataText(quint8 categoryId, quint32 row, quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		const quint32 recordId = _categoryRecords.value(categoryId).at(row);
		return dataText(recordId, fieldId);
	}
	else
	{
		return VocabularyDatabase::dataText(categoryId, row, fieldId);
	} // if else
} // dataText

QString Vocabulary::dataText(quint32 recordId, quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _recordData->value(recordId).value(fieldId);
	}
	else
	{
		return VocabularyDatabase::dataText(recordId, fieldId);
	} // if else
} // dataText

VocabularyDatabase::FieldAttributes Vocabulary::fieldAttributes(quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _fieldData.value(fieldId).attributes;
	}
	else
	{
		return VocabularyDatabase::fieldAttributes(fieldId);
	} // if else
} // fieldAttributes

#ifndef EDITION_FREE
VocabularyDatabase::FieldBuiltIn Vocabulary::fieldBuiltIn(quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _fieldData.value(fieldId).builtIn;
	}
	else
	{
		return VocabularyDatabase::fieldBuiltIn(fieldId);
	} // if else
} // fieldBuiltIn
#endif

quint8 Vocabulary::fieldCount() const
{
	if (_cacheEnabled)
	{
		return _fieldData.size();
	}
	else
	{
		return VocabularyDatabase::fieldCount();
	} // if else
} // fieldCount

Vocabulary::FieldData Vocabulary::getFieldData(quint8 fieldId) const
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
} // getFieldData

quint8 Vocabulary::fieldId(quint8 position) const
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
			} // if else
		} // for

		return NOT_FOUND;
	}
	else
	{
		return VocabularyDatabase::fieldId(position);
	} // if else
} // fieldId

VocabularyDatabase::FieldIdList Vocabulary::fieldIds() const
{
	if (_cacheEnabled)
	{
		return _fieldData.keys();
	}
	else
	{
		return VocabularyDatabase::fieldIds();
	} // if else
} // fieldIds

VocabularyDatabase::FieldLanguage Vocabulary::fieldLanguage(quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _fieldData.value(fieldId).language;
	}
	else
	{
		return VocabularyDatabase::fieldLanguage(fieldId);
	} // if else
} // fieldLanguage

QString Vocabulary::fieldName(quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _fieldData.value(fieldId).name;
	}
	else
	{
		return VocabularyDatabase::fieldName(fieldId);
	} // if else
} // fieldName

QString Vocabulary::fieldTemplateName(quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _fieldData.value(fieldId).templateName;
	}
	else
	{
		return VocabularyDatabase::fieldTemplateName(fieldId);
	} // if else
} // fieldTemplateName

VocabularyDatabase::FieldType Vocabulary::fieldType(quint8 fieldId) const
{
	if (_cacheEnabled)
	{
		return _fieldData.value(fieldId).type;
	}
	else
	{
		return VocabularyDatabase::fieldType(fieldId);
	} // if else
} // fieldType

#ifndef EDITION_FREE
QStringList Vocabulary::record(quint32 recordId) const
{
    QStringList data;

    const FieldIdList fieldIdList = fieldIds();
    foreach (quint8 fieldId, fieldIdList)
	{
        data.append(dataText(recordId, fieldId));
    } // foreach

    return data;
} // record
#endif

quint8 Vocabulary::recordCategory(quint32 recordId) const
{
	if (_cacheEnabled)
	{
		for (CategoryRecordsHash::const_iterator category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); category++)
		{
			if (category->contains(recordId))
			{
				return category.key();
			} // if
		} // for

		return NOT_FOUND;
	}
	else
	{
		return VocabularyDatabase::recordCategory(recordId);
	}
} // recordCategory

quint32 Vocabulary::recordCount() const
{
	if (_cacheEnabled)
	{
		quint32 recordCount = 0;
		const QList<RecordIdList> allRecordIds = _categoryRecords.values();
		foreach (RecordIdList recordIds, allRecordIds)
		{
			recordCount += recordIds.size();
		} // foreach

		return recordCount;
	}
	else
	{
		return VocabularyDatabase::recordCount();
	} // if else
} // recordCount

quint32 Vocabulary::recordCount(quint8 categoryId) const
{
	if (_cacheEnabled)
	{
		return _categoryRecords.value(categoryId).size();
	}
	else
	{
		return VocabularyDatabase::recordCount(categoryId);
	} // if else
} // recordCount

#ifndef EDITION_FREE
quint32 Vocabulary::recordCount(quint8 categoryId, bool enabled) const
{
	if (_cacheEnabled)
	{
		quint32 recordCount = 0;

		const RecordIdList recordIds = _categoryRecords.value(categoryId);
		foreach (quint32 recordId, recordIds)
		{
			if (recordEnabled(recordId))
			{
				recordCount++;
			} // if
		} // foreach

		return recordCount;
	}
	else
	{
		return VocabularyDatabase::recordCount(categoryId, enabled);
	} // if else
} // recordCount

quint32 Vocabulary::recordCount(bool enabled) const
{
	if (_cacheEnabled)
	{
		quint32 records = 0;

		const CategoryIdList categoryIdList = categoryIds();
		foreach (quint8 categoryId, categoryIdList)
		{
			if (categoryEnabled(categoryId))
			{
				records += recordCount(categoryId, enabled);
			} // if
		} // foreach

		return records;
	}
	else
	{
		return VocabularyDatabase::recordCount(enabled);
	} // if else
} // recordCount

bool Vocabulary::recordEnabled(quint32 recordId) const
{
	foreach (quint8 fieldId, fieldIds())
	{
		if (fieldHasAttribute(fieldId, FieldAttributeBuiltIn))
		{
			const FieldBuiltIn builtIn = fieldBuiltIn(fieldId);
			switch (builtIn)
			{
				case VocabularyDatabase::FieldBuiltInEnabled:
					const QString data = dataText(recordId, fieldId);
					if (data.isNull())
					{
						return true;
					}
					else
					{
						return data.toUInt();
					} // if else
			} // switch
		} // if
	} // foreach

	return true;
} // recordEnabled
#endif

quint32 Vocabulary::recordId(quint32 row) const
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
			} // if else
		} // for

		return NOT_FOUND;
	}
	else
	{
		return VocabularyDatabase::recordId(row);
	} // if else
} // recordId

quint32 Vocabulary::recordId(quint8 categoryId, quint32 row) const
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
		} // if else
	}
	else
	{
		return VocabularyDatabase::recordId(categoryId, row);
	} // if else
} // recordId

VocabularyDatabase::RecordIdList Vocabulary::recordIds(quint8 categoryId) const
{
	if (_cacheEnabled)
	{
		return _categoryRecords.value(categoryId);
	}
	else
	{
		return VocabularyDatabase::recordIds(categoryId);
	} // if else
} // recordIds

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
        foreach (quint8 fieldId, fieldIdList)
		{
            const FieldData fieldData = getFieldData(fieldId);
            _fieldData.insert(fieldId, fieldData);
        } // foreach

		// categories
		CategoryIdList categoryIdList = VocabularyDatabase::categoryIds();
		foreach (quint8 categoryId, categoryIdList)
		{
            const RecordIdList recordIdList = VocabularyDatabase::recordIds(categoryId);
			_categoryRecords.insert(categoryId, recordIdList);
		} // foreach

        // records
        _recordData = new RecordDataHash();
        _recordData = VocabularyDatabase::dataText();

		_cacheEnabled = true;
	} // if
} // initCache

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
} // new

#ifndef EDITION_TRY
void Vocabulary::open(const QString &filePath)
{
	VocabularyDatabase::open(filePath);
	initCache();
} // open
#else

void Vocabulary::openMemory()
{
    initCache();
} // openMemory
#endif

void Vocabulary::removeCategory(quint8 categoryId)
{
	if (_cacheEnabled)
	{
		const RecordIdList records = _categoryRecords.value(categoryId);
		foreach (quint32 recordId, records)
		{
			_recordData->remove(recordId);
		} // foreach
		_categoryRecords.remove(categoryId);
	} // if

	VocabularyDatabase::removeCategory(categoryId);
} // removeCategory

#ifndef EDITION_FREE
void Vocabulary::removeField(quint8 fieldId)
{
	if (_cacheEnabled)
	{
		for (RecordDataHash::iterator fieldData = _recordData->begin(); fieldData != _recordData->end(); fieldData++)
		{
			fieldData->remove(fieldId);
		} // for
	} // if

    VocabularyDatabase::removeField(fieldId);
} // removeField
#endif

void Vocabulary::removeRecord(quint8 categoryId, quint32 row)
{
	if (_cacheEnabled)
	{
		_recordData->remove(_categoryRecords.value(categoryId).at(row));
		_categoryRecords[categoryId].removeAt(row);
	} // if

	VocabularyDatabase::removeRecord(categoryId, row);
} // removeRecord

void Vocabulary::setDataText(quint8 categoryId, quint32 row, quint8 fieldId, const QString &data)
{
	if (_cacheEnabled)
	{
		const quint32 recordId = _categoryRecords.value(categoryId).at(row);
		setDataText(recordId, fieldId, data);
	}
	else
	{
		VocabularyDatabase::setDataText(categoryId, row, fieldId, data);
	} // if else
} // setDataText

void Vocabulary::setDataText(quint32 recordId, quint8 fieldId, const QString &data)
{
	if (_cacheEnabled)
	{
		_recordData->operator[](recordId).operator[](fieldId) = data;
	} // if

    VocabularyDatabase::setDataText(recordId, fieldId, data);
} // setDataText

#ifndef EDITION_FREE
void Vocabulary::setFieldAttributes(quint8 fieldId, VocabularyDatabase::FieldAttributes attributes)
{
	if (_cacheEnabled)
	{
		_fieldData[fieldId].attributes = attributes;
	} // if

    VocabularyDatabase::setFieldAttributes(fieldId, attributes);
} // setFieldAttributes

void Vocabulary::setFieldLanguage(quint8 fieldId, VocabularyDatabase::FieldLanguage language)
{
	if (_cacheEnabled)
	{
		_fieldData[fieldId].language = language;
	} // if

    VocabularyDatabase::setFieldLanguage(fieldId, language);
} // setFieldLanguage

void Vocabulary::setFieldName(quint8 fieldId, const QString &name)
{
	if (_cacheEnabled)
	{
		_fieldData[fieldId].name = name;
	} // if

    VocabularyDatabase::setFieldName(fieldId, name);
} // setFieldName

void Vocabulary::setFieldTemplateName(quint8 fieldId, const QString &templateName)
{
	if (_cacheEnabled)
	{
		_fieldData[fieldId].templateName = templateName;
	} // if

    VocabularyDatabase::setFieldTemplateName(fieldId, templateName);
} // setFieldTemplateName

void Vocabulary::setRecordByRowCategory(quint8 oldCategoryId, quint32 recordRow, quint8 newCategoryId)
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
	} // if else
} // setRecordByRowCategory

void Vocabulary::swapFields(quint8 sourceId, quint8 destinationId)
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
		} // for
	} // if

    VocabularyDatabase::swapFields(sourceId, destinationId);
} // swapFields
#endif