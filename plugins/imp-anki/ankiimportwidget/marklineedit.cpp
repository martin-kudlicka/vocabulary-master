#include "ankiimportwidget/marklineedit.h"

#include <QtGui/QFocusEvent>

void MarkLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    if (e->gotFocus()) {
        selectAll();
        _bFocusedIn = true;
    } // if
} // focusInEvent

MarkLineEdit::MarkLineEdit(const QString &pContents, QWidget *pParent /* NULL */) : QLineEdit(pContents, pParent)
{
    _bFocusedIn = false;

    setReadOnly(true);
} // MarkLineEdit

void MarkLineEdit::mousePressEvent(QMouseEvent *e)
{
    if (_bFocusedIn) {
        _bFocusedIn = false;
    } else {
        QLineEdit::mousePressEvent(e);
    } // if else
} // mousePressEvent