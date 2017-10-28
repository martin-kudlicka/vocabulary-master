#ifndef LICENSETEXTDIALOG_H
#define LICENSETEXTDIALOG_H

#include <ui_licensetextdialog.h>

#include <QtWidgets/QDialog>
#include "../../common/licensecommon.h"
#include <stdint.h>

class Settings;

class LicenseTextDialog : public QDialog
{
  public:
             LicenseTextDialog(const LicenseCommon::LicenseContentList &licenses, const Settings *settings, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~LicenseTextDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    Ui::LicenseTextDialog _ui;

    void showLicenses   (const LicenseCommon::LicenseContentList &pLicenses) const;
    void showMainLicense(const Settings *pSettings)                          const;
};

#endif