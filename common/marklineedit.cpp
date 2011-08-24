#include "marklineedit.h"

#include <QtGui/QFocusEvent>

void MarkLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    if (e->gotFocus()) {
        selectAll();
        _bFocusedIn = true;
    } // if
} // focusInEvent

const void MarkLineEdit::Initialize()
{
    _bFocusedIn = false;
    setReadOnly(true);
} // Initialize

MarkLineEdit::MarkLineEdit(QWidget *pParent /* NULL */) : QLineEdit(pParent)
{
    Initialize();
} // MarkLineEdit

MarkLineEdit::MarkLineEdit(const QString &pContents, QWidget *pParent /* NULL */) : QLineEdit(pContents, pParent)
{
    Initialize();
} // MarkLineEdit

void MarkLineEdit::mousePressEvent(QMouseEvent *e)
{
    if (_bFocusedIn) {
        _bFocusedIn = false;
    } else {
        QLineEdit::mousePressEvent(e);
    } // if else
} // mousePressEvent

void MarkLineEdit::showEvent(QShowEvent *event)
{
    deselect();
    QLineEdit::showEvent(event);
} // showEvent