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
		qsKey = MODIFIER_CTRL;
	} // if
	if (qkeKey->modifiers() & Qt::AltModifier) {
		if (!qsKey.isEmpty()) {
			qsKey += MODIFIER_SEPARATOR;
		} // if
		qsKey += MODIFIER_ALT;
	} // if
	if (qkeKey->modifiers() & Qt::ShiftModifier) {
		if (!qsKey.isEmpty()) {
			qsKey += MODIFIER_SEPARATOR;
		} // if
		qsKey += MODIFIER_SHIFT;
	} // if
	if (!qsKey.isEmpty()) {
		qsKey += MODIFIER_SEPARATOR;
	} // if

	if (QKeySequence(qkeKey->key()).toString().at(0) != 0x17C0) {
		// not only Ctrl, Alt or Shift keys pressed
		qsKey += QKeySequence(qkeKey->key()).toString();
		setText(qsKey);
		setProperty(PROPERTY_VIRTUALKEY, qkeKey->nativeVirtualKey());
	} // if
} // keyPressEvent