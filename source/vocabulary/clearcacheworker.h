#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QThread
{
  public:
    ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData);

  private:
    VocabularyDatabase::RecordDataHash *_recordData;

    virtual ~ClearCacheWorker() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual void run() Q_DECL_OVERRIDE;
};

#endif