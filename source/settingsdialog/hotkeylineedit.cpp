#include "settingsdialog/hotkeylineedit.h"

#include <QtGui/QKeyEvent>

const auto MODIFIER_SEPARATOR = '+';

HotkeyLineEdit::HotkeyLineEdit(QWidget *parent /* Q_NULLPTR */) : QLineEdit(parent)
{
}

void HotkeyLineEdit::keyPressEvent(QKeyEvent *event)
{
  QString keySequence;

  // get shortcut
  if (event->modifiers() & Qt::ControlModifier)
  {
    keySequence = MODIFIER_CTRL;
  }
  if (event->modifiers() & Qt::AltModifier)
  {
    if (!keySequence.isEmpty())
    {
      keySequence += MODIFIER_SEPARATOR;
    }
    keySequence += MODIFIER_ALT;
  }
  if (event->modifiers() & Qt::ShiftModifier)
  {
    if (!keySequence.isEmpty())
    {
      keySequence += MODIFIER_SEPARATOR;
    }
    keySequence += MODIFIER_SHIFT;
  }
  if (!keySequence.isEmpty())
  {
    keySequence += MODIFIER_SEPARATOR;
  }

  if (QKeySequence(event->key()).toString().at(0) != QChar(0x17C0))
  {
    // not only Ctrl, Alt or Shift keys pressed
    keySequence += QKeySequence(event->key()).toString();
    setText(keySequence);
    setProperty(PROPERTY_VIRTUALKEY, event->nativeVirtualKey());
  }
}