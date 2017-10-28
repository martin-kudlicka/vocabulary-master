#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData) : _recordData(recordData)
{
  connect(this, &ClearCacheWorker::finished, this, &ClearCacheWorker::deleteLater);
}

void ClearCacheWorker::run()
{
  _recordData->clear();
  delete _recordData;
}