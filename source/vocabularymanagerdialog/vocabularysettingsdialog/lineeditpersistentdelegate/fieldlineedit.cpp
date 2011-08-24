#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"

FieldLineEdit::FieldLineEdit(QWidget *pParent /* NULL */) : QLineEdit(pParent)
{
	_qsDefaultSheet = styleSheet();
	connect(this, SIGNAL(textEdited(const QString &)), SLOT(on_textEdited(const QString &)));
} // FieldLineEdit

const void FieldLineEdit::on_textEdited(const QString &text)
{
	if (hasAcceptableInput()) {
		setStyleSheet(_qsDefaultSheet);
	} else {
		setStyleSheet("background: tomato");
	} // if else
} // on_textEdited

void FieldLineEdit::showEvent(QShowEvent *event)
{
	deselect();
	QLineEdit::showEvent(event);
} // showEvent