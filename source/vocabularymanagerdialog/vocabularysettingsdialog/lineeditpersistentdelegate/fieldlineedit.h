#ifndef FIELDLINEEDIT_H
#define FIELDLINEEDIT_H

#include <QtGui/QLineEdit>

class FieldLineEdit : public QLineEdit
{
	Q_OBJECT

	public:
		FieldLineEdit(QWidget *pParent = NULL);

	private:
		virtual void showEvent(QShowEvent *event);
}; // FieldLineEdit

#endif // FIELDLINEEDIT_H