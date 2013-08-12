#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QtWidgets/QDialog>
#include <ui_licensedialog.h>

#include "license.h"
#include "settings.h"

class LicenseDialog : public QDialog
{
	Q_OBJECT

	public:
		LicenseDialog(License *pLicense, Settings *pSettings, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
		bool _bRefreshed;
        License *_lLicense;
		Settings *_sSettings;
        Ui::qdLicense _qdlLicenseDialog;

        const void FillLicenseInfo() const;

	private slots:
		const void on_qpbEULA_clicked(bool checked = false);
		const void on_qpbLoad_clicked(bool checked = false);
		const void on_qpbOk_clicked(bool checked = false);
}; // LicenseDialog

#endif // LICENSEDIALOG_H