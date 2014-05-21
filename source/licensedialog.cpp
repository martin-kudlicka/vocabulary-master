#include "licensedialog.h"

#include <QtWidgets/QFileDialog>
#include "common/licensetextdialog.h"

LicenseDialog::LicenseDialog(License *license, Settings *settings, QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _refreshed(false), _license(license), _settings(settings)
{
	_ui.setupUi(this);

	fillLicenseInfo();
} // LicenseDialog

LicenseDialog::~LicenseDialog()
{
} // ~LicenseDialog

const void LicenseDialog::fillLicenseInfo() const
{
	if (_license->status() == License::StatusOk || _license->status() == License::StatusExpired)
	{
		_ui.firstName->setText(_license->firstName());
		_ui.lastName->setText(_license->lastName());
		_ui.email->setText(_license->email());
		_ui.validTo->setText(_license->validTo().toString(Qt::DefaultLocaleLongDate));

		_ui.firstName->show();
		_ui.lastName->show();
		_ui.email->show();
		_ui.validTo->show();
	}
	else
	{
		_ui.firstName->hide();
		_ui.lastName->hide();
		_ui.email->hide();
		_ui.validTo->hide();
	} // if else

	switch (_license->status())
	{
		case License::StatusNone:
			_ui.status->setText(tr("No license."));
			break;
		case License::StatusExpired:
			_ui.status->setText(tr("License has been expired."));
			break;
		case License::StatusInvalid:
			_ui.status->setText(tr("License is invalid."));
	} // switch
	if (_license->status() == License::StatusOk)
	{
		_ui.status->hide();
	}
	else
	{
		_ui.status->show();
	} // if else
} // fillLicenseInfo

const void LicenseDialog::on_eula_clicked(bool checked /* false */)
{
	LicenseTextDialog licenseTextDialog(LicenseCommon::LicenseContentList(), _settings, this);
	licenseTextDialog.exec();
} // on_eula_clicked

const void LicenseDialog::on_load_clicked(bool checked /* false */)
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Load license"), QString(), tr("license (*.lic)"));
	if (fileName.isEmpty())
	{
		return;
	} // if

	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	const QByteArray licenseData = file.readAll();

	_settings->setLicense(licenseData);
	_license->refreshLicense();

	fillLicenseInfo();

	_refreshed = true;
} // on_load_clicked

const void LicenseDialog::on_ok_clicked(bool checked /* false */)
{
	if (_refreshed)
	{
		accept();
	}
	else
	{
		reject();
	} // if else
} // on_ok_clicked