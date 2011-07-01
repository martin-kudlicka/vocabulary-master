#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>
#include "ui_settingsdialog.h"

#include "settings.h"

class SettingsDialog : public QDialog
{
	Q_OBJECT

	public:
		SettingsDialog(Settings *pSettings, QWidget *parent = 0, Qt::WindowFlags f = 0);

	private:
		Settings *_sSettings;
		Ui::qdUiSettings _usdSettingsDialog;

        void accept();
        const void FillOptions();
        const void SaveOptions();
}; // MainWindow

#endif // SETTINGSDIALOG_H