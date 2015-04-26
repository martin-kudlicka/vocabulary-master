#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"

FieldLineEdit::FieldLineEdit(QWidget *parent /* nullptr */) : QLineEdit(parent)
{
	_defaultSheet = styleSheet();
	connect(this, SIGNAL(textEdited(const QString &)), SLOT(on_textEdited(const QString &)));
} // FieldLineEdit

FieldLineEdit::~FieldLineEdit()
{
} // ~FieldLineEdit

void FieldLineEdit::on_textEdited(const QString &text)
{
	if (hasAcceptableInput())
	{
		setStyleSheet(_defaultSheet);
	}
	else
	{
		setStyleSheet("background: tomato");
	} // if else
} // on_textEdited

void FieldLineEdit::showEvent(QShowEvent *event)
{
	deselect();
	QLineEdit::showEvent(event);
} // showEvent