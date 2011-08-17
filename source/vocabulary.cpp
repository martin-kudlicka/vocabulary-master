#include "vocabulary.h"

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
} // AddRecord
#endif

const void Vocabulary::ClearCache()
{
	_qhCategoryRecords.clear();
} // ClearCache

const QString Vocabulary::GetDataText(const int &pCategoryId, const int &pRow, const int &pFieldId) const
{
    int iRecordId = _qhCategoryRecords.value(pCategoryId).at(pRow);
    return GetDataText(iRecordId, pFieldId);
} // GetDataText

const QString Vocabulary::GetDataText(const int &pRecordId, const int &pFieldId) const
{
    return VocabularyDatabase::GetDataText(pRecordId, pFieldId);
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
	_qhCategoryRecords.remove(pCategoryId);
	VocabularyDatabase::RemoveCategory(pCategoryId);
} // RemoveCategory

const void Vocabulary::RemoveRecord(const int &pCategoryId, const int &pRow)
{
	VocabularyDatabase::RemoveRecord(pCategoryId, pRow);
	_qhCategoryRecords[pCategoryId].removeAt(pRow);
} // RemoveRecord