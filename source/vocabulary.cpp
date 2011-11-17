#include "vocabulary.h"

#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"

const int Vocabulary::AddCategory(const QString &pName)
{
	int iCategoryId = VocabularyDatabase::AddCategory(pName);
	_tcrmCategoryRecords.insert(iCategoryId, tRecordIdList());
	return iCategoryId;
} // AddCategory

#ifndef FREE
const void Vocabulary::AddField()
{
    int iFieldId = VocabularyDatabase::AddField();

    sFieldData sfdFieldData = GetFieldData(iFieldId);
    _tfdmFieldData.insert(iFieldId, sfdFieldData);
} // AddField
#endif

const void Vocabulary::AddRecord(const int &pCategoryId)
{
	int iRecordId = VocabularyDatabase::AddRecord(pCategoryId);
	_tcrmCategoryRecords[pCategoryId].append(iRecordId);
} // AddRecord

#ifndef FREE
const void Vocabulary::AddRecord(const int &pCategoryId, const QStringList &pData)
{
	int iRecordId = VocabularyDatabase::AddRecord(pCategoryId, pData);
	/*_tcrmCategoryRecords[pCategoryId].append(iRecordId);

    // insert data to cache
    int iData = 0;
    tFieldDataHash *tfdhFieldData = &_trdhRecordData->operator[](iRecordId);
    foreach (int iFieldId, GetFieldIds()) {
        tfdhFieldData->insert(iFieldId, pData.at(iData));
        iData++;
    } // foreach*/
} // AddRecord
#endif

const void Vocabulary::BeginEdit()
{
    _tfdmFieldDataBackup = _tfdmFieldData;
    _trdhRecordDataBackup = *_trdhRecordData;

    VocabularyDatabase::BeginEdit();
} // BeginEdit

const void Vocabulary::Close()
{
	ClearCache();
	VocabularyDatabase::Close();
} // Close

const void Vocabulary::EndEdit(const bool &pSave /* true */)
{
    if (!pSave) {
        _tfdmFieldData = _tfdmFieldDataBackup;
        *_trdhRecordData = _trdhRecordDataBackup;
    } // if

    VocabularyDatabase::EndEdit(pSave);
} // EndEdit

const void Vocabulary::ClearCache()
{
	_tcrmCategoryRecords.clear();
	_tfdmFieldData.clear();

    // clear large cache in background
    if (_trdhRecordData) {
        ClearCacheWorker *ccwClearCacheWorker = new ClearCacheWorker(_trdhRecordData, this);
        ccwClearCacheWorker->start(QThread::LowPriority);
        _trdhRecordData = NULL;
    } // if
} // ClearCache

const bool Vocabulary::FieldHasAttribute(const int &pFieldId, const eFieldAttribute &pAttribute) const
{
	qfFieldAttributes qfaAttributes = GetFieldAttributes(pFieldId);
	return qfaAttributes & pAttribute;
} // FieldHasAttribute

const VocabularyDatabase::tCategoryIdList Vocabulary::GetCategoryIds() const
{
    return _tcrmCategoryRecords.keys();
} // GetCategoryIds

const QString Vocabulary::GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const
{
    int iRecordId = _tcrmCategoryRecords.value(pCategoryId).at(pRow);
    return GetDataText(iRecordId, pFieldId);
} // GetDataText

const QString Vocabulary::GetDataText(const int &pRecordId, const int &pFieldId) const
{
    return _trdhRecordData->value(pRecordId).value(pFieldId);
} // GetDataText

const VocabularyDatabase::qfFieldAttributes Vocabulary::GetFieldAttributes(const int &pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).qfaAttributes;
} // GetFieldAttributes

#ifndef FREE
const VocabularyDatabase::eFieldBuiltIn Vocabulary::GetFieldBuiltIn(const int &pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).efbBuiltIn;
} // GetFieldBuiltIn
#endif

const int Vocabulary::GetFieldCount() const
{
    return _tfdmFieldData.size();
} // GetFieldCount

const Vocabulary::sFieldData Vocabulary::GetFieldData(const int &pFieldId) const
{
    sFieldData sfdFieldData;

    sfdFieldData.qsTemplateName = VocabularyDatabase::GetFieldTemplateName(pFieldId);
    sfdFieldData.qsName = VocabularyDatabase::GetFieldName(pFieldId);
	sfdFieldData.eftType = VocabularyDatabase::GetFieldType(pFieldId);
	sfdFieldData.qfaAttributes = VocabularyDatabase::GetFieldAttributes(pFieldId);
#ifndef FREE
    sfdFieldData.efbBuiltIn = VocabularyDatabase::GetFieldBuiltIn(pFieldId);
#endif
    sfdFieldData.eflLanguage = VocabularyDatabase::GetFieldLanguage(pFieldId);

    return sfdFieldData;
} // GetFieldData

const int Vocabulary::GetFieldId(const int &pPosition) const
{
    int iPos = 0;
    for (tFieldDataMap::const_iterator ciFieldId = _tfdmFieldData.constBegin(); ciFieldId != _tfdmFieldData.constEnd(); ciFieldId++) {
        if (iPos == pPosition) {
            return ciFieldId.key();
        } else {
            iPos++;
        } // if else
    } // for

    return NOT_FOUND;
} // GetFieldId

const VocabularyDatabase::tFieldIdList Vocabulary::GetFieldIds() const
{
    return _tfdmFieldData.keys();
} // GetFieldIds

const VocabularyDatabase::eFieldLanguage Vocabulary::GetFieldLanguage(const int &pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).eflLanguage;
} // GetFieldLanguage

const QString Vocabulary::GetFieldName(const int &pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).qsName;
} // GetFieldName

const QString Vocabulary::GetFieldTemplateName(const int &pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).qsTemplateName;
} // GetFieldTemplateName

const VocabularyDatabase::eFieldType Vocabulary::GetFieldType(const int &pFieldId) const
{
    return _tfdmFieldData.value(pFieldId).eftType;
} // GetFieldType

#ifndef FREE
const QStringList Vocabulary::GetRecord(const int &pRecordId) const
{
    QStringList qslData;

    tFieldIdList tfilFieldIds = GetFieldIds();
    foreach (int iFieldId, tfilFieldIds) {
        qslData.append(GetDataText(pRecordId, iFieldId));
    } // foreach

    return qslData;
} // GetRecord
#endif

const int Vocabulary::GetRecordCategory(const int &pRecordId) const
{
    for (tCategoryRecordsMap::const_iterator ciCategory = _tcrmCategoryRecords.constBegin(); ciCategory != _tcrmCategoryRecords.constEnd(); ciCategory++) {
        if (ciCategory->contains(pRecordId)) {
            return ciCategory.key();
        } // if
    } // for

    return NOT_FOUND;
} // GetRecordCategory

const int Vocabulary::GetRecordCount() const
{
	int iRecordCount = 0;
	QList<tRecordIdList> qlAllRecordIds = _tcrmCategoryRecords.values();
	foreach (tRecordIdList trilRecordIds, qlAllRecordIds) {
		iRecordCount += trilRecordIds.size();
	} // foreach

	return iRecordCount;
} // GetRecordCount

const int Vocabulary::GetRecordCount(const int &pCategoryId) const
{
	return _tcrmCategoryRecords.value(pCategoryId).size();
} // GetRecordCount

#ifndef FREE
const int Vocabulary::GetRecordCount(const int &pCategoryId, const bool &pEnabled) const
{
	int iRecordCount = 0;

	tRecordIdList trilRecordIds = _tcrmCategoryRecords.value(pCategoryId);
	foreach (int iRecordId, trilRecordIds) {
		if (GetRecordEnabled(iRecordId)) {
			iRecordCount++;
		} // if
	} // foreach

	return iRecordCount;
} // GetRecordCount

const int Vocabulary::GetRecordCount(const bool &pEnabled) const
{
	int iRecordCount = 0;

	tCategoryIdList tcilCategoryIds = GetCategoryIds();
	foreach (int iCategoryId, tcilCategoryIds) {
		if (GetCategoryEnabled(iCategoryId)) {
			iRecordCount += GetRecordCount(iCategoryId, pEnabled);
		} // if
	} // foreach

	return iRecordCount;
} // GetRecordCount

const bool Vocabulary::GetRecordEnabled(const int &pRecordId) const
{
	foreach (int iFieldId, GetFieldIds()) {
		if (FieldHasAttribute(iFieldId, FieldAttributeBuiltIn)) {
			eFieldBuiltIn efbBuiltIn = GetFieldBuiltIn(iFieldId);
			switch (efbBuiltIn) {
				case VocabularyDatabase::FieldBuiltInEnabled:
					QString qsData = GetDataText(pRecordId, iFieldId);
					if (qsData.isNull()) {
						return true;
					} else {
						return qsData.toInt();
					} // if else
			} // switch
		} // if
	} // foreach

	return true;
} // GetRecordEnabled
#endif

const int Vocabulary::GetRecordId(const int &pRow) const
{
    int iRecordsTotal = 0;
    for (tCategoryRecordsMap::const_iterator ciCategory = _tcrmCategoryRecords.constBegin(); ciCategory != _tcrmCategoryRecords.constEnd(); ciCategory++) {
        int iRecords = iRecordsTotal + ciCategory->size();
        if (pRow < iRecords) {
            return ciCategory->at(pRow - iRecordsTotal);
        } else {
            iRecordsTotal = iRecords;
        } // if else
    } // for

    return NOT_FOUND;
} // GetRecordId

const int Vocabulary::GetRecordId(const int &pCategoryId, const int &pRow) const
{
    if (pRow == NOT_FOUND) {
        return NOT_FOUND;
    } else {
        return _tcrmCategoryRecords.value(pCategoryId).at(pRow);
    } // if else
} // GetRecordId

const VocabularyDatabase::tRecordIdList Vocabulary::GetRecordIds(const int &pCategoryId) const
{
    return _tcrmCategoryRecords.value(pCategoryId);
} // GetRecordIds

const void Vocabulary::InitCache()
{
	if (IsOpen()) {
        // fields
        tFieldIdList tfilFieldIds = VocabularyDatabase::GetFieldIds();
        foreach (int iFieldId, tfilFieldIds) {
            sFieldData sfdFieldData = GetFieldData(iFieldId);
            _tfdmFieldData.insert(iFieldId, sfdFieldData);
        } // foreach

		// categories
		tCategoryIdList tcilCategoryIds = VocabularyDatabase::GetCategoryIds();
		foreach (int iCategoryId, tcilCategoryIds) {
            tRecordIdList tdilRecordIds = VocabularyDatabase::GetRecordIds(iCategoryId);
			_tcrmCategoryRecords.insert(iCategoryId, tdilRecordIds);
		} // foreach

        // records
        _trdhRecordData = new tRecordDataHash();
        _trdhRecordData = VocabularyDatabase::GetDataText();
	} // if
} // InitCache

const void Vocabulary::New(const QString &pFilePath)
{
	VocabularyDatabase::New(pFilePath);
	InitCache();
} // New

#ifndef TRY
const void Vocabulary::Open(const QString &pFilePath)
{
	VocabularyDatabase::Open(pFilePath);
	InitCache();
} // Open
#else

const void Vocabulary::OpenMemory()
{
    InitCache();
} // OpenMemory
#endif

const void Vocabulary::RemoveCategory(const int &pCategoryId)
{
    tRecordIdList trilRecords = _tcrmCategoryRecords.value(pCategoryId);
    foreach (int iRecordId, trilRecords) {
        _trdhRecordData->remove(iRecordId);
    } // foreach
	_tcrmCategoryRecords.remove(pCategoryId);

	VocabularyDatabase::RemoveCategory(pCategoryId);
} // RemoveCategory

#ifndef FREE
const void Vocabulary::RemoveField(const int &pFieldId)
{
    for (tRecordDataHash::iterator iFieldData = _trdhRecordData->begin(); iFieldData != _trdhRecordData->end(); iFieldData++) {
        iFieldData->remove(pFieldId);
    } // for

    VocabularyDatabase::RemoveField(pFieldId);
} // RemoveField
#endif

const void Vocabulary::RemoveRecord(const int &pCategoryId, const int &pRow)
{
    _trdhRecordData->remove(_tcrmCategoryRecords.value(pCategoryId).at(pRow));
    _tcrmCategoryRecords[pCategoryId].removeAt(pRow);

	VocabularyDatabase::RemoveRecord(pCategoryId, pRow);
} // RemoveRecord

const void Vocabulary::SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData)
{
    int iRecordId = _tcrmCategoryRecords.value(pCategoryId).at(pRow);
    SetDataText(iRecordId, pFieldId, pData);
} // SetDataText

const void Vocabulary::SetDataText(const int &pRecordId, const int &pFieldId, const QString &pData)
{
    _trdhRecordData->operator[](pRecordId).operator[](pFieldId) = pData;
    VocabularyDatabase::SetDataText(pRecordId, pFieldId, pData);
} // SetDataText

#ifndef FREE
const void Vocabulary::SetFieldAttributes(const int &pFieldId, const VocabularyDatabase::qfFieldAttributes &pAttributes)
{
    _tfdmFieldData[pFieldId].qfaAttributes = pAttributes;
    VocabularyDatabase::SetFieldAttributes(pFieldId, pAttributes);
} // SetFieldAttributes

const void Vocabulary::SetFieldLanguage(const int &pFieldId, const VocabularyDatabase::eFieldLanguage &pLanguage)
{
    _tfdmFieldData[pFieldId].eflLanguage = pLanguage;
    VocabularyDatabase::SetFieldLanguage(pFieldId, pLanguage);
} // SetFieldLanguage

const void Vocabulary::SetFieldName(const int &pFieldId, const QString &pName)
{
    _tfdmFieldData[pFieldId].qsName = pName;
    VocabularyDatabase::SetFieldName(pFieldId, pName);
} // SetFieldName

const void Vocabulary::SetFieldTemplateName(const int &pFieldId, const QString &pTemplateName)
{
    _tfdmFieldData[pFieldId].qsTemplateName = pTemplateName;
    VocabularyDatabase::SetFieldTemplateName(pFieldId, pTemplateName);
} // SetFieldTemplateName

const void Vocabulary::SetRecordByRowCategory(const int &pOldCategoryId, const int &pRecordRow, const int &pNewCategoryId)
{
    int iRecordId = _tcrmCategoryRecords[pOldCategoryId].takeAt(pRecordRow);
    tRecordIdList *trilRecordIds = &_tcrmCategoryRecords[pNewCategoryId];
    tRecordIdList::iterator iLowerBound = qLowerBound(trilRecordIds->begin(), trilRecordIds->end(), iRecordId);
    trilRecordIds->insert(iLowerBound, iRecordId);

    VocabularyDatabase::SetRecordCategory(iRecordId, pNewCategoryId);
} // SetRecordByRowCategory

const void Vocabulary::SwapFields(const int &pSourceId, const int &pDestinationId)
{
    // swap in fields table
    sFieldData sfdFieldTemp = _tfdmFieldData.value(pSourceId);
    _tfdmFieldData[pSourceId] = _tfdmFieldData.value(pDestinationId);
    _tfdmFieldData[pDestinationId] = sfdFieldTemp;

    // swap in data table
    for (tRecordDataHash::iterator iRecord = _trdhRecordData->begin(); iRecord != _trdhRecordData->end(); iRecord++) {
        tDataHash *tdhData = iRecord.operator->();

        QString qsDataTemp = tdhData->value(pSourceId);
        tdhData->operator[](pSourceId) = tdhData->value(pDestinationId);
        tdhData->operator[](pDestinationId) = qsDataTemp;
    } // for

    VocabularyDatabase::SwapFields(pSourceId, pDestinationId);
} // SwapFields
#endif

Vocabulary::Vocabulary()
{
    _trdhRecordData = NULL;
} // Vocabulary