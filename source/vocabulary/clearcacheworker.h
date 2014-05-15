#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include <QtCore/QThread>
#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QThread
{
    public:
        ClearCacheWorker(VocabularyDatabase::tRecordDataHash *recordData, QObject *parent = NULL);

    private:
        VocabularyDatabase::tRecordDataHash *_recordData;

        virtual void run();
}; // ClearCacheWorker

#endif // CLEARCACHEWORKER_H