#ifndef MARKLINEEDIT_H
#define MARKLINEEDIT_H

#include <QtWidgets/QLineEdit>

class MarkLineEdit : public QLineEdit
{
	Q_OBJECT

    public:
        MarkLineEdit(QWidget *parent = nullptr);
        MarkLineEdit(const QString &contents, QWidget *parent = nullptr);

    private:
        bool _focusedIn;

		virtual ~MarkLineEdit() override;

        virtual void focusInEvent   (QFocusEvent *e)    override;
                void initialize     ();
        virtual void mousePressEvent(QMouseEvent *e)    override;
        virtual void showEvent      (QShowEvent *event) override;
}; // MarkLineEdit

#endif // MARKLINEEDIT_H