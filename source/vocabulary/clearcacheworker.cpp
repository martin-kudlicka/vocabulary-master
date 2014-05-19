#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::tRecordDataHash *recordData, QObject *parent /* NULL */) : QThread(parent), _recordData(recordData)
{
	connect(this, SIGNAL(finished()), SLOT(deleteLater()));
} // ClearCacheWorker

ClearCacheWorker::~ClearCacheWorker()
{
} // ~ClearCacheWorker

void ClearCacheWorker::run()
{
    _recordData->clear();
    delete _recordData;
 } // run