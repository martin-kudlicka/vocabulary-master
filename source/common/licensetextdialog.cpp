#include "common/licensetextdialog.h"

#include <QtGui/QPlainTextEdit>

LicenseTextDialog::LicenseTextDialog(const LicenseCommon::tLicenseContentList &pLicenses, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _qdltLicenseText.setupUi(this);

    // TODO show main license

    ShowLicenses(pLicenses);
} // LicenseTextDialog

const void LicenseTextDialog::ShowLicenses(const LicenseCommon::tLicenseContentList &pLicenses) const
{
    foreach (LicenseCommon::sLicenseContent slcLicense, pLicenses) {
        QPlainTextEdit *qpteLicense = new QPlainTextEdit(slcLicense.qsText, _qdltLicenseText.qtwTabs);
        _qdltLicenseText.qtwTabs->addTab(qpteLicense, slcLicense.qsTitle);
    } // foreach
} // ShowLicenses