#include "settingsdialog.h"

#include "settings/colordelegate.h"

void SettingsDialog::accept()
{
    SaveOptions();

    QDialog::accept();
} // accept

const void SettingsDialog::FillColorFlash()
{
    for (int iI = 0; iI < _usdSettingsDialog.qcbColorFlash->count(); iI++) {
        if (_usdSettingsDialog.qcbColorFlash->itemData(iI, Qt::UserRole).toString() == _sSettings->GetColorFlash()) {
            _usdSettingsDialog.qcbColorFlash->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // FillColorFlash

const void SettingsDialog::FillOptions()
{
    // general
    _usdSettingsDialog.qcbHorizontalLayout->setChecked(_sSettings->GetHorizontalLayout());
    _usdSettingsDialog.qcbAlwaysOnTop->setChecked(_sSettings->GetAlwaysOnTop());
    _usdSettingsDialog.qcbRememberWindowPosition->setChecked(_sSettings->GetRememberWindowPosition());
    _usdSettingsDialog.qcbSystemTrayIcon->setChecked(_sSettings->GetSystemTrayIcon());

    // learning
    _usdSettingsDialog.qsbWordsFrequency->setValue(_sSettings->GetWordsFrequency());
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(_sSettings->GetWordsFrequency() - 1);
	_usdSettingsDialog.qsbWaitForAnswer->setValue(_sSettings->GetWaitForAnswer());
	_usdSettingsDialog.cbNewWordSound->setChecked(_sSettings->GetNewWordSound());
	_usdSettingsDialog.cbNewWordFlash->setChecked(_sSettings->GetNewWordFlash());
	_usdSettingsDialog.cbSwitchLearningDirection->setCheckState(_sSettings->GetSwitchLearningDirection());
	_usdSettingsDialog.cbStartLearningOnStartup->setChecked(_sSettings->GetStartLearningOnStartup());

    // appearance
    _usdSettingsDialog.qsbFontSizeNote->setValue(_sSettings->GetFontSizeNote());
    _usdSettingsDialog.qsbFontSizeWord->setValue(_sSettings->GetFontSizeWord());
    FillColorFlash();
} // FillOptions

void SettingsDialog::on_qsbWordsFrequency_valueChanged(int i)
{
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(i - 1);
} // on_qsbWordsFrequency_valueChanged

const void SettingsDialog::PrepareColorFlash()
{
	foreach (QString qsColor, QColor::colorNames()) {
		_usdSettingsDialog.qcbColorFlash->addItem(QString(), qsColor);
	} // foreach
} // PrepareColorFlash

const void SettingsDialog::SaveOptions()
{
    // general
    _sSettings->SetHorizontalLayout(_usdSettingsDialog.qcbHorizontalLayout->isChecked());
    _sSettings->SetAlwaysOnTop(_usdSettingsDialog.qcbAlwaysOnTop->isChecked());
    _sSettings->SetRememberWindowPosition(_usdSettingsDialog.qcbRememberWindowPosition->isChecked());
    _sSettings->SetSystemTrayIcon(_usdSettingsDialog.qcbSystemTrayIcon->isChecked());

    // learning
    _sSettings->SetWordsFrequency(_usdSettingsDialog.qsbWordsFrequency->value());
	_sSettings->SetWaitForAnswer(_usdSettingsDialog.qsbWaitForAnswer->value());
	_sSettings->SetNewWordSound(_usdSettingsDialog.cbNewWordSound->isChecked());
	_sSettings->SetNewWordFlash(_usdSettingsDialog.cbNewWordFlash->isChecked());
	_sSettings->SetSwitchLearningDirection(_usdSettingsDialog.cbSwitchLearningDirection->checkState());
	_sSettings->SetStartLearningOnStartup(_usdSettingsDialog.cbStartLearningOnStartup->isChecked());

    // appearance
    _sSettings->SetFontSizeNote(_usdSettingsDialog.qsbFontSizeNote->value());
    _sSettings->SetFontSizeWord(_usdSettingsDialog.qsbFontSizeWord->value());
    _sSettings->SetColorFlash(_usdSettingsDialog.qcbColorFlash->itemData(_usdSettingsDialog.qcbColorFlash->currentIndex(), Qt::UserRole).toString());
} // SaveOptions

SettingsDialog::SettingsDialog(Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_sSettings = pSettings;

	_usdSettingsDialog.setupUi(this);
	_usdSettingsDialog.qcbColorFlash->setItemDelegate(new ColorDelegate(_usdSettingsDialog.qcbColorFlash));

	PrepareColorFlash();
    FillOptions();
} // SettingsDialog