#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"

FieldLineEdit::FieldLineEdit(QWidget *pParent /* NULL */) : QLineEdit(pParent)
{
} // FieldLineEdit

void FieldLineEdit::showEvent(QShowEvent *event)
{
	deselect();
	QLineEdit::showEvent(event);
} // showEvent