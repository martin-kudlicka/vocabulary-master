#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include <QtCore/QRunnable>
#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QRunnable
{
    public:
        ClearCacheWorker(VocabularyDatabase::tRecordDataHash *pRecordData);

    private:
        VocabularyDatabase::tRecordDataHash *_trdhRecordData;

        virtual void run();
}; // ClearCacheWorker

#endif // CLEARCACHEWORKER_H