#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include <QtCore/QThread>
#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QThread
{
    public:
        ClearCacheWorker(VocabularyDatabase::tRecordDataHash *pRecordData, QObject *pParent = NULL);

    private:
        VocabularyDatabase::tRecordDataHash *_trdhRecordData;

        virtual void run();
}; // ClearCacheWorker

#endif // CLEARCACHEWORKER_H