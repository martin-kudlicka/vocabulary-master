#ifndef FIELDLINEEDIT_H
#define FIELDLINEEDIT_H

#include <QtWidgets/QLineEdit>

class FieldLineEdit : public QLineEdit
{
	Q_OBJECT

	public:
		FieldLineEdit(QWidget *parent = NULL);

	private:
		QString _defaultSheet;

		virtual ~FieldLineEdit();

		virtual void showEvent(QShowEvent *event);

	private slots:
		void on_textEdited(const QString &text);
}; // FieldLineEdit

#endif // FIELDLINEEDIT_H