#include "settingsdialog.h"

void SettingsDialog::accept()
{
    SaveOptions();

    QDialog::accept();
} // accept

const void SettingsDialog::FillOptions()
{
    // general
    _usdSettingsDialog.qcbHorizontalLayout->setChecked(_sSettings->GetHorizontalLayout());
    _usdSettingsDialog.qcbAlwaysOnTop->setChecked(_sSettings->GetAlwaysOnTop());
    _usdSettingsDialog.qcbRememberWindowPosition->setChecked(_sSettings->GetRememberWindowPosition());

    // training
    _usdSettingsDialog.qsbWordsFrequency->setValue(_sSettings->GetWordsFrequency());
} // FillOptions

const void SettingsDialog::SaveOptions()
{
    // general
    _sSettings->SetHorizontalLayout(_usdSettingsDialog.qcbHorizontalLayout->isChecked());
    _sSettings->SetAlwaysOnTop(_usdSettingsDialog.qcbAlwaysOnTop->isChecked());
    _sSettings->SetRememberWindowPosition(_usdSettingsDialog.qcbRememberWindowPosition->isChecked());

    // training
    _sSettings->SetWordsFrequency(_usdSettingsDialog.qsbWordsFrequency->value());
} // SaveOptions

SettingsDialog::SettingsDialog(Settings *pSettings, QWidget *parent /* 0 */, Qt::WindowFlags f /* 0 */) : QDialog(parent, f)
{
	_sSettings = pSettings;

	_usdSettingsDialog.setupUi(this);

    FillOptions();
} // SettingsDialog