#include "settingsdialog.h"

#ifndef FREE
# include "settings/colordelegate.h"
#endif
#include <QtCore/QDir>

void SettingsDialog::accept()
{
    SaveOptions();

    QDialog::accept();
} // accept

#ifndef FREE
const void SettingsDialog::FillColorFlash()
{
    for (int iI = 0; iI < _usdSettingsDialog.qcbColorFlash->count(); iI++) {
        if (_usdSettingsDialog.qcbColorFlash->itemData(iI, Qt::UserRole).toString() == _sSettings->GetColorFlash()) {
            _usdSettingsDialog.qcbColorFlash->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // FillColorFlash

# ifdef Q_WS_WIN
const void SettingsDialog::FillHotkey(HotkeyLineEdit *pControl, const Settings::eHotkey &pHotkey) const
{
	Settings::sHotKeyInfo shkiHotkey = _sSettings->GetHotkey(pHotkey);
	pControl->setText(shkiHotkey.qsText);
	pControl->setProperty(PROPERTY_VIRTUALKEY, shkiHotkey.qui32VirtualKey);
} // FillHotkey
# endif
#endif

const void SettingsDialog::FillOptions()
{
    // general
#ifndef FREE
    _usdSettingsDialog.qcbHorizontalLayout->setChecked(_sSettings->GetHorizontalLayout());
#endif
    _usdSettingsDialog.qcbAlwaysOnTop->setChecked(_sSettings->GetAlwaysOnTop());
#ifndef FREE
    _usdSettingsDialog.qcbRememberWindowPosition->setChecked(_sSettings->GetRememberWindowPosition());
    _usdSettingsDialog.qcbSystemTrayIcon->setChecked(_sSettings->GetSystemTrayIcon());
	on_qcbSystemTrayIcon_stateChanged(_usdSettingsDialog.qcbSystemTrayIcon->checkState());
	_usdSettingsDialog.qcbShowWordsInTrayBalloon->setChecked(_sSettings->GetShowWordsInTrayBalloon());
	_usdSettingsDialog.qcbMinimizeToTray->setChecked(_sSettings->GetMinimizeToTray());
#endif
	FillTranslation();

    // learning
    _usdSettingsDialog.qsbWordsFrequency->setValue(_sSettings->GetWordsFrequency());
#ifndef FREE
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(_sSettings->GetWordsFrequency() - 1);
	_usdSettingsDialog.qsbWaitForAnswer->setValue(_sSettings->GetWaitForAnswer());
	_usdSettingsDialog.cbNewWordSound->setChecked(_sSettings->GetNewWordSound());
	_usdSettingsDialog.cbNewWordFlash->setChecked(_sSettings->GetNewWordFlash());
#endif
	_usdSettingsDialog.cbSwitchLearningDirection->setCheckState(_sSettings->GetSwitchLearningDirection());
#ifndef FREE
	_usdSettingsDialog.cbStartLearningOnStartup->setChecked(_sSettings->GetStartLearningOnStartup());
#endif

#ifndef FREE
    // appearance
    FillColorFlash();

# ifdef Q_WS_WIN
    // hotkeys
    // learning
	FillHotkey(_usdSettingsDialog.qleHotkeyNext, Settings::HotkeyNext);
    FillHotkey(_usdSettingsDialog.qleHotkeyAnswer, Settings::HotkeyAnswer);
    // window
	FillHotkey(_usdSettingsDialog.qleHotkeyMinimize, Settings::HotkeyMinimize);
	FillHotkey(_usdSettingsDialog.qleHotkeyRestore, Settings::HotkeyRestore);
# endif
#endif
} // FillOptions

const void SettingsDialog::FillTranslation()
{
	for (int iI = 0; iI < _usdSettingsDialog.qcbLanguage->count(); iI++) {
		if (_usdSettingsDialog.qcbLanguage->itemData(iI) == _sSettings->GetTranslation()) {
			_usdSettingsDialog.qcbLanguage->setCurrentIndex(iI);
			return;
		} // if
	} // for
} // FillTranslation

#ifndef FREE
const void SettingsDialog::on_qcbSystemTrayIcon_stateChanged(int state)
{
	_usdSettingsDialog.qcbMinimizeToTray->setEnabled(state == Qt::Checked);
	_usdSettingsDialog.qcbShowWordsInTrayBalloon->setEnabled(state == Qt::Checked);
} // on_qcbSystemTrayIcon_stateChanged

const void SettingsDialog::on_qsbWordsFrequency_valueChanged(int i)
{
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(i - 1);
} // on_qsbWordsFrequency_valueChanged

const void SettingsDialog::PrepareColorFlash()
{
	foreach (QString qsColor, QColor::colorNames()) {
		_usdSettingsDialog.qcbColorFlash->addItem(QString(), qsColor);
	} // foreach
} // PrepareColorFlash
#endif

const void SettingsDialog::PrepareTranslations()
{
    // add default language
    _usdSettingsDialog.qcbLanguage->addItem(tr("English"), QLocale(QLocale::English).name() + '.' + LANG_SUFFIX);

    // get installed languages
    QDir qdDir;
    qdDir.cd(DIR_LANG);
    QFileInfoList qfilFiles = qdDir.entryInfoList(QStringList() << "*." + LANG_SUFFIX, QDir::Files);

    foreach (QFileInfo qfiFile, qfilFiles) {
        QLocale qlLocale(qfiFile.completeBaseName());
        _usdSettingsDialog.qcbLanguage->addItem(QLocale::languageToString(qlLocale.language()) + " (" + QLocale::countryToString(qlLocale.country()) + ')', qfiFile.fileName());
    } // foreach
} // PrepareTranslations

#if !defined(FREE) && defined(Q_WS_WIN)
const void SettingsDialog::SaveHotkey(const HotkeyLineEdit *pControl, const Settings::eHotkey &pHotkey) const
{
	Settings::sHotKeyInfo shkiHotkey;

	shkiHotkey.qsText = pControl->text();
	shkiHotkey.qui32VirtualKey = pControl->property(PROPERTY_VIRTUALKEY).toUInt();

	_sSettings->SetHotkey(pHotkey, shkiHotkey);
} // SaveHotkey
#endif

const void SettingsDialog::SaveOptions()
{
    // general
#ifndef FREE
    _sSettings->SetHorizontalLayout(_usdSettingsDialog.qcbHorizontalLayout->isChecked());
#endif
    _sSettings->SetAlwaysOnTop(_usdSettingsDialog.qcbAlwaysOnTop->isChecked());
#ifndef FREE
    _sSettings->SetRememberWindowPosition(_usdSettingsDialog.qcbRememberWindowPosition->isChecked());
    _sSettings->SetSystemTrayIcon(_usdSettingsDialog.qcbSystemTrayIcon->isChecked());
	_sSettings->SetShowWordsInTrayBalloon(_usdSettingsDialog.qcbShowWordsInTrayBalloon->isChecked());
	_sSettings->SetMinimizeToTray(_usdSettingsDialog.qcbMinimizeToTray->isChecked());
#endif
	_sSettings->SetTranslation(_usdSettingsDialog.qcbLanguage->itemData(_usdSettingsDialog.qcbLanguage->currentIndex()).toString());

    // learning
    _sSettings->SetWordsFrequency(_usdSettingsDialog.qsbWordsFrequency->value());
#ifndef FREE
	_sSettings->SetWaitForAnswer(_usdSettingsDialog.qsbWaitForAnswer->value());
	_sSettings->SetNewWordSound(_usdSettingsDialog.cbNewWordSound->isChecked());
	_sSettings->SetNewWordFlash(_usdSettingsDialog.cbNewWordFlash->isChecked());
#endif
	_sSettings->SetSwitchLearningDirection(_usdSettingsDialog.cbSwitchLearningDirection->checkState());
#ifndef FREE
	_sSettings->SetStartLearningOnStartup(_usdSettingsDialog.cbStartLearningOnStartup->isChecked());
#endif

#ifndef FREE
    // appearance
    _sSettings->SetColorFlash(_usdSettingsDialog.qcbColorFlash->itemData(_usdSettingsDialog.qcbColorFlash->currentIndex()).toString());

# ifdef Q_WS_WIN
	// hotkeys
    // learning
	SaveHotkey(_usdSettingsDialog.qleHotkeyNext, Settings::HotkeyNext);
	SaveHotkey(_usdSettingsDialog.qleHotkeyAnswer, Settings::HotkeyAnswer);
    // window
	SaveHotkey(_usdSettingsDialog.qleHotkeyMinimize, Settings::HotkeyMinimize);
	SaveHotkey(_usdSettingsDialog.qleHotkeyRestore, Settings::HotkeyRestore);
# endif
#endif
} // SaveOptions

SettingsDialog::SettingsDialog(Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_sSettings = pSettings;

	_usdSettingsDialog.setupUi(this);
#ifdef FREE
    // general
    delete _usdSettingsDialog.qcbHorizontalLayout;
    delete _usdSettingsDialog.qcbRememberWindowPosition;
    delete _usdSettingsDialog.qcbSystemTrayIcon;
    delete _usdSettingsDialog.qcbShowWordsInTrayBalloon;
    delete _usdSettingsDialog.qcbMinimizeToTray;

    // learning
    delete _usdSettingsDialog.qlWaitForAnswer;
    delete _usdSettingsDialog.qsbWaitForAnswer;
    delete _usdSettingsDialog.cbNewWordSound;
    delete _usdSettingsDialog.cbNewWordFlash;
    delete _usdSettingsDialog.cbStartLearningOnStartup;

    // appearance
    _usdSettingsDialog.qtwTabs->removeTab(TabHotkey);
    _usdSettingsDialog.qtwTabs->removeTab(TabAppearance);
#elif !defined(Q_WS_WIN)
    _usdSettingsDialog.qtwTabs->removeTab(TabHotkey);
#else
	_usdSettingsDialog.qcbColorFlash->setItemDelegate(new ColorDelegate(_usdSettingsDialog.qcbColorFlash));

	PrepareColorFlash();
#endif
    PrepareTranslations();
    FillOptions();
} // SettingsDialog