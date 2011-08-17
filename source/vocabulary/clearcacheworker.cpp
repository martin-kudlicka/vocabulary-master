#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::tRecordDataHash *pRecordData)
{
    _trdhRecordData = pRecordData;

	connect(this, SIGNAL(finished()), SLOT(deleteLater()));
} // ClearCacheWorker

void ClearCacheWorker::run()
{
    _trdhRecordData->clear();
 } // run