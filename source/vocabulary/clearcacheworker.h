#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include <QtCore/QThread>
#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QThread
{
  public:
    ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData, QObject *parent = Q_NULLPTR);

  private:
    VocabularyDatabase::RecordDataHash *_recordData;

    virtual ~ClearCacheWorker() Q_DECL_OVERRIDE;

    virtual void run() Q_DECL_OVERRIDE;
};

#endif