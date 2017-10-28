#include "common/licensetextdialog.h"

#include "settings.h"
#include <QtWidgets/QPlainTextEdit>
#include <QtCore/QFile>

const auto TAB_INVALID = UINT8_MAX;

LicenseTextDialog::LicenseTextDialog(const LicenseCommon::LicenseContentList &licenses, const Settings *settings, QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags)
{
  _ui.setupUi(this);

  showMainLicense(settings);
  showLicenses(licenses);
}

void LicenseTextDialog::showLicenses(const LicenseCommon::LicenseContentList &licenses) const
{
  auto tab = TAB_INVALID;

  for (const auto &license : licenses)
  {
    auto licenseEdit = new QPlainTextEdit(license.text, _ui.tabs);
    licenseEdit->setReadOnly(true);
    const auto newTab = _ui.tabs->addTab(licenseEdit, license.title);
    if (tab == TAB_INVALID)
    {
      tab = newTab;
    }
  }

  if (tab != TAB_INVALID)
  {
    _ui.tabs->setCurrentIndex(tab);
  }
}

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
  }
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
}