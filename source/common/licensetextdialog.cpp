#include "common/licensetextdialog.h"

#include <QtGui/QPlainTextEdit>
#include <QtCore/QFile>

LicenseTextDialog::LicenseTextDialog(const LicenseCommon::tLicenseContentList &pLicenses, const Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
    _qdltLicenseText.setupUi(this);

    ShowMainLicense(pSettings);
    ShowLicenses(pLicenses);
} // LicenseTextDialog

const void LicenseTextDialog::ShowLicenses(const LicenseCommon::tLicenseContentList &pLicenses) const
{
    int iTab = TAB_INVALID;

    foreach (LicenseCommon::sLicenseContent slcLicense, pLicenses) {
        QPlainTextEdit *qpteLicense = new QPlainTextEdit(slcLicense.qsText, _qdltLicenseText.qtwTabs);
        int iNewTab = _qdltLicenseText.qtwTabs->addTab(qpteLicense, slcLicense.qsTitle);
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
    LicenseCommon::tLicenseContentList tlclLicenses;

    LicenseCommon::sLicenseContent slcLicense;
    slcLicense.qsTitle = "Vocabulary Master";
    QFile qfLicense;
    if (pSettings->GetTranslation() == "cs_CZ.qm") {
        qfLicense.setFileName(":/res/eula/cze.txt");
    } else {
        qfLicense.setFileName(":/res/eula/eng.txt");
    } // if else
    qfLicense.open(QIODevice::ReadOnly);
    slcLicense.qsText = qfLicense.readAll();
    tlclLicenses.append(slcLicense);

    ShowLicenses(tlclLicenses);
} // ShowMainLicense