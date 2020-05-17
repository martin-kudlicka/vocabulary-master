#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"

FieldLineEdit::FieldLineEdit(QWidget *parent) : QLineEdit(parent), _defaultSheet(styleSheet())
{
  connect(this, &FieldLineEdit::textEdited, this, &FieldLineEdit::on_textEdited);
}

void FieldLineEdit::on_textEdited(const QString &text)
{
  Q_UNUSED(text);

  if (hasAcceptableInput())
  {
    setStyleSheet(_defaultSheet);
  }
  else
  {
    setStyleSheet("background: tomato");
  }
}

void FieldLineEdit::showEvent(QShowEvent *event)
{
  deselect();
  QLineEdit::showEvent(event);
}