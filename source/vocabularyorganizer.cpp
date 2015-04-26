#include "vocabularyorganizer.h"

#ifndef EDITION_TRY
# include "common/vocabularyopenprogressdialog.h"
#endif

VocabularyOrganizer::VocabularyOrganizer(Settings *settings) : _settings(settings)
{
} // VocabularyOrganizer

VocabularyOrganizer::~VocabularyOrganizer()
{
} // ~VocabularyOrganizer

#ifndef EDITION_TRY
void VocabularyOrganizer::addExisting(VocabularyInfo *vocabularyInfo, QWidget *parent)
{
	vocabularyInfo->vocabulary = new Vocabulary(_settings);

# ifndef EDITION_FREE
	if (vocabularyInfo->vocabularyInfo.enabled)
	{
# endif
		open(vocabularyInfo, parent);
# ifndef EDITION_FREE
	} // if
# endif

	_vocabularies.append(*vocabularyInfo);
} // addExisting
#endif

void VocabularyOrganizer::addNew(
#ifndef EDITION_TRY
	const QString &file
#endif
	)
{
	VocabularyInfo vocabularyInfo;
#ifndef EDITION_TRY
	vocabularyInfo.vocabularyInfo.filePath = file;
#endif
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	vocabularyInfo.vocabularyInfo.enabled  = true;
#endif
	vocabularyInfo.vocabulary              = new Vocabulary(_settings);
	vocabularyInfo.vocabulary->new2(
#ifndef EDITION_TRY
		file
#endif
		);

	_vocabularies.append(vocabularyInfo);
} // addNew

bool VocabularyOrganizer::isOpen() const
{
	for (const VocabularyInfo &vocabularyInfo : _vocabularies)
	{
		if (vocabularyInfo.vocabulary->isOpen())
		{
			return true;
		} // if
	} // for

	return false;
} // isOpen

#ifndef EDITION_TRY
void VocabularyOrganizer::openAll(QWidget *parent)
{
	const quint8 vocabularies = _settings->vocabularyCount();
	for (quint8 index = 0; index < vocabularies; index++)
	{
		VocabularyInfo vocabularyInfo;
		vocabularyInfo.vocabularyInfo = _settings->vocabularyInfo(index);
		addExisting(&vocabularyInfo, parent);
	} // for
} // openAll
#endif

quint32 VocabularyOrganizer::recordCount() const
{
	quint32 count = 0;

	for (const VocabularyInfo &vocabularyInfo : _vocabularies)
	{
		if (vocabularyInfo.vocabulary->isOpen())
		{
			count += vocabularyInfo.vocabulary->recordCount();
		} // if
	} // for

	return count;
} // recordCount

quint32 VocabularyOrganizer::recordCount(bool enabled) const
{
	quint32 count = 0;

	for (const VocabularyInfo &vocabularyInfo : _vocabularies)
	{
		if (vocabularyInfo.vocabulary->isOpen())
		{
			count += vocabularyInfo.vocabulary->recordCount(enabled);
		} // if
	} // for

	return count;
} // recordCount

VocabularyOrganizer::RecordInfo VocabularyOrganizer::recordInfo(quint32 row) const
{
	quint32 currentRow = 0;
	RecordInfo recordInfo;

	for (const VocabularyInfo &vocabularyInfo : _vocabularies)
	{
		if (vocabularyInfo.vocabulary->isOpen())
		{
			const quint32 records = vocabularyInfo.vocabulary->recordCount();
			if (currentRow + records > row)
			{
				recordInfo.vocabulary = vocabularyInfo.vocabulary;
				recordInfo.id         = vocabularyInfo.vocabulary->recordId(row - currentRow);

				return recordInfo;
			}
			else
			{
				currentRow += records;
			} // if else
		} // if
	} // for

	recordInfo.vocabulary = nullptr;
	recordInfo.id         = VocabularyDatabase::NOT_FOUND;

	return recordInfo;
} // recordInfo

void VocabularyOrganizer::remove(quint8 index)
{
	Vocabulary *vocabulary = _vocabularies.at(index).vocabulary;

	_vocabularies.removeAt(index);
	emit vocabularyClose(vocabulary);

	vocabulary->close();
	delete vocabulary;
} // remove

#ifndef EDITION_TRY
void VocabularyOrganizer::saveAll()
{
	const quint8 vocabularies = _vocabularies.size();

	for (quint8 index = 0; index < vocabularies; index++)
	{
		_settings->setVocabularyInfo(index, _vocabularies.at(index).vocabularyInfo);
	} // for
	_settings->setVocabularyCount(vocabularies);
} // saveAll

# ifndef EDITION_FREE
void VocabularyOrganizer::setVocabularyEnabled(quint8 index, bool enabled, QWidget *parent)
{
	VocabularyInfo *vocabularyInfo         = &_vocabularies[index];
	vocabularyInfo->vocabularyInfo.enabled = enabled;

	if (enabled)
	{
		open(vocabularyInfo, parent);
	}
	else
	{
		emit vocabularyClose(vocabularyInfo->vocabulary);
		vocabularyInfo->vocabulary->close();
	} // if else
} // setVocabularyEnabled
# endif
#endif

quint8 VocabularyOrganizer::vocabularyCount() const
{
	return _vocabularies.size();
} // vocabularyCount

const VocabularyOrganizer::VocabularyInfo &VocabularyOrganizer::vocabularyInfo(quint8 index) const
{
	return _vocabularies.at(index);
} // vocabularyInfo

#ifndef EDITION_TRY
void VocabularyOrganizer::open(VocabularyInfo *vocabularyInfo, QWidget *parent)
{
	VocabularyOpenProgressDialog openProgress(vocabularyInfo->vocabulary, parent);
	openProgress.show();
	vocabularyInfo->vocabulary->open(vocabularyInfo->vocabularyInfo.filePath);

# ifndef EDITION_FREE
	if (!vocabularyInfo->vocabulary->isOpen())
	{
		vocabularyInfo->vocabularyInfo.enabled = false;
	} // if
# endif
} // open
#endif