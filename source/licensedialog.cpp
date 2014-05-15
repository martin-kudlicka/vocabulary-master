#include "licensedialog.h"

#include <QtWidgets/QFileDialog>
#include "common/licensetextdialog.h"

const void LicenseDialog::FillLicenseInfo() const
{
	if (_lLicense->status() == License::StatusOk || _lLicense->status() == License::StatusExpired) {
		_qdlLicenseDialog.qlFirstName->setText(_lLicense->firstName());
		_qdlLicenseDialog.qlLastName->setText(_lLicense->lastName());
		_qdlLicenseDialog.qlEmail->setText(_lLicense->email());
		_qdlLicenseDialog.qlValidTo->setText(_lLicense->validTo().toString(Qt::DefaultLocaleLongDate));

		_qdlLicenseDialog.qlFirstName->show();
		_qdlLicenseDialog.qlLastName->show();
		_qdlLicenseDialog.qlEmail->show();
		_qdlLicenseDialog.qlValidTo->show();
	} else {
		_qdlLicenseDialog.qlFirstName->hide();
		_qdlLicenseDialog.qlLastName->hide();
		_qdlLicenseDialog.qlEmail->hide();
		_qdlLicenseDialog.qlValidTo->hide();
	} // if else

	switch (_lLicense->status()) {
		case License::StatusNone:
			_qdlLicenseDialog.qlStatus->setText(tr("No license."));
			break;
		case License::StatusExpired:
			_qdlLicenseDialog.qlStatus->setText(tr("License has been expired."));
			break;
		case License::StatusInvalid:
			_qdlLicenseDialog.qlStatus->setText(tr("License is invalid."));
	} // switch
	if (_lLicense->status() == License::StatusOk) {
		_qdlLicenseDialog.qlStatus->hide();
	} else {
		_qdlLicenseDialog.qlStatus->show();
	} // if else
} // FillLicenseInfo

LicenseDialog::LicenseDialog(License *pLicense, Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _lLicense = pLicense;
	_sSettings = pSettings;

	_bRefreshed = false;

    _qdlLicenseDialog.setupUi(this);

    FillLicenseInfo();
} // LicenseDialog

const void LicenseDialog::on_qpbEULA_clicked(bool checked /* false */)
{
	LicenseTextDialog ltdEula(LicenseCommon::LicenseContentList(), _sSettings, this);
	ltdEula.exec();
} // on_qpbEULA_clicked

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
	_lLicense->refreshLicense();

	FillLicenseInfo();

	_bRefreshed = true;
} // on_qpbLoad_clicked

const void LicenseDialog::on_qpbOk_clicked(bool checked /* false */)
{
	if (_bRefreshed) {
		accept();
	} else {
		reject();
	} // if else
} // on_qpbOk_clicked