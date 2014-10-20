#include "vocabulary.h"

#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"

Vocabulary::Vocabulary()
{
	_recordData = NULL;
} // Vocabulary

Vocabulary::~Vocabulary()
{
}

quint8 Vocabulary::addCategory(const QString &name)
{
	const quint8 categoryId = VocabularyDatabase::addCategory(name);
	_categoryRecords.insert(categoryId, RecordIdList());
	return categoryId;
} // addCategory

#ifndef EDITION_FREE
void Vocabulary::addField()
{
    const quint8 fieldId = VocabularyDatabase::addField();

    const FieldData fieldData = getFieldData(fieldId);
    _fieldData.insert(fieldId, fieldData);
} // addField
#endif

void Vocabulary::addRecord(quint8 categoryId)
{
	const quint32 recordId = VocabularyDatabase::addRecord(categoryId);
	_categoryRecords[categoryId].append(recordId);
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
    _fieldDataBackup  = _fieldData;
    _recordDataBackup = *_recordData;

    VocabularyDatabase::beginEdit();
} // beginEdit

void Vocabulary::close()
{
	clearCache();
	VocabularyDatabase::close();
} // close

void Vocabulary::endEdit(bool save /* true */)
{
    if (!save)
	{
        _fieldData   = _fieldDataBackup;
        *_recordData = _recordDataBackup;
    } // if

    VocabularyDatabase::endEdit(save);
} // endEdit

void Vocabulary::clearCache()
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
} // clearCache

bool Vocabulary::fieldHasAttribute(quint8 fieldId, FieldAttribute attribute) const
{
	const FieldAttributes attributes = fieldAttributes(fieldId);
	return attributes & attribute;
} // fieldHasAttribute

VocabularyDatabase::CategoryIdList Vocabulary::categoryIds() const
{
    return _categoryRecords.keys();
} // categoryIds

QString Vocabulary::dataText(quint8 categoryId, quint32 row, quint8 fieldId) const
{
    const quint32 recordId = _categoryRecords.value(categoryId).at(row);
    return dataText(recordId, fieldId);
} // dataText

QString Vocabulary::dataText(quint32 recordId, quint8 fieldId) const
{
    return _recordData->value(recordId).value(fieldId);
} // dataText

VocabularyDatabase::FieldAttributes Vocabulary::fieldAttributes(quint8 fieldId) const
{
    return _fieldData.value(fieldId).attributes;
} // fieldAttributes

#ifndef EDITION_FREE
VocabularyDatabase::FieldBuiltIn Vocabulary::fieldBuiltIn(quint8 fieldId) const
{
    return _fieldData.value(fieldId).builtIn;
} // fieldBuiltIn
#endif

quint8 Vocabulary::fieldCount() const
{
    return _fieldData.size();
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
    quint8 pos = 0;
    for (FieldDataMap::const_iterator fieldId = _fieldData.constBegin(); fieldId != _fieldData.constEnd(); fieldId++)
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
} // fieldId

VocabularyDatabase::FieldIdList Vocabulary::fieldIds() const
{
    return _fieldData.keys();
} // fieldIds

VocabularyDatabase::FieldLanguage Vocabulary::fieldLanguage(quint8 fieldId) const
{
    return _fieldData.value(fieldId).language;
} // fieldLanguage

QString Vocabulary::fieldName(quint8 fieldId) const
{
    return _fieldData.value(fieldId).name;
} // fieldName

QString Vocabulary::fieldTemplateName(quint8 fieldId) const
{
    return _fieldData.value(fieldId).templateName;
} // fieldTemplateName

VocabularyDatabase::FieldType Vocabulary::fieldType(quint8 fieldId) const
{
    return _fieldData.value(fieldId).type;
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
    for (CategoryRecordsMap::const_iterator category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); category++)
	{
        if (category->contains(recordId))
		{
            return category.key();
        } // if
    } // for

    return NOT_FOUND;
} // recordCategory

quint32 Vocabulary::recordCount() const
{
	quint32 recordCount = 0;
	const QList<RecordIdList> allRecordIds = _categoryRecords.values();
	foreach (RecordIdList recordIds, allRecordIds)
	{
		recordCount += recordIds.size();
	} // foreach

	return recordCount;
} // recordCount

quint32 Vocabulary::recordCount(quint8 categoryId) const
{
	return _categoryRecords.value(categoryId).size();
} // recordCount

#ifndef EDITION_FREE
quint32 Vocabulary::recordCount(quint8 categoryId, bool enabled) const
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
} // recordCount

quint32 Vocabulary::recordCount(bool enabled) const
{
	quint32 records = 0;

	const CategoryIdList categoryIdList = categoryIds();
	foreach (quint8 categoryId, categoryIdList)
	{
		if (GetCategoryEnabled(categoryId))
		{
			records += recordCount(categoryId, enabled);
		} // if
	} // foreach

	return records;
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
						return data.toInt();
					} // if else
			} // switch
		} // if
	} // foreach

	return true;
} // recordEnabled
#endif

quint32 Vocabulary::recordId(quint32 row) const
{
    quint32 recordsTotal = 0;
    for (CategoryRecordsMap::const_iterator category = _categoryRecords.constBegin(); category != _categoryRecords.constEnd(); category++)
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
} // recordId

quint32 Vocabulary::recordId(quint8 categoryId, quint32 row) const
{
    if (row == NOT_FOUND)
	{
        return NOT_FOUND;
    }
	else
	{
        return _categoryRecords.value(categoryId).at(row);
    } // if else
} // recordId

VocabularyDatabase::RecordIdList Vocabulary::recordIds(quint8 categoryId) const
{
    return _categoryRecords.value(categoryId);
} // recordIds

void Vocabulary::initCache()
{
	if (IsOpen())
	{
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
    const RecordIdList records = _categoryRecords.value(categoryId);
    foreach (quint32 recordId, records)
	{
        _recordData->remove(recordId);
    } // foreach
	_categoryRecords.remove(categoryId);

	VocabularyDatabase::removeCategory(categoryId);
} // removeCategory

#ifndef EDITION_FREE
void Vocabulary::removeField(quint8 fieldId)
{
    for (RecordDataHash::iterator fieldData = _recordData->begin(); fieldData != _recordData->end(); fieldData++)
	{
        fieldData->remove(fieldId);
    } // for

    VocabularyDatabase::removeField(fieldId);
} // removeField
#endif

void Vocabulary::removeRecord(quint8 categoryId, quint32 row)
{
    _recordData->remove(_categoryRecords.value(categoryId).at(row));
    _categoryRecords[categoryId].removeAt(row);

	VocabularyDatabase::removeRecord(categoryId, row);
} // removeRecord

void Vocabulary::setDataText(quint8 categoryId, quint32 row, quint8 fieldId, const QString &data)
{
    const quint32 recordId = _categoryRecords.value(categoryId).at(row);
    setDataText(recordId, fieldId, data);
} // setDataText

void Vocabulary::setDataText(quint32 recordId, quint8 fieldId, const QString &data)
{
    _recordData->operator[](recordId).operator[](fieldId) = data;
    VocabularyDatabase::setDataText(recordId, fieldId, data);
} // setDataText

#ifndef EDITION_FREE
void Vocabulary::setFieldAttributes(quint8 fieldId, VocabularyDatabase::FieldAttributes attributes)
{
    _fieldData[fieldId].attributes = attributes;
    VocabularyDatabase::setFieldAttributes(fieldId, attributes);
} // setFieldAttributes

void Vocabulary::setFieldLanguage(quint8 fieldId, VocabularyDatabase::FieldLanguage language)
{
    _fieldData[fieldId].language = language;
    VocabularyDatabase::setFieldLanguage(fieldId, language);
} // setFieldLanguage

void Vocabulary::setFieldName(quint8 fieldId, const QString &name)
{
    _fieldData[fieldId].name = name;
    VocabularyDatabase::setFieldName(fieldId, name);
} // setFieldName

void Vocabulary::setFieldTemplateName(quint8 fieldId, const QString &templateName)
{
    _fieldData[fieldId].templateName = templateName;
    VocabularyDatabase::setFieldTemplateName(fieldId, templateName);
} // setFieldTemplateName

void Vocabulary::setRecordByRowCategory(quint8 oldCategoryId, quint32 recordRow, quint8 newCategoryId)
{
    const quint32 recordId                   = _categoryRecords[oldCategoryId].takeAt(recordRow);
    RecordIdList *trilRecordIds              = &_categoryRecords[newCategoryId];
    const RecordIdList::iterator iLowerBound = qLowerBound(trilRecordIds->begin(), trilRecordIds->end(), recordId);
    trilRecordIds->insert(iLowerBound, recordId);

    VocabularyDatabase::SetRecordCategory(recordId, newCategoryId);
} // setRecordByRowCategory

void Vocabulary::swapFields(quint8 sourceId, quint8 destinationId)
{
    // swap in fields table
    const FieldData fieldTemp = _fieldData.value(sourceId);
    _fieldData[sourceId]      = _fieldData.value(destinationId);
    _fieldData[destinationId] = fieldTemp;

    // swap in data table
    for (RecordDataHash::iterator record = _recordData->begin(); record != _recordData->end(); record++)
	{
        tDataHash *data = record.operator->();

		QString dataTemp                = data->value(sourceId);
		data->operator[](sourceId)      = data->value(destinationId);
		data->operator[](destinationId) = dataTemp;
    } // for

    VocabularyDatabase::swapFields(sourceId, destinationId);
} // swapFields
#endif