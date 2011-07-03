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
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(_sSettings->GetWordsFrequency() - 1);
	_usdSettingsDialog.qsbWaitForAnswer->setValue(_sSettings->GetWaitForAnswer());
	_usdSettingsDialog.cbNewWordSound->setChecked(_sSettings->GetNewWordSound());
} // FillOptions

void SettingsDialog::on_qsbWordsFrequency_valueChanged(int i)
{
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(i - 1);
} // on_qsbWordsFrequency_valueChanged

const void SettingsDialog::SaveOptions()
{
    // general
    _sSettings->SetHorizontalLayout(_usdSettingsDialog.qcbHorizontalLayout->isChecked());
    _sSettings->SetAlwaysOnTop(_usdSettingsDialog.qcbAlwaysOnTop->isChecked());
    _sSettings->SetRememberWindowPosition(_usdSettingsDialog.qcbRememberWindowPosition->isChecked());

    // training
    _sSettings->SetWordsFrequency(_usdSettingsDialog.qsbWordsFrequency->value());
	_sSettings->SetWaitForAnswer(_usdSettingsDialog.qsbWaitForAnswer->value());
	_sSettings->SetNewWordSound(_usdSettingsDialog.cbNewWordSound->isChecked());
} // SaveOptions

SettingsDialog::SettingsDialog(Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_sSettings = pSettings;

	_usdSettingsDialog.setupUi(this);

    FillOptions();
} // SettingsDialog