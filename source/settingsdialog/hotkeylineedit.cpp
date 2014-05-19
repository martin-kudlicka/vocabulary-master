#include "settingsdialog/hotkeylineedit.h"

#include <QtGui/QKeyEvent>

HotkeyLineEdit::HotkeyLineEdit(QWidget *parent /* NULL */) : QLineEdit(parent)
{
} // HotkeyLineEdit

HotkeyLineEdit::~HotkeyLineEdit()
{
} // ~HotkeyLineEdit

void HotkeyLineEdit::keyPressEvent(QKeyEvent *event)
{
	QString keySequence;

	// get shortcut
	const QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	if (keyEvent->modifiers() & Qt::ControlModifier)
	{
		keySequence = MODIFIER_CTRL;
	} // if
	if (keyEvent->modifiers() & Qt::AltModifier)
	{
		if (!keySequence.isEmpty())
		{
			keySequence += MODIFIER_SEPARATOR;
		} // if
		keySequence += MODIFIER_ALT;
	} // if
	if (keyEvent->modifiers() & Qt::ShiftModifier)
	{
		if (!keySequence.isEmpty())
		{
			keySequence += MODIFIER_SEPARATOR;
		} // if
		keySequence += MODIFIER_SHIFT;
	} // if
	if (!keySequence.isEmpty())
	{
		keySequence += MODIFIER_SEPARATOR;
	} // if

	if (QKeySequence(keyEvent->key()).toString().at(0) != 0x17C0)
	{
		// not only Ctrl, Alt or Shift keys pressed
		keySequence += QKeySequence(keyEvent->key()).toString();
		setText(keySequence);
		setProperty(PROPERTY_VIRTUALKEY, keyEvent->nativeVirtualKey());
	} // if
} // keyPressEvent