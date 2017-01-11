#include "vocabularyorganizer.h"

#ifndef EDITION_TRY
# include "common/vocabularyopenprogressdialog.h"
#endif

VocabularyOrganizer::VocabularyOrganizer(Settings *settings) : _settings(settings)
{
}

VocabularyOrganizer::~VocabularyOrganizer()
{
}

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
  }
# endif

  _vocabularies.append(*vocabularyInfo);
}
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
  vocabularyInfo.vocabularyInfo.enabled = true;
#endif
  vocabularyInfo.vocabulary             = new Vocabulary(_settings);
  vocabularyInfo.vocabulary->new2(
#ifndef EDITION_TRY
    file
#endif
  );

  _vocabularies.append(vocabularyInfo);
}

bool VocabularyOrganizer::isOpen() const
{
  for (const auto &vocabularyInfo : _vocabularies)
  {
    if (vocabularyInfo.vocabulary->isOpen())
    {
      return true;
    }
  }

  return false;
}

#ifndef EDITION_TRY
void VocabularyOrganizer::openAll(QWidget *parent)
{
  const auto vocabularies = _settings->vocabularyCount();
  for (auto index = 0; index < vocabularies; index++)
  {
    VocabularyInfo vocabularyInfo;
    vocabularyInfo.vocabularyInfo = _settings->vocabularyInfo(index);
    addExisting(&vocabularyInfo, parent);
  }
}
#endif

quintptr VocabularyOrganizer::recordCount() const
{
  auto count = 0;

  for (const auto &vocabularyInfo : _vocabularies)
  {
    if (vocabularyInfo.vocabulary->isOpen())
    {
      count += vocabularyInfo.vocabulary->recordCount();
    }
  }

  return count;
}

quintptr VocabularyOrganizer::recordCount(bool enabled) const
{
  auto count = 0;

  for (const auto &vocabularyInfo : _vocabularies)
  {
    if (vocabularyInfo.vocabulary->isOpen())
    {
      count += vocabularyInfo.vocabulary->recordCount(enabled);
    }
  }

  return count;
}

VocabularyOrganizer::RecordInfo VocabularyOrganizer::recordInfo(quintptr row) const
{
  auto currentRow = 0;
  RecordInfo recordInfo;

  for (const auto &vocabularyInfo : _vocabularies)
  {
    if (vocabularyInfo.vocabulary->isOpen())
    {
      const auto records = vocabularyInfo.vocabulary->recordCount();
      if (currentRow + records > row)
      {
        recordInfo.vocabulary = vocabularyInfo.vocabulary;
        recordInfo.id         = vocabularyInfo.vocabulary->recordId(row - currentRow);

        return recordInfo;
      }
      else
      {
        currentRow += records;
      }
    }
  }

  recordInfo.vocabulary = Q_NULLPTR;
  recordInfo.id         = VocabularyDatabase::NOT_FOUND;

  return recordInfo;
}

void VocabularyOrganizer::remove(quintptr index)
{
  auto vocabulary = _vocabularies.at(index).vocabulary;

  _vocabularies.removeAt(index);
  emit vocabularyClose(vocabulary);

  vocabulary->close();
  delete vocabulary;
}

#ifndef EDITION_TRY
void VocabularyOrganizer::saveAll()
{
  const auto vocabularies = _vocabularies.size();

  for (auto index = 0; index < vocabularies; index++)
  {
    _settings->setVocabularyInfo(index, _vocabularies.at(index).vocabularyInfo);
  }
  _settings->setVocabularyCount(vocabularies);
}

# ifndef EDITION_FREE
void VocabularyOrganizer::setVocabularyEnabled(quintptr index, bool enabled, QWidget *parent)
{
  auto vocabularyInfo = &_vocabularies[index];
  vocabularyInfo->vocabularyInfo.enabled = enabled;

  if (enabled)
  {
    open(vocabularyInfo, parent);
  }
  else
  {
    emit vocabularyClose(vocabularyInfo->vocabulary);
    vocabularyInfo->vocabulary->close();
  }
}
# endif
#endif

quintptr VocabularyOrganizer::vocabularyCount() const
{
  return _vocabularies.size();
}

const VocabularyOrganizer::VocabularyInfo &VocabularyOrganizer::vocabularyInfo(quintptr index) const
{
  return _vocabularies.at(index);
}

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
  }
# endif
}
#endif