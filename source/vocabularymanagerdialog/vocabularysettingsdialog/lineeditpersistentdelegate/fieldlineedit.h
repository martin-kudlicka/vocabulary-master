#ifndef FIELDLINEEDIT_H
#define FIELDLINEEDIT_H

#include <QtGui/QLineEdit>

class FieldLineEdit : public QLineEdit
{
	Q_OBJECT

	public:
		FieldLineEdit(QWidget *pParent = NULL);

	private:
		QString _qsDefaultSheet;

		virtual void showEvent(QShowEvent *event);

	private slots:
		const void on_textEdited(const QString &text);
}; // FieldLineEdit

#endif // FIELDLINEEDIT_H