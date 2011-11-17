#include "vocabularyorganizer.h"

#ifndef TRY
# include "common/vocabularyopenprogressdialog.h"
#endif

const int VocabularyOrganizer::GetRecordCount() const
{
	int iCount = 0;

	foreach (const Vocabulary *vVocabulary, _qlVocabularies) {
		if (vVocabulary->IsOpen()) {
			iCount += vVocabulary->GetRecordCount();
		} // if
	} // foreach

	return iCount;
} // GetRecordCount

const int VocabularyOrganizer::GetRecordCount(const bool &pEnabled) const
{
	int iCount = 0;

	foreach (const Vocabulary *vVocabulary, _qlVocabularies) {
		if (vVocabulary->IsOpen()) {
			iCount += vVocabulary->GetRecordCount(pEnabled);
		} // if
	} // foreach

	return iCount;
} // GetRecordCount

const VocabularyOrganizer::sRecordInfo VocabularyOrganizer::GetRecordInfo(const int &pRow) const
{
	int iCurrentRow = 0;
	sRecordInfo sriRecordInfo;

	foreach (Vocabulary *vVocabulary, _qlVocabularies) {
		if (vVocabulary->IsOpen()) {
			int iRecords = vVocabulary->GetRecordCount();
			if (iCurrentRow + iRecords > pRow) {
				sriRecordInfo.vVocabulary = vVocabulary;
				sriRecordInfo.iId = vVocabulary->GetRecordId(pRow - iCurrentRow);

				return sriRecordInfo;
			} else {
				iCurrentRow += iRecords;
			} // if else
		} // if
	} // foreach

	sriRecordInfo.vVocabulary = NULL;
	sriRecordInfo.iId = VocabularyDatabase::NOT_FOUND;
	return sriRecordInfo;
} // GetRecordInfo

const int VocabularyOrganizer::GetVocabularyCount() const
{
	return _qlVocabularies.size();
} // GetVocabularyCount

const Vocabulary *VocabularyOrganizer::GetVocabularyInfo(const int &pIndex) const
{
	return _qlVocabularies.at(pIndex);
} // GetVocabularyInfo

const bool VocabularyOrganizer::IsOpen() const
{
	foreach (const Vocabulary *vVocabulary, _qlVocabularies) {
		if (vVocabulary->IsOpen()) {
			return true;
		} // if
	} // foreach

	return false;
} // IsOpen

const void VocabularyOrganizer::New(
#ifndef TRY
	const QString &pFile
#endif
	)
{
	Vocabulary *vVocabulary = new Vocabulary;
	vVocabulary->New(
#ifndef TRY
		pFile
#endif
		);
	_qlVocabularies.append(vVocabulary);
} // New

#ifndef TRY
const void VocabularyOrganizer::Open(const QString &pFile, QWidget *pParent)
{
	Vocabulary *vVocabulary = new Vocabulary;

	VocabularyOpenProgressDialog vopdOpenProgress(vVocabulary, pParent);
	vopdOpenProgress.show();
	vVocabulary->Open(pFile);

	_qlVocabularies.append(vVocabulary);
} // Open

const void VocabularyOrganizer::OpenAll(QWidget *pParent)
{
	int iVocabularies = _sSettings->GetVocabularyCount();
	for (int iI = 0; iI < iVocabularies; iI++) {
		Vocabulary *vVocabulary = new Vocabulary;

		VocabularyOpenProgressDialog vopdOpenProgress(vVocabulary, pParent);
		vopdOpenProgress.show();
		vVocabulary->Open(_sSettings->GetVocabularyFile(iI));

		_qlVocabularies.append(vVocabulary);
	} // for
} // OpenAll
#endif

VocabularyOrganizer::VocabularyOrganizer(Settings *pSettings)
{
	_sSettings = pSettings;	
} // VocabularyOrganizer