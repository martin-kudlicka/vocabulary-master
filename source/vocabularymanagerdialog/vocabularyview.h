#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <QtGui/QTableView>
#include "vocabulary.h"

class VocabularyView : public QTableView
{
    Q_OBJECT

    public:
        VocabularyView(
#ifndef FREE
            Vocabulary *pVocabulary,
#endif
            QWidget *pParent = NULL);

#ifndef FREE
    private:
        virtual void mousePressEvent(QMouseEvent *event);

        const Vocabulary *_vVocabulary;
#endif
}; // VocabularyView

#endif // VOCABULARYVIEW_H