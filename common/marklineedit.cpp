#include "marklineedit.h"

#include <QtGui/QFocusEvent>

MarkLineEdit::MarkLineEdit(QWidget *parent /* Q_NULLPTR */) : QLineEdit(parent)
{
  initialize();
}

MarkLineEdit::MarkLineEdit(const QString &contents, QWidget *parent /* Q_NULLPTR */) : QLineEdit(contents, parent)
{
  initialize();
}

void MarkLineEdit::initialize()
{
  _focusedIn = false;
  setReadOnly(true);
}

void MarkLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);
  if (e->gotFocus())
  {
    selectAll();
    _focusedIn = true;
  }
}

void MarkLineEdit::mousePressEvent(QMouseEvent *e)
{
  if (_focusedIn)
  {
    _focusedIn = false;
  }
  else
  {
    QLineEdit::mousePressEvent(e);
  }
}

void MarkLineEdit::showEvent(QShowEvent *event)
{
  deselect();
  QLineEdit::showEvent(event);
}