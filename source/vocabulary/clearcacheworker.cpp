#include "vocabulary/clearcacheworker.h"

ClearCacheWorker::ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData, QObject *parent /* Q_NULLPTR */) : QThread(parent), _recordData(recordData)
{
  connect(this, SIGNAL(finished()), SLOT(deleteLater()));
}

ClearCacheWorker::~ClearCacheWorker()
{
}

void ClearCacheWorker::run()
{
  _recordData->clear();
  delete _recordData;
}