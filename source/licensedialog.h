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
		         LicenseDialog(License *license, Settings *settings, QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~LicenseDialog();

    private:
		bool              _refreshed;
        License          *_license;
		Settings         *_settings;
        Ui::LicenseDialog _ui;

        const void fillLicenseInfo() const;

	private slots:
		const void on_eula_clicked(bool checked = false);
		const void on_load_clicked(bool checked = false);
		const void on_ok_clicked  (bool checked = false);
}; // LicenseDialog

#endif // LICENSEDIALOG_H