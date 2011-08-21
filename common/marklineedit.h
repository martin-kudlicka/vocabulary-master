#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

#include <QtGui/QLineEdit>

class MarkLineEdit : public QLineEdit
{
	Q_OBJECT

    public:
        MarkLineEdit(const QString &pContents, QWidget *pParent = NULL);

    private:
        bool _bFocusedIn;

        virtual void focusInEvent(QFocusEvent *e);
        virtual void mousePressEvent(QMouseEvent *e);
}; // MarkLineEdit

#endif // MARKLINEEDIT_H