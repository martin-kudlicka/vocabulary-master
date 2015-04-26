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
		         LicenseDialog(License *license, Settings *settings, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
		virtual ~LicenseDialog() override;

    private:
		bool              _refreshed;
        License          *_license;
		Settings         *_settings;
        Ui::LicenseDialog _ui;

        void fillLicenseInfo() const;

	private slots:
		void on_eula_clicked(bool checked = false);
		void on_load_clicked(bool checked = false);
		void on_ok_clicked  (bool checked = false);
}; // LicenseDialog

#endif // LICENSEDIALOG_H