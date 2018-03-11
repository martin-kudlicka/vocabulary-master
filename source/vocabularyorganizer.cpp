#include "vocabularyorganizer.h"

#include "common/vocabularyopenprogressdialog.h"

VocabularyOrganizer::VocabularyOrganizer(Settings *settings) : _settings(settings)
{
}

void VocabularyOrganizer::addExisting(VocabularyInfo *vocabularyInfo, QWidget *parent)
{
  vocabularyInfo->vocabulary = QSharedPointer<Vocabulary>::create(_settings);

  if (vocabularyInfo->vocabularyInfo.enabled)
  {
    open(vocabularyInfo, parent);
  }

  _vocabularies.append(*vocabularyInfo);
}

void VocabularyOrganizer::addNew(const QString &file)
{
  VocabularyInfo vocabularyInfo;
  vocabularyInfo.vocabularyInfo.filePath = file;
  vocabularyInfo.vocabularyInfo.enabled  = true;
  vocabularyInfo.vocabulary.reset(new Vocabulary(_settings));
  vocabularyInfo.vocabulary->new2(file);

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

void VocabularyOrganizer::openAll(QWidget *parent)
{
  auto vocabularies = _settings->vocabularyCount();
  for (decltype(vocabularies) index = 0; index < vocabularies; ++index)
  {
    VocabularyInfo vocabularyInfo;
    vocabularyInfo.vocabularyInfo = _settings->vocabularyInfo(index);
    addExisting(&vocabularyInfo, parent);
  }
}

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

  recordInfo.vocabulary.clear();
  recordInfo.id = VocabularyDatabase::NOT_FOUND;

  return recordInfo;
}

void VocabularyOrganizer::remove(quintptr index)
{
  auto vocabulary = _vocabularies.at(index).vocabulary;

  _vocabularies.removeAt(index);
  emit vocabularyClose(vocabulary);

  vocabulary->close();
  vocabulary.clear();
}

void VocabularyOrganizer::saveAll()
{
  const auto vocabularies = _vocabularies.count();

  for (auto index = 0; index < vocabularies; ++index)
  {
    _settings->setVocabularyInfo(index, _vocabularies.at(index).vocabularyInfo);
  }
  _settings->setVocabularyCount(vocabularies);
}

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

quintptr VocabularyOrganizer::vocabularyCount() const
{
  return _vocabularies.count();
}

const VocabularyOrganizer::VocabularyInfo &VocabularyOrganizer::vocabularyInfo(quintptr index) const
{
  return _vocabularies.at(index);
}

void VocabularyOrganizer::open(VocabularyInfo *vocabularyInfo, QWidget *parent)
{
  VocabularyOpenProgressDialog openProgress(vocabularyInfo->vocabulary.data(), parent);
  openProgress.show();
  vocabularyInfo->vocabulary->open(vocabularyInfo->vocabularyInfo.filePath);

  if (!vocabularyInfo->vocabulary->isOpen())
  {
    vocabularyInfo->vocabularyInfo.enabled = false;
  }
}