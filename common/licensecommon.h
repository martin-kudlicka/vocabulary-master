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

    typedef QList<LicenseContent> LicenseContentList;
};

#endif