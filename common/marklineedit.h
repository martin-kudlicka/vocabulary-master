#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

#include <QtGui/QLineEdit>

class MarkLineEdit : public QLineEdit
{
	Q_OBJECT

    public:
        MarkLineEdit(QWidget *pParent = NULL);
        MarkLineEdit(const QString &pContents, QWidget *pParent = NULL);

    private:
        bool _bFocusedIn;

        virtual void focusInEvent(QFocusEvent *e);
        const void Initialize();
        virtual void mousePressEvent(QMouseEvent *e);
        virtual void showEvent(QShowEvent *event);
}; // MarkLineEdit

#endif // MARKLINEEDIT_H