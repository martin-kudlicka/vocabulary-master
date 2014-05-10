#include "vocabularyorganizer.h"

#ifndef EDITION_TRY
# include "common/vocabularyopenprogressdialog.h"

const void VocabularyOrganizer::AddExisting(sVocabulary &pVocabulary, QWidget *pParent)
{
	pVocabulary.vVocabulary = new Vocabulary;

# ifndef EDITION_FREE
	if (pVocabulary.sviVocabularyInfo.bEnabled) {
# endif
		Open(&pVocabulary, pParent);
# ifndef EDITION_FREE
	} // if
# endif

	_qlVocabularies.append(pVocabulary);
} // AddExisting
#endif

const void VocabularyOrganizer::AddNew(
#ifndef EDITION_TRY
	const QString &pFile
#endif
	)
{
	sVocabulary svVocabulary;
#ifndef EDITION_TRY
	svVocabulary.sviVocabularyInfo.qsFile = pFile;
#endif
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	svVocabulary.sviVocabularyInfo.bEnabled = true;
#endif
	svVocabulary.vVocabulary = new Vocabulary;
	svVocabulary.vVocabulary->New(
#ifndef EDITION_TRY
		pFile
#endif
		);

	_qlVocabularies.append(svVocabulary);
} // AddNew

const int VocabularyOrganizer::GetRecordCount() const
{
	int iCount = 0;

	foreach (sVocabulary svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->IsOpen()) {
			iCount += svVocabulary.vVocabulary->GetRecordCount();
		} // if
	} // foreach

	return iCount;
} // GetRecordCount

const int VocabularyOrganizer::GetRecordCount(const bool &pEnabled) const
{
	int iCount = 0;

	foreach (sVocabulary svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->IsOpen()) {
			iCount += svVocabulary.vVocabulary->GetRecordCount(pEnabled);
		} // if
	} // foreach

	return iCount;
} // GetRecordCount

const VocabularyOrganizer::sRecordInfo VocabularyOrganizer::GetRecordInfo(const int &pRow) const
{
	int iCurrentRow = 0;
	sRecordInfo sriRecordInfo;

	foreach (sVocabulary svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->IsOpen()) {
			int iRecords = svVocabulary.vVocabulary->GetRecordCount();
			if (iCurrentRow + iRecords > pRow) {
				sriRecordInfo.vVocabulary = svVocabulary.vVocabulary;
				sriRecordInfo.iId = svVocabulary.vVocabulary->GetRecordId(pRow - iCurrentRow);

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

const VocabularyOrganizer::sVocabulary &VocabularyOrganizer::GetVocabularyInfo(const int &pIndex) const
{
	return _qlVocabularies.at(pIndex);
} // GetVocabularyInfo

const bool VocabularyOrganizer::IsOpen() const
{
	foreach (sVocabulary svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->IsOpen()) {
			return true;
		} // if
	} // foreach

	return false;
} // IsOpen

#ifndef EDITION_TRY
const void VocabularyOrganizer::Open(sVocabulary *pVocabulary, QWidget *pParent)
{
	VocabularyOpenProgressDialog vopdOpenProgress(pVocabulary->vVocabulary, pParent);
	vopdOpenProgress.show();
	pVocabulary->vVocabulary->Open(pVocabulary->sviVocabularyInfo.qsFile);

# ifndef EDITION_FREE
	if (!pVocabulary->vVocabulary->IsOpen()) {
		pVocabulary->sviVocabularyInfo.bEnabled = false;
	} // if
#endif
} // Open

const void VocabularyOrganizer::OpenAll(QWidget *pParent)
{
	int iVocabularies = _sSettings->GetVocabularyCount();
	for (int iI = 0; iI < iVocabularies; iI++) {
		sVocabulary svVocabulary;
		svVocabulary.sviVocabularyInfo = _sSettings->GetVocabularyInfo(iI);
		AddExisting(svVocabulary, pParent);
	} // for
} // OpenAll
#endif

const void VocabularyOrganizer::Remove(const int &pIndex)
{
	Vocabulary *vVocabulary = _qlVocabularies.at(pIndex).vVocabulary;

	_qlVocabularies.removeAt(pIndex);
	emit VocabularyClose(vVocabulary);

	vVocabulary->Close();
	delete vVocabulary;
} // Remove

#ifndef EDITION_TRY
const void VocabularyOrganizer::SaveAll()
{
	int iVocabularies = _qlVocabularies.size();

	for (int iI = 0; iI < iVocabularies; iI++) {
		_sSettings->SetVocabularyInfo(iI, _qlVocabularies.at(iI).sviVocabularyInfo);
	} // for
	_sSettings->SetVocabularyCount(iVocabularies);
} // SaveAll

# ifndef EDITION_FREE
const void VocabularyOrganizer::SetVocabularyEnabled(const int &pIndex, const bool &pEnabled, QWidget *pParent)
{
	sVocabulary *svVocabulary = &_qlVocabularies[pIndex];
	svVocabulary->sviVocabularyInfo.bEnabled = pEnabled;

	if (pEnabled) {
		Open(svVocabulary, pParent);
	} else {
		emit VocabularyClose(svVocabulary->vVocabulary);
		svVocabulary->vVocabulary->Close();
	} // if else
} // SetVocabularyEnabled
# endif
#endif

VocabularyOrganizer::VocabularyOrganizer(Settings *pSettings)
{
	_sSettings = pSettings;	
} // VocabularyOrganizer