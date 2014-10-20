#include "common/licensetextdialog.h"

#include <QtWidgets/QPlainTextEdit>
#include <QtCore/QFile>

LicenseTextDialog::LicenseTextDialog(const LicenseCommon::LicenseContentList &licenses, const Settings *settings, QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags)
{
    _ui.setupUi(this);

    showMainLicense(settings);
    showLicenses(licenses);
} // LicenseTextDialog

LicenseTextDialog::~LicenseTextDialog()
{
} // ~LicenseTextDialog

void LicenseTextDialog::showLicenses(const LicenseCommon::LicenseContentList &licenses) const
{
    quint8 tab = TAB_INVALID;

    foreach (const LicenseCommon::LicenseContent &license, licenses)
	{
        QPlainTextEdit *licenseEdit = new QPlainTextEdit(license.text, _ui.tabs);
        licenseEdit->setReadOnly(true);
		const quint8 newTab = _ui.tabs->addTab(licenseEdit, license.title);
        if (tab == TAB_INVALID)
		{
            tab = newTab;
        } // if
    } // foreach

    if (tab != TAB_INVALID)
	{
        _ui.tabs->setCurrentIndex(tab);
    } // if
} // showLicenses

void LicenseTextDialog::showMainLicense(const Settings *settings) const
{
    LicenseCommon::LicenseContentList licenses;

	// EULA
    LicenseCommon::LicenseContent licenseContent;
    licenseContent.title = "Vocabulary Master";
    QFile licenseFile;
    if (settings->translation() == "cs_CZ.qm")
	{
        licenseFile.setFileName(":/res/eula/cze.txt");
    }
	else
	{
        licenseFile.setFileName(":/res/eula/eng.txt");
    } // if else
    licenseFile.open(QIODevice::ReadOnly);
    licenseContent.text = licenseFile.readAll();
	licenseFile.close();
    licenses.append(licenseContent);

	// fugue-icons
	licenseContent.title = "Fugue Icons";
	licenseFile.setFileName(":/res/eula/fugue-icons/README.txt");
	licenseFile.open(QIODevice::ReadOnly);
	licenseContent.text = licenseFile.readAll();
	licenses.append(licenseContent);

    showLicenses(licenses);
} // showMainLicense