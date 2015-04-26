#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData, QObject *parent /* nullptr */) : QThread(parent), _recordData(recordData)
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