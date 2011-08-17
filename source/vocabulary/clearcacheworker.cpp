#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::tRecordDataHash *pRecordData)
{
    _trdhRecordData = pRecordData;
} // ClearCacheWorker

void ClearCacheWorker::run()
{
    _trdhRecordData->clear();
    deleteLater();
} // run