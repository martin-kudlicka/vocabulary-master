#include "licensedialog.h"

#include <QtGui/QFileDialog>

const void LicenseDialog::FillLicenseInfo() const
{
    _qdlLicenseDialog.qlFirstName->setText(_lLicense->GetFirstName());
	_qdlLicenseDialog.qlLastName->setText(_lLicense->GetLastName());
	_qdlLicenseDialog.qlEmail->setText(_lLicense->GetEmail());
	_qdlLicenseDialog.qlValidTo->setText(_lLicense->GetValidTo().toString(Qt::DefaultLocaleLongDate));
} // FillLicenseInfo

LicenseDialog::LicenseDialog(License *pLicense, Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _lLicense = pLicense;
	_sSettings = pSettings;

    _qdlLicenseDialog.setupUi(this);

    FillLicenseInfo();
} // LicenseDialog

const void LicenseDialog::on_qpbLoad_clicked(bool checked /* false */)
{
	QString qsFile = QFileDialog::getOpenFileName(this, tr("Load license"), QString(), tr("license (*.lic)"));
	if (qsFile.isEmpty()) {
		return;
	} // if

	QFile qfFile(qsFile);
	qfFile.open(QIODevice::ReadOnly);
	QByteArray qbaData = qfFile.readAll();

	_sSettings->SetLicense(qbaData);
	_lLicense->RefreshLicense();

	FillLicenseInfo();
} // on_qpbLoad_clicked