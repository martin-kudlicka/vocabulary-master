#include "common/licensetextdialog.h"

#include <QtWidgets/QPlainTextEdit>
#include <QtCore/QFile>

LicenseTextDialog::LicenseTextDialog(const LicenseCommon::LicenseContentList &pLicenses, const Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _qdltLicenseText.setupUi(this);

    ShowMainLicense(pSettings);
    ShowLicenses(pLicenses);
} // LicenseTextDialog

const void LicenseTextDialog::ShowLicenses(const LicenseCommon::LicenseContentList &pLicenses) const
{
    int iTab = TAB_INVALID;

    foreach (LicenseCommon::LicenseContent slcLicense, pLicenses) {
        QPlainTextEdit *qpteLicense = new QPlainTextEdit(slcLicense.text, _qdltLicenseText.qtwTabs);
        qpteLicense->setReadOnly(true);
        int iNewTab = _qdltLicenseText.qtwTabs->addTab(qpteLicense, slcLicense.title);
        if (iTab == TAB_INVALID) {
            iTab = iNewTab;
        } // if
    } // foreach

    if (iTab != TAB_INVALID) {
        _qdltLicenseText.qtwTabs->setCurrentIndex(iTab);
    } // if
} // ShowLicenses

const void LicenseTextDialog::ShowMainLicense(const Settings *pSettings) const
{
    LicenseCommon::LicenseContentList tlclLicenses;

	// EULA
    LicenseCommon::LicenseContent slcLicense;
    slcLicense.title = "Vocabulary Master";
    QFile qfLicense;
    if (pSettings->GetTranslation() == "cs_CZ.qm") {
        qfLicense.setFileName(":/res/eula/cze.txt");
    } else {
        qfLicense.setFileName(":/res/eula/eng.txt");
    } // if else
    qfLicense.open(QIODevice::ReadOnly);
    slcLicense.text = qfLicense.readAll();
	qfLicense.close();
    tlclLicenses.append(slcLicense);

	// fugue-icons
	slcLicense.title = "Fugue Icons";
	qfLicense.setFileName(":/res/eula/fugue-icons/README.txt");
	qfLicense.open(QIODevice::ReadOnly);
	slcLicense.text = qfLicense.readAll();
	tlclLicenses.append(slcLicense);

    ShowLicenses(tlclLicenses);
} // ShowMainLicense