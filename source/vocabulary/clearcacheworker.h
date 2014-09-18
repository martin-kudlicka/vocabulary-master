#ifndef CLEARCACHEWORKER_H
#define CLEARCACHEWORKER_H

#include <QtCore/QThread>
#include "vocabulary/vocabularydatabase.h"

class ClearCacheWorker : public QThread
{
    public:
        ClearCacheWorker(VocabularyDatabase::RecordDataHash *recordData, QObject *parent = NULL);

    private:
        VocabularyDatabase::RecordDataHash *_recordData;

		virtual ~ClearCacheWorker();

        virtual void run();
}; // ClearCacheWorker

#endif // CLEARCACHEWORKER_H