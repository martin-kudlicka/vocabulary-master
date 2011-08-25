#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QtGui/QDialog>
#include <ui_licensedialog.h>

#include "license.h"

class LicenseDialog : public QDialog
{
	public:
		LicenseDialog(const License *pLicense, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        const License *_lLicense;
        Ui::qdLicense _qdlLicenseDialog;

        const void FillLicenseInfo() const;
}; // LicenseDialog

#endif // LICENSEDIALOG_H