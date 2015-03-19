#ifndef LICENSETEXTDIALOG_H
#define LICENSETEXTDIALOG_H

#include <ui_licensetextdialog.h>

#include <QtWidgets/QDialog>
#include "../../common/licensecommon.h"
#include "settings.h"
#include <stdint.h>

class LicenseTextDialog : public QDialog
{
    public:
                 LicenseTextDialog(const LicenseCommon::LicenseContentList &licenses, const Settings *settings, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~LicenseTextDialog() override;

    private:
		static const quint8 TAB_INVALID = UINT8_MAX;

		Ui::LicenseTextDialog _ui;

        void showLicenses   (const LicenseCommon::LicenseContentList &pLicenses) const;
        void showMainLicense(const Settings *pSettings)                          const;
}; // LicenseTextDialog

#endif // LICENSETEXTDIALOG_H