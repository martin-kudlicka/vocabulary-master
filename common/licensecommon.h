#ifndef LICENSECOMMON_H
#define LICENSECOMMON_H

class LicenseCommon
{
  public:
    struct LicenseContent
    {
      QString title;
      QString text;
    };

    using LicenseContentList = QList<LicenseContent>;
};

#endif