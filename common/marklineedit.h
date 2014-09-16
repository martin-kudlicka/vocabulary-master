#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

#include <QtWidgets/QLineEdit>

class MarkLineEdit : public QLineEdit
{
	Q_OBJECT

    public:
        MarkLineEdit(QWidget *parent = NULL);
        MarkLineEdit(const QString &contents, QWidget *parent = NULL);

    private:
        bool _focusedIn;

		virtual ~MarkLineEdit();

        virtual void focusInEvent   (QFocusEvent *e);
                void initialize     ();
        virtual void mousePressEvent(QMouseEvent *e);
        virtual void showEvent      (QShowEvent *event);
}; // MarkLineEdit

#endif // MARKLINEEDIT_H