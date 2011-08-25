#include "licensedialog.h"

const void LicenseDialog::FillLicenseInfo() const
{
    _qdlLicenseDialog.qlFirstName->setText(_lLicense->GetFirstName());
	_qdlLicenseDialog.qlLastName->setText(_lLicense->GetLastName());
	_qdlLicenseDialog.qlValidTo->setText(_lLicense->GetValidTo().toString(Qt::DefaultLocaleLongDate));
} // FillLicenseInfo

LicenseDialog::LicenseDialog(const License *pLicense, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _lLicense = pLicense;

    _qdlLicenseDialog.setupUi(this);

    FillLicenseInfo();
} // LicenseDialog