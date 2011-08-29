#include "common/licensetextdialog.h"

LicenseTextDialog::LicenseTextDialog(const QString &pText, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _qdltLicenseText.setupUi(this);

    _qdltLicenseText.qpteLicense->setPlainText(pText);
} // LicenseTextDialog