#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <QtWidgets/QTableView>
#include "vocabulary.h"

class VocabularyView : public QTableView
{
    Q_OBJECT

    public:
        VocabularyView(
#ifndef EDITION_FREE
            Vocabulary *pVocabulary,
#endif
            QWidget *pParent = NULL);

#ifndef EDITION_FREE
    private:
        virtual bool eventFilter(QObject *watched, QEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mousePressEvent(QMouseEvent *event);

        QModelIndex _qmiMoveOld;
        const Vocabulary *_vVocabulary;
#endif
}; // VocabularyView

#endif // VOCABULARYVIEW_H