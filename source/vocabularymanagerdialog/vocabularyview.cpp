#include "vocabularymanagerdialog/vocabularyview.h"

#include "vocabulary.h"
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSpinBox>
#include <QtCore/QCoreApplication>

VocabularyView::VocabularyView(const QSharedPointer<Vocabulary> &vocabulary, QWidget *parent /* Q_NULLPTR */) : QTableView(parent), _vocabulary(vocabulary)
{
  setMouseTracking(true);
  viewport()->installEventFilter(this);
}

bool VocabularyView::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == viewport() && event->type() == QEvent::Leave)
  {
    update(_moveOld);
    _moveOld = QModelIndex();
  }

  return QTableView::eventFilter(watched, event);
}

void VocabularyView::mouseMoveEvent(QMouseEvent *event)
{
  const auto mouse = indexAt(event->pos());
  if (_moveOld.isValid() && _moveOld != mouse)
  {
    update(_moveOld);
  }
  _moveOld = mouse;

  const auto fieldId = _vocabulary->fieldId(mouse.column());
  if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
  {
    const auto builtIn = _vocabulary->fieldBuiltIn(fieldId);
    if (builtIn == VocabularyDatabase::FieldBuiltIn::Priority)
    {
      update(mouse);
    }
  }

  QTableView::mouseMoveEvent(event);
}

void VocabularyView::mousePressEvent(QMouseEvent *event)
{
  const auto oldIndex = currentIndex();
  QTableView::mousePressEvent(event);
  const auto newIndex = currentIndex();

  if (oldIndex != newIndex && event->button() == Qt::LeftButton)
  {
    const auto fieldId = _vocabulary->fieldId(newIndex.column());
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      const auto builtIn = _vocabulary->fieldBuiltIn(fieldId);
      if (builtIn == VocabularyDatabase::FieldBuiltIn::Priority)
      {
        auto editor          = qobject_cast<QSpinBox *>(indexWidget(newIndex));
        const auto editorPos = editor->mapFromParent(event->pos());

        QMouseEvent press(QEvent::MouseButtonPress, editorPos, Qt::LeftButton, Qt::LeftButton, 0);
        QCoreApplication::sendEvent(editor, &press);
        QMouseEvent release(QEvent::MouseButtonRelease, editorPos, Qt::LeftButton, Qt::LeftButton, 0);
        QCoreApplication::sendEvent(editor, &release);
      }
    }
  }
}