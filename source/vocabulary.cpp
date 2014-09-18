#include "vocabulary.h"

#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"

const int Vocabulary::addCategory(const QString &pName)
{
	int iCategoryId = VocabularyDatabase::addCategory(pName);
	_tcrmCategoryRecords.insert(iCategoryId, RecordIdList());
	return iCategoryId;
} // addCategory

#ifndef EDITION_FREE
const void Vocabulary::addField()
{
    int iFieldId = VocabularyDatabase::addField();

    FieldData sfdFieldData = getFieldData(iFieldId);
    _tfdmFieldData.insert(iFieldId, sfdFieldData);
} // addField
#endif

const void Vocabulary::addRecord(quint8 pCategoryId)
{
	int iRecordId = VocabularyDatabase::addRecord(pCategoryId);
	_tcrmCategoryRecords[pCategoryId].append(iRecordId);
} // addRecord

#ifndef EDITION_FREE
const void Vocabulary::addRecord(quint8 pCategoryId, const QStringList &pData)
{
	int iRecordId = VocabularyDatabase::addRecord(pCategoryId, pData);
	/*_tcrmCategoryRecords[pCategoryId].append(iRecordId);

    // insert data to cache
    int iData = 0;
    tFieldDataHash *tfdhFieldData = &_trdhRecordData->operator[](iRecordId);
    foreach (int iFieldId, fieldIds()) {
        tfdhFieldData->insert(iFieldId, pData.at(iData));
        iData++;
    } // foreach*/
} // addRecord
#endif

const void Vocabulary::beginEdit()
{
    _tfdmFieldDataBackup = _tfdmFieldData;
    _trdhRecordDataBackup = *_trdhRecordData;

    VocabularyDatabase::beginEdit();
} // beginEdit

const void Vocabulary::close()
{
	clearCache();
	VocabularyDatabase::close();
} // close

const void Vocabulary::endEdit(bool pSave /* true */)
{
    if (!pSave) {
        _tfdmFieldData = _tfdmFieldDataBackup;
        *_trdhRecordData = _trdhRecordDataBackup;
    } // if

    VocabularyDatabase::endEdit(pSave);
} // endEdit

const void Vocabulary::clearCache()
{
	_tcrmCategoryRecords.clear();
	_tfdmFieldData.clear();

    // clear large cache in background
    if (_trdhRecordData) {
        ClearCacheWorker *ccwClearCacheWorker = new ClearCacheWorker(_trdhRecordData);
        ccwClearCacheWorker->start(QThread::LowPriority);
        _trdhRecordData = NULL;
    } // if
} // clearCache

const bool Vocabulary::fieldHasAttribute(quint8 pFieldId, FieldAttribute pAttribute) const
{
	FieldAttributes qfaAttributes = fieldAttributes(pFieldId);
	return qfaAttributes & pAttribute;
} // fieldHasAttribute

const VocabularyDatabase::CategoryIdList Vocabulary::categoryIds() const
{
    return _tcrmCategoryRecords.keys();
} // categoryIds

const QString Vocabulary::dataText(quint8 pCategoryId, quint32 pRow, quint8 pFieldId) const
{
    int iRecordId = _tcrmCategoryRecords.value(pCategoryId).at(pRow);
    return dataText(iRecordId, pFieldId);
} // dataText

const QString Vocabulary::dataText(quint32 pRecordId, quint8 pFieldId) const
{
    return _trdhRecordData->value(pRecordId).value(pFieldId);
} // dataText

const VocabularyDatabase::FieldAttributes Vocabulary::fieldAttributes(quint8 pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).attributes;
} // fieldAttributes

#ifndef EDITION_FREE
const VocabularyDatabase::FieldBuiltIn Vocabulary::fieldBuiltIn(quint8 pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).builtIn;
} // fieldBuiltIn
#endif

const int Vocabulary::fieldCount() const
{
    return _tfdmFieldData.size();
} // fieldCount

const Vocabulary::FieldData Vocabulary::getFieldData(quint8 pFieldId) const
{
    FieldData sfdFieldData;

    sfdFieldData.templateName = VocabularyDatabase::fieldTemplateName(pFieldId);
    sfdFieldData.name = VocabularyDatabase::fieldName(pFieldId);
	sfdFieldData.type = VocabularyDatabase::fieldType(pFieldId);
	sfdFieldData.attributes = VocabularyDatabase::fieldAttributes(pFieldId);
#ifndef EDITION_FREE
    sfdFieldData.builtIn = VocabularyDatabase::fieldBuiltIn(pFieldId);
#endif
    sfdFieldData.language = VocabularyDatabase::fieldLanguage(pFieldId);

    return sfdFieldData;
} // getFieldData

const int Vocabulary::fieldId(quint8 pPosition) const
{
    int iPos = 0;
    for (FieldDataMap::const_iterator ciFieldId = _tfdmFieldData.constBegin(); ciFieldId != _tfdmFieldData.constEnd(); ciFieldId++) {
        if (iPos == pPosition) {
            return ciFieldId.key();
        } else {
            iPos++;
        } // if else
    } // for

    return NOT_FOUND;
} // fieldId

const VocabularyDatabase::FieldIdList Vocabulary::fieldIds() const
{
    return _tfdmFieldData.keys();
} // fieldIds

const VocabularyDatabase::FieldLanguage Vocabulary::fieldLanguage(quint8 pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).language;
} // fieldLanguage

const QString Vocabulary::fieldName(quint8 pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).name;
} // fieldName

const QString Vocabulary::fieldTemplateName(quint8 pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).templateName;
} // fieldTemplateName

const VocabularyDatabase::FieldType Vocabulary::fieldType(quint8 pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).type;
} // fieldType

#ifndef EDITION_FREE
const QStringList Vocabulary::record(quint32 pRecordId) const
{
    QStringList qslData;

    FieldIdList tfilFieldIds = fieldIds();
    foreach (int iFieldId, tfilFieldIds) {
        qslData.append(dataText(pRecordId, iFieldId));
    } // foreach

    return qslData;
} // record
#endif

const int Vocabulary::recordCategory(quint32 pRecordId) const
{
    for (CategoryRecordsMap::const_iterator ciCategory = _tcrmCategoryRecords.constBegin(); ciCategory != _tcrmCategoryRecords.constEnd(); ciCategory++) {
        if (ciCategory->contains(pRecordId)) {
            return ciCategory.key();
        } // if
    } // for

    return NOT_FOUND;
} // recordCategory

const int Vocabulary::recordCount() const
{
	int iRecordCount = 0;
	QList<RecordIdList> qlAllRecordIds = _tcrmCategoryRecords.values();
	foreach (RecordIdList trilRecordIds, qlAllRecordIds) {
		iRecordCount += trilRecordIds.size();
	} // foreach

	return iRecordCount;
} // recordCount

const int Vocabulary::recordCount(quint8 pCategoryId) const
{
	return _tcrmCategoryRecords.value(pCategoryId).size();
} // recordCount

#ifndef EDITION_FREE
const int Vocabulary::recordCount(quint8 pCategoryId, bool pEnabled) const
{
	int iRecordCount = 0;

	RecordIdList trilRecordIds = _tcrmCategoryRecords.value(pCategoryId);
	foreach (int iRecordId, trilRecordIds) {
		if (recordEnabled(iRecordId)) {
			iRecordCount++;
		} // if
	} // foreach

	return iRecordCount;
} // recordCount

const int Vocabulary::recordCount(bool pEnabled) const
{
	int iRecordCount = 0;

	CategoryIdList tcilCategoryIds = categoryIds();
	foreach (int iCategoryId, tcilCategoryIds) {
		if (GetCategoryEnabled(iCategoryId)) {
			iRecordCount += recordCount(iCategoryId, pEnabled);
		} // if
	} // foreach

	return iRecordCount;
} // recordCount

const bool Vocabulary::recordEnabled(quint32 pRecordId) const
{
	foreach (int iFieldId, fieldIds()) {
		if (fieldHasAttribute(iFieldId, FieldAttributeBuiltIn)) {
			FieldBuiltIn efbBuiltIn = fieldBuiltIn(iFieldId);
			switch (efbBuiltIn) {
				case VocabularyDatabase::FieldBuiltInEnabled:
					QString qsData = dataText(pRecordId, iFieldId);
					if (qsData.isNull()) {
						return true;
					} else {
						return qsData.toInt();
					} // if else
			} // switch
		} // if
	} // foreach

	return true;
} // recordEnabled
#endif

const int Vocabulary::recordId(quint32 pRow) const
{
    int iRecordsTotal = 0;
    for (CategoryRecordsMap::const_iterator ciCategory = _tcrmCategoryRecords.constBegin(); ciCategory != _tcrmCategoryRecords.constEnd(); ciCategory++) {
        int iRecords = iRecordsTotal + ciCategory->size();
        if (pRow < iRecords) {
            return ciCategory->at(pRow - iRecordsTotal);
        } else {
            iRecordsTotal = iRecords;
        } // if else
    } // for

    return NOT_FOUND;
} // recordId

const int Vocabulary::recordId(quint8 pCategoryId, quint32 pRow) const
{
    if (pRow == NOT_FOUND) {
        return NOT_FOUND;
    } else {
        return _tcrmCategoryRecords.value(pCategoryId).at(pRow);
    } // if else
} // recordId

const VocabularyDatabase::RecordIdList Vocabulary::recordIds(quint8 pCategoryId) const
{
    return _tcrmCategoryRecords.value(pCategoryId);
} // recordIds

const void Vocabulary::initCache()
{
	if (IsOpen()) {
        // fields
        FieldIdList tfilFieldIds = VocabularyDatabase::fieldIds();
        foreach (int iFieldId, tfilFieldIds) {
            FieldData sfdFieldData = getFieldData(iFieldId);
            _tfdmFieldData.insert(iFieldId, sfdFieldData);
        } // foreach

		// categories
		CategoryIdList tcilCategoryIds = VocabularyDatabase::categoryIds();
		foreach (int iCategoryId, tcilCategoryIds) {
            RecordIdList tdilRecordIds = VocabularyDatabase::recordIds(iCategoryId);
			_tcrmCategoryRecords.insert(iCategoryId, tdilRecordIds);
		} // foreach

        // records
        _trdhRecordData = new RecordDataHash();
        _trdhRecordData = VocabularyDatabase::dataText();
	} // if
} // initCache

const void Vocabulary::new2(
#ifndef EDITION_TRY
	const QString &pFilePath
#endif
	)
{
	VocabularyDatabase::new2(
#ifndef EDITION_TRY
		pFilePath
#endif
		);
	initCache();
} // new

#ifndef EDITION_TRY
const void Vocabulary::open(const QString &pFilePath)
{
	VocabularyDatabase::open(pFilePath);
	initCache();
} // open
#else

const void Vocabulary::openMemory()
{
    initCache();
} // openMemory
#endif

const void Vocabulary::removeCategory(quint8 pCategoryId)
{
    RecordIdList trilRecords = _tcrmCategoryRecords.value(pCategoryId);
    foreach (int iRecordId, trilRecords) {
        _trdhRecordData->remove(iRecordId);
    } // foreach
	_tcrmCategoryRecords.remove(pCategoryId);

	VocabularyDatabase::removeCategory(pCategoryId);
} // removeCategory

#ifndef EDITION_FREE
const void Vocabulary::removeField(quint8 pFieldId)
{
    for (RecordDataHash::iterator iFieldData = _trdhRecordData->begin(); iFieldData != _trdhRecordData->end(); iFieldData++) {
        iFieldData->remove(pFieldId);
    } // for

    VocabularyDatabase::removeField(pFieldId);
} // removeField
#endif

const void Vocabulary::removeRecord(quint8 pCategoryId, quint32 pRow)
{
    _trdhRecordData->remove(_tcrmCategoryRecords.value(pCategoryId).at(pRow));
    _tcrmCategoryRecords[pCategoryId].removeAt(pRow);

	VocabularyDatabase::removeRecord(pCategoryId, pRow);
} // removeRecord

const void Vocabulary::setDataText(quint8 pCategoryId, quint32 pRow, quint8 pFieldId, const QString &pData)
{
    int iRecordId = _tcrmCategoryRecords.value(pCategoryId).at(pRow);
    setDataText(iRecordId, pFieldId, pData);
} // setDataText

const void Vocabulary::setDataText(quint32 pRecordId, quint8 pFieldId, const QString &pData)
{
    _trdhRecordData->operator[](pRecordId).operator[](pFieldId) = pData;
    VocabularyDatabase::setDataText(pRecordId, pFieldId, pData);
} // setDataText

#ifndef EDITION_FREE
const void Vocabulary::setFieldAttributes(quint8 pFieldId, VocabularyDatabase::FieldAttributes pAttributes)
{
    _tfdmFieldData[pFieldId].attributes = pAttributes;
    VocabularyDatabase::setFieldAttributes(pFieldId, pAttributes);
} // setFieldAttributes

const void Vocabulary::setFieldLanguage(quint8 pFieldId, VocabularyDatabase::FieldLanguage pLanguage)
{
    _tfdmFieldData[pFieldId].language = pLanguage;
    VocabularyDatabase::setFieldLanguage(pFieldId, pLanguage);
} // setFieldLanguage

const void Vocabulary::setFieldName(quint8 pFieldId, const QString &pName)
{
    _tfdmFieldData[pFieldId].name = pName;
    VocabularyDatabase::setFieldName(pFieldId, pName);
} // setFieldName

const void Vocabulary::setFieldTemplateName(quint8 pFieldId, const QString &pTemplateName)
{
    _tfdmFieldData[pFieldId].templateName = pTemplateName;
    VocabularyDatabase::setFieldTemplateName(pFieldId, pTemplateName);
} // setFieldTemplateName

const void Vocabulary::setRecordByRowCategory(quint8 pOldCategoryId, quint32 pRecordRow, quint8 pNewCategoryId)
{
    int iRecordId = _tcrmCategoryRecords[pOldCategoryId].takeAt(pRecordRow);
    RecordIdList *trilRecordIds = &_tcrmCategoryRecords[pNewCategoryId];
    RecordIdList::iterator iLowerBound = qLowerBound(trilRecordIds->begin(), trilRecordIds->end(), iRecordId);
    trilRecordIds->insert(iLowerBound, iRecordId);

    VocabularyDatabase::SetRecordCategory(iRecordId, pNewCategoryId);
} // setRecordByRowCategory

const void Vocabulary::swapFields(quint8 pSourceId, quint8 pDestinationId)
{
    // swap in fields table
    FieldData sfdFieldTemp = _tfdmFieldData.value(pSourceId);
    _tfdmFieldData[pSourceId] = _tfdmFieldData.value(pDestinationId);
    _tfdmFieldData[pDestinationId] = sfdFieldTemp;

    // swap in data table
    for (RecordDataHash::iterator iRecord = _trdhRecordData->begin(); iRecord != _trdhRecordData->end(); iRecord++) {
        tDataHash *tdhData = iRecord.operator->();

        QString qsDataTemp = tdhData->value(pSourceId);
        tdhData->operator[](pSourceId) = tdhData->value(pDestinationId);
        tdhData->operator[](pDestinationId) = qsDataTemp;
    } // for

    VocabularyDatabase::swapFields(pSourceId, pDestinationId);
} // swapFields
#endif

Vocabulary::Vocabulary()
{
    _trdhRecordData = NULL;
} // Vocabulary