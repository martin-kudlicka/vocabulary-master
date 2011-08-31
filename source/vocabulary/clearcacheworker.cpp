#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::tRecordDataHash *pRecordData, QObject *pParent /* NULL */) : QThread(pParent)
{
    _trdhRecordData = pRecordData;

	connect(this, SIGNAL(finished()), SLOT(deleteLater()));
} // ClearCacheWorker

void ClearCacheWorker::run()
{
    _trdhRecordData->clear();
    delete _trdhRecordData;
 } // run