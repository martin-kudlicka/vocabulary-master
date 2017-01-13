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
    HotkeyLineEdit(QWidget *parent = Q_NULLPTR);

  private:
    virtual ~HotkeyLineEdit() Q_DECL_OVERRIDE;

    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};

#endif