#include "vocabulary.h"

const int Vocabulary::AddCategory(const QString &pName)
{
	int iCategoryId = VocabularyDatabase::AddCategory(pName);
	_qhCategoryRecordCount.insert(iCategoryId, 0);
	return iCategoryId;
} // AddCategory

const void Vocabulary::AddRecord(const int &pCategoryId)
{
	VocabularyDatabase::AddRecord(pCategoryId);
	_qhCategoryRecordCount[pCategoryId]++;
} // AddRecord

#ifndef FREE
const void Vocabulary::AddRecord(const int &pCategoryId, const QStringList &pData)
{
	VocabularyDatabase::AddRecord(pCategoryId, pData);
	_qhCategoryRecordCount[pCategoryId]++;
} // AddRecord
#endif

const void Vocabulary::ClearCache()
{
	_qhCategoryRecordCount.clear();
} // ClearCache

const int Vocabulary::GetRecordCount() const
{
	int iRecordCount = 0;
	QList<int> qlRecords = _qhCategoryRecordCount.values();
	foreach (int iRecords, qlRecords) {
		iRecordCount += iRecords;
	} // foreach

	return iRecordCount;
} // GetRecordCount

const int Vocabulary::GetRecordCount(const int &pCategoryId) const
{
	return _qhCategoryRecordCount.value(pCategoryId);
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
			int iRecordCount = VocabularyDatabase::GetRecordCount(iCategoryId);
			_qhCategoryRecordCount.insert(iCategoryId, iRecordCount);
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
	_qhCategoryRecordCount.remove(pCategoryId);
	VocabularyDatabase::RemoveCategory(pCategoryId);
} // RemoveCategory

const void Vocabulary::RemoveRecord(const int &pCategoryId, const int &pRow)
{
	VocabularyDatabase::RemoveRecord(pCategoryId, pRow);
	_qhCategoryRecordCount[pCategoryId]--;
} // RemoveRecord