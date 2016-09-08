#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate/fieldlineedit.h"

FieldLineEdit::FieldLineEdit(QWidget *parent /* nullptr */) : QLineEdit(parent), _defaultSheet(styleSheet())
{
	connect(this, SIGNAL(textEdited(const QString &)), SLOT(on_textEdited(const QString &)));
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