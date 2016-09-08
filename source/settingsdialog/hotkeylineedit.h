#ifndef HOTKEYLINEEDIT_H
#define HOTKEYLINEEDIT_H

#include <QtWidgets/QLineEdit>

static const char   *PROPERTY_VIRTUALKEY = "VirtualKey";
       const QString MODIFIER_ALT        = "Alt";
       const QString MODIFIER_CTRL       = "Ctrl";
       const QString MODIFIER_SHIFT      = "Shift";

class HotkeyLineEdit : public QLineEdit
{
  public:
    HotkeyLineEdit(QWidget *parent = nullptr);

  private:
    virtual ~HotkeyLineEdit() override;

    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif