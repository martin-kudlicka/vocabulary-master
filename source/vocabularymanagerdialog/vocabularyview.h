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
            QWidget *parent = nullptr);

#ifndef EDITION_FREE
    private:
		      QModelIndex _moveOld;
		const Vocabulary *_vocabulary;

		virtual ~VocabularyView() override;

        virtual bool eventFilter    (QObject *watched, QEvent *event) override;
        virtual void mouseMoveEvent (QMouseEvent *event)              override;
        virtual void mousePressEvent(QMouseEvent *event)              override;
#endif
}; // VocabularyView

#endif // VOCABULARYVIEW_H