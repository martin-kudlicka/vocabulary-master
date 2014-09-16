#include "marklineedit.h"

#include <QtGui/QFocusEvent>

MarkLineEdit::MarkLineEdit(QWidget *parent /* NULL */) : QLineEdit(parent)
{
	initialize();
} // MarkLineEdit

MarkLineEdit::MarkLineEdit(const QString &contents, QWidget *parent /* NULL */) : QLineEdit(contents, parent)
{
	initialize();
} // MarkLineEdit

MarkLineEdit::~MarkLineEdit()
{
} // ~MarkLineEdit

void MarkLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    if (e->gotFocus())
	{
        selectAll();
        _focusedIn = true;
    } // if
} // focusInEvent

void MarkLineEdit::initialize()
{
    _focusedIn = false;
    setReadOnly(true);
} // initialize

void MarkLineEdit::mousePressEvent(QMouseEvent *e)
{
    if (_focusedIn)
	{
        _focusedIn = false;
    }
	else
	{
        QLineEdit::mousePressEvent(e);
    } // if else
} // mousePressEvent

void MarkLineEdit::showEvent(QShowEvent *event)
{
    deselect();
    QLineEdit::showEvent(event);
} // showEvent