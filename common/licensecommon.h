#ifndef LICENSECOMMON_H
#define LICENSECOMMON_H

class LicenseCommon
{
    public:
        struct sLicenseContent {
            QString qsTitle;
            QString qsText;
        }; // sLicenseContent

        typedef QList<sLicenseContent> tLicenseContentList;
}; // LicenseCommon

#endif // LICENSECOMMON_H