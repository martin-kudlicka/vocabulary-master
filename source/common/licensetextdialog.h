#ifndef LICENSETEXTDIALOG_H
#define LICENSETEXTDIALOG_H

#include <ui_licensetextdialog.h>

#include <QtWidgets/QDialog>
#include "../../common/licensecommon.h"
#include "settings.h"

class LicenseTextDialog : public QDialog
{
    public:
                 LicenseTextDialog(const LicenseCommon::LicenseContentList &licenses, const Settings *settings, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~LicenseTextDialog();

    private:
        static const qint8 TAB_INVALID = -1;

		Ui::LicenseTextDialog _ui;

        void showLicenses   (const LicenseCommon::LicenseContentList &pLicenses) const;
        void showMainLicense(const Settings *pSettings)                          const;
}; // LicenseTextDialog

#endif // LICENSETEXTDIALOG_H