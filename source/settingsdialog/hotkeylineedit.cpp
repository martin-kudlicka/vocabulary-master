#include "settingsdialog/hotkeylineedit.h"

#include <QtGui/QKeyEvent>

HotkeyLineEdit::HotkeyLineEdit(QWidget *pParent /* NULL */) : QLineEdit(pParent)
{
} // HotkeyLineEdit

void HotkeyLineEdit::keyPressEvent(QKeyEvent *event)
{
	QString qsKey;

	// get shortcut
	const QKeyEvent *qkeKey = static_cast<QKeyEvent *>(event);
	if (qkeKey->modifiers() & Qt::ControlModifier) {
		qsKey = "Ctrl";
	} // if
	if (qkeKey->modifiers() & Qt::AltModifier) {
		if (!qsKey.isEmpty()) {
			qsKey += "+";
		} // if
		qsKey += "Alt";
	} // if
	if (qkeKey->modifiers() & Qt::ShiftModifier) {
		if (!qsKey.isEmpty()) {
			qsKey += "+";
		} // if
		qsKey += "Shift";
	} // if
	if (!qsKey.isEmpty()) {
		qsKey += "+";
	} // if

	if (QKeySequence(qkeKey->key()).toString().at(0) != 0x17C0) {
		// not only Ctrl, Alt or Shift keys pressed
		qsKey += QKeySequence(qkeKey->key()).toString();
		setText(qsKey);
	} // if
} // keyPressEvent