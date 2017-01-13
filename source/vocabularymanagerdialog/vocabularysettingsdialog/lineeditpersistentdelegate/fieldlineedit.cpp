#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"

FieldLineEdit::FieldLineEdit(QWidget *parent /* Q_NULLPTR */) : QLineEdit(parent), _defaultSheet(styleSheet())
{
  connect(this, &FieldLineEdit::textEdited, this, &FieldLineEdit::on_textEdited);
}

FieldLineEdit::~FieldLineEdit()
{
}

void FieldLineEdit::on_textEdited(const QString &text)
{
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