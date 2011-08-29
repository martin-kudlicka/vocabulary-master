#ifndef LICENSETEXTDIALOG_H
#define LICENSETEXTDIALOG_H

#include <ui_licensetextdialog.h>

#include <QtGui/QDialog>

class LicenseTextDialog : public QDialog
{
    public:
        LicenseTextDialog(const QString &pText, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
		Ui::qdLicenseText _qdltLicenseText;
}; // LicenseTextDialog

#endif // LICENSETEXTDIALOG_H