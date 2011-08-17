#include "vocabulary.h"

#include <QtCore/QStringList>
#include "vocabulary/clearcacheworker.h"
#include <QtCore/QThreadPool>

const int Vocabulary::AddCategory(const QString &pName)
{
	int iCategoryId = VocabularyDatabase::AddCategory(pName);
	_qhCategoryRecords.insert(iCategoryId, tRecordIdList());
	return iCategoryId;
} // AddCategory

const void Vocabulary::AddRecord(const int &pCategoryId)
{
	int iRecordId = VocabularyDatabase::AddRecord(pCategoryId);
	_qhCategoryRecords[pCategoryId].append(iRecordId);
} // AddRecord

#ifndef FREE
const void Vocabulary::AddRecord(const int &pCategoryId, const QStringList &pData)
{
	int iRecordId = VocabularyDatabase::AddRecord(pCategoryId, pData);
	_qhCategoryRecords[pCategoryId].append(iRecordId);

    // insert data to cache
    int iData = 0;
    tFieldDataHash *tfdhFieldData = &_trdhRecordData->operator[](iRecordId);
    foreach (int iFieldId, GetFieldIds()) {
        tfdhFieldData->insert(iFieldId, pData.at(iData));
        iData++;
    } // foreach
} // AddRecord
#endif

const void Vocabulary::ClearCache()
{
	_qhCategoryRecords.clear();

    // clear large cache in background
    if (_trdhRecordData) {
        ClearCacheWorker *ccwClearCacheWorker = new ClearCacheWorker(_trdhRecordData);
        QThreadPool::globalInstance()->start(ccwClearCacheWorker);
        _trdhRecordData = NULL;
    } // if
} // ClearCache

const QString Vocabulary::GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const
{
    int iRecordId = _qhCategoryRecords.value(pCategoryId).at(pRow);
    return GetDataText(iRecordId, pFieldId);
} // GetDataText

const QString Vocabulary::GetDataText(const int &pRecordId, const int &pFieldId) const
{
    return _trdhRecordData->value(pRecordId).value(pFieldId);
} // GetDataText

const int Vocabulary::GetRecordCount() const
{
	int iRecordCount = 0;
	QList<tRecordIdList> qlAllRecordIds = _qhCategoryRecords.values();
	foreach (tRecordIdList trilRecordIds, qlAllRecordIds) {
		iRecordCount += trilRecordIds.size();
	} // foreach

	return iRecordCount;
} // GetRecordCount

const int Vocabulary::GetRecordCount(const int &pCategoryId) const
{
	return _qhCategoryRecords.value(pCategoryId).size();
} // GetRecordCount

const int Vocabulary::GetRecordCount(const bool &pEnabled) const
{
	int iRecordCount = 0;
	tCategoryIdList tcilCategoryIds = GetCategoryIds();
	foreach (int iCategoryId, tcilCategoryIds) {
		if (GetCategoryEnabled(iCategoryId)) {
			iRecordCount += GetRecordCount(iCategoryId);
		} // if
	} // foreach

	return iRecordCount;
} // GetRecordCount

const void Vocabulary::InitCache()
{
	if (IsOpen()) {
		// categories
		tCategoryIdList tcilCategoryIds = GetCategoryIds();
		foreach (int iCategoryId, tcilCategoryIds) {
			tRecordIdList tdilRecordIds = GetRecordIds(iCategoryId);
			_qhCategoryRecords.insert(iCategoryId, tdilRecordIds);
		} // foreach

        // records
        _trdhRecordData = new tRecordDataHash();
        _trdhRecordData = VocabularyDatabase::GetDataText();
	} // if
} // InitCache

const void Vocabulary::New(const QString &pFilePath)
{
	ClearCache();
	VocabularyDatabase::New(pFilePath);
	InitCache();
} // New

const void Vocabulary::Open(const QString &pFilePath)
{
	ClearCache();
	VocabularyDatabase::Open(pFilePath);
	InitCache();
} // Open

const void Vocabulary::RemoveCategory(const int &pCategoryId)
{
    tRecordIdList trilRecords = _qhCategoryRecords.value(pCategoryId);
    foreach (int iRecordId, trilRecords) {
        _trdhRecordData->remove(iRecordId);
    } // foreach
	_qhCategoryRecords.remove(pCategoryId);

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
    _trdhRecordData->remove(_qhCategoryRecords.value(pCategoryId).at(pRow));
    _qhCategoryRecords[pCategoryId].removeAt(pRow);

	VocabularyDatabase::RemoveRecord(pCategoryId, pRow);
} // RemoveRecord

const void Vocabulary::SetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId, const QString &pData)
{
    int iRecordId = _qhCategoryRecords.value(pCategoryId).at(pRow);
    SetDataText(iRecordId, pFieldId, pData);
} // SetDataText

const void Vocabulary::SetDataText(const int &pRecordId, const int &pFieldId, const QString &pData)
{
    _trdhRecordData->operator[](pRecordId).operator[](pFieldId) = pData;
    VocabularyDatabase::SetDataText(pRecordId, pFieldId, pData);
} // SetDataText

Vocabulary::Vocabulary()
{
    _trdhRecordData = NULL;
} // Vocabulary