#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <QtWidgets/QTableView>

class Vocabulary;

class VocabularyView : public QTableView
{
  Q_OBJECT

  public:
    VocabularyView(const QSharedPointer<Vocabulary> &vocabulary, QWidget *parent = Q_NULLPTR);

  private:
          QModelIndex                _moveOld;
    const QSharedPointer<Vocabulary> _vocabulary;

    virtual ~VocabularyView() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    virtual bool eventFilter    (QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent (QMouseEvent *event)              Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event)              Q_DECL_OVERRIDE;
};

#endif