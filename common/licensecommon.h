#ifndef LICENSECOMMON_H
#define LICENSECOMMON_H

class LicenseCommon
{
    public:
        struct LicenseContent
		{
            QString title;
            QString text;
        }; // LicenseContent

        typedef QList<LicenseContent> LicenseContentList;
}; // LicenseCommon

#endif // LICENSECOMMON_H