#ifndef LICENSETEXTDIALOG_H
#define LICENSETEXTDIALOG_H

#include <ui_licensetextdialog.h>

#include <QtGui/QDialog>
#include "../../common/licensecommon.h"

class LicenseTextDialog : public QDialog
{
    public:
        LicenseTextDialog(const LicenseCommon::tLicenseContentList &pLicenses, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
		Ui::qdLicenseText _qdltLicenseText;

        const void ShowLicenses(const LicenseCommon::tLicenseContentList &pLicenses) const;
}; // LicenseTextDialog

#endif // LICENSETEXTDIALOG_H