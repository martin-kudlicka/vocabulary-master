#include "settingsdialog.h"

SettingsDialog::SettingsDialog(Settings *pSettings, QWidget *parent /* 0 */, Qt::WindowFlags f /* 0 */) : QDialog(parent, f)
{
	_sSettings = pSettings;

	_usdSettingsDialog.setupUi(this);
} // SettingsDialog