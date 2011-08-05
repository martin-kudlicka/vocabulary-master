#ifndef HOTKEYLINEEDIT_H
#define HOTKEYLINEEDIT_H

#include <QtGui/QLineEdit>

class HotkeyLineEdit : public QLineEdit
{
    public:
        HotkeyLineEdit(QWidget *pParent = NULL);

    private:
        virtual void keyPressEvent(QKeyEvent *event);
}; // HotkeyLineEdit

#endif // HOTKEYLINEEDIT_H