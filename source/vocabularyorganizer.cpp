#include "vocabularyorganizer.h"

#ifndef EDITION_TRY
# include "common/vocabularyopenprogressdialog.h"

const void VocabularyOrganizer::AddExisting(VocabularyInfo &pVocabulary, QWidget *pParent)
{
	pVocabulary.vVocabulary = new Vocabulary;

# ifndef EDITION_FREE
	if (pVocabulary.sviVocabularyInfo.enabled) {
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
	VocabularyInfo svVocabulary;
#ifndef EDITION_TRY
	svVocabulary.sviVocabularyInfo.filePath = pFile;
#endif
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	svVocabulary.sviVocabularyInfo.enabled = true;
#endif
	svVocabulary.vVocabulary = new Vocabulary;
	svVocabulary.vVocabulary->new2(
#ifndef EDITION_TRY
		pFile
#endif
		);

	_qlVocabularies.append(svVocabulary);
} // AddNew

const int VocabularyOrganizer::GetRecordCount() const
{
	int iCount = 0;

	foreach (VocabularyInfo svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->isOpen()) {
			iCount += svVocabulary.vVocabulary->recordCount();
		} // if
	} // foreach

	return iCount;
} // GetRecordCount

const int VocabularyOrganizer::GetRecordCount(const bool &pEnabled) const
{
	int iCount = 0;

	foreach (VocabularyInfo svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->isOpen()) {
			iCount += svVocabulary.vVocabulary->recordCount(pEnabled);
		} // if
	} // foreach

	return iCount;
} // GetRecordCount

const VocabularyOrganizer::RecordInfo VocabularyOrganizer::GetRecordInfo(const int &pRow) const
{
	int iCurrentRow = 0;
	RecordInfo sriRecordInfo;

	foreach (VocabularyInfo svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->isOpen()) {
			int iRecords = svVocabulary.vVocabulary->recordCount();
			if (iCurrentRow + iRecords > pRow) {
				sriRecordInfo.vVocabulary = svVocabulary.vVocabulary;
				sriRecordInfo.iId = svVocabulary.vVocabulary->recordId(pRow - iCurrentRow);

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

const VocabularyOrganizer::VocabularyInfo &VocabularyOrganizer::GetVocabularyInfo(const int &pIndex) const
{
	return _qlVocabularies.at(pIndex);
} // GetVocabularyInfo

const bool VocabularyOrganizer::IsOpen() const
{
	foreach (VocabularyInfo svVocabulary, _qlVocabularies) {
		if (svVocabulary.vVocabulary->isOpen()) {
			return true;
		} // if
	} // foreach

	return false;
} // IsOpen

#ifndef EDITION_TRY
const void VocabularyOrganizer::Open(VocabularyInfo *pVocabulary, QWidget *pParent)
{
	VocabularyOpenProgressDialog vopdOpenProgress(pVocabulary->vVocabulary, pParent);
	vopdOpenProgress.show();
	pVocabulary->vVocabulary->open(pVocabulary->sviVocabularyInfo.filePath);

# ifndef EDITION_FREE
	if (!pVocabulary->vVocabulary->isOpen()) {
		pVocabulary->sviVocabularyInfo.enabled = false;
	} // if
#endif
} // Open

const void VocabularyOrganizer::OpenAll(QWidget *pParent)
{
	int iVocabularies = _sSettings->vocabularyCount();
	for (int iI = 0; iI < iVocabularies; iI++) {
		VocabularyInfo svVocabulary;
		svVocabulary.sviVocabularyInfo = _sSettings->vocabularyInfo(iI);
		AddExisting(svVocabulary, pParent);
	} // for
} // OpenAll
#endif

const void VocabularyOrganizer::Remove(const int &pIndex)
{
	Vocabulary *vVocabulary = _qlVocabularies.at(pIndex).vVocabulary;

	_qlVocabularies.removeAt(pIndex);
	emit VocabularyClose(vVocabulary);

	vVocabulary->close();
	delete vVocabulary;
} // Remove

#ifndef EDITION_TRY
const void VocabularyOrganizer::SaveAll()
{
	int iVocabularies = _qlVocabularies.size();

	for (int iI = 0; iI < iVocabularies; iI++) {
		_sSettings->setVocabularyInfo(iI, _qlVocabularies.at(iI).sviVocabularyInfo);
	} // for
	_sSettings->setVocabularyCount(iVocabularies);
} // SaveAll

# ifndef EDITION_FREE
const void VocabularyOrganizer::SetVocabularyEnabled(const int &pIndex, const bool &pEnabled, QWidget *pParent)
{
	VocabularyInfo *svVocabulary = &_qlVocabularies[pIndex];
	svVocabulary->sviVocabularyInfo.enabled = pEnabled;

	if (pEnabled) {
		Open(svVocabulary, pParent);
	} else {
		emit VocabularyClose(svVocabulary->vVocabulary);
		svVocabulary->vVocabulary->close();
	} // if else
} // SetVocabularyEnabled
# endif
#endif

VocabularyOrganizer::VocabularyOrganizer(Settings *pSettings)
{
	_sSettings = pSettings;	
} // VocabularyOrganizer