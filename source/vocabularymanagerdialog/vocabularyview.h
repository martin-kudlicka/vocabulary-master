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
            Vocabulary *vocabulary,
#endif
            QWidget *parent = NULL);

#ifndef EDITION_FREE
    private:
		virtual ~VocabularyView() override;

        virtual bool eventFilter    (QObject *watched, QEvent *event) override;
        virtual void mouseMoveEvent (QMouseEvent *event)              override;
        virtual void mousePressEvent(QMouseEvent *event)              override;

              QModelIndex _moveOld;
        const Vocabulary *_vocabulary;
#endif
}; // VocabularyView

#endif // VOCABULARYVIEW_H