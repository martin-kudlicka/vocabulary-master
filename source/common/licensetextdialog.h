#ifndef LICENSETEXTDIALOG_H
#define LICENSETEXTDIALOG_H

#include <ui_licensetextdialog.h>

#include <QtWidgets/QDialog>
#include "../../common/licensecommon.h"
#include "settings.h"

class LicenseTextDialog : public QDialog
{
    public:
        LicenseTextDialog(const LicenseCommon::LicenseContentList &pLicenses, const Settings *pSettings, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        static const int TAB_INVALID = -1;

		Ui::qdLicenseText _qdltLicenseText;

        const void ShowLicenses(const LicenseCommon::LicenseContentList &pLicenses) const;
        const void ShowMainLicense(const Settings *pSettings) const;
}; // LicenseTextDialog

#endif // LICENSETEXTDIALOG_H