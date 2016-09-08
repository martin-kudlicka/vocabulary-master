#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include <QtCore/QThread>
#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QThread
{
  public:
    ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData, QObject *parent = nullptr);

  private:
    VocabularyDatabase::RecordDataHash *_recordData;

    virtual ~ClearCacheWorker() override;

    virtual void run() override;
};

#endif