#include "settingsdialog.h"

#ifndef EDITION_FREE
# include "settings/colordelegate.h"
#endif
#include <QtCore/QDir>
#ifndef EDITION_FREE
# include "common/licensetextdialog.h"
# include <QtWidgets/QFileDialog>
#endif

void SettingsDialog::accept()
{
    SaveOptions();

    QDialog::accept();
} // accept

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
const void SettingsDialog::ClearHotkey(HotkeyLineEdit *pControl) const
{
	pControl->clear();
	pControl->setProperty(PROPERTY_VIRTUALKEY, VIRTUALKEY_NONE);
} // ClearHotkey
# endif

const void SettingsDialog::FillColorFlash()
{
    for (int iI = 0; iI < _usdSettingsDialog.qcbColorFlash->count(); iI++) {
        if (_usdSettingsDialog.qcbColorFlash->itemData(iI, Qt::UserRole).toString() == _sSettings->GetColorFlash()) {
            _usdSettingsDialog.qcbColorFlash->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // FillColorFlash

# ifdef Q_OS_WIN
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
#ifndef EDITION_FREE
    _usdSettingsDialog.qcbHorizontalLayout->setChecked(_sSettings->GetHorizontalLayout());
#endif
    _usdSettingsDialog.qcbAlwaysOnTop->setChecked(_sSettings->GetAlwaysOnTop());
#ifndef EDITION_FREE
    _usdSettingsDialog.qcbRememberWindowPosition->setChecked(_sSettings->GetRememberWindowPosition());
    _usdSettingsDialog.qcbSystemTrayIcon->setChecked(_sSettings->GetSystemTrayIcon());
	on_qcbSystemTrayIcon_stateChanged(_usdSettingsDialog.qcbSystemTrayIcon->checkState());
	_usdSettingsDialog.qcbShowWordsInTrayBalloon->setChecked(_sSettings->GetShowWordsInTrayBalloon());
	_usdSettingsDialog.qcbMinimizeToTray->setChecked(_sSettings->GetMinimizeToTray());
#endif
	FillTranslation();
	_usdSettingsDialog.qcbUpdateCheck->setChecked(_sSettings->GetUpdateCheck());

    // learning
    _usdSettingsDialog.qsbWordsFrequency->setValue(_sSettings->GetWordsFrequency());
#ifndef EDITION_FREE
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(_sSettings->GetWordsFrequency() - 1);
	_usdSettingsDialog.qsbWaitForAnswer->setValue(_sSettings->GetWaitForAnswer());
	_usdSettingsDialog.qcbNewWordSound->setChecked(_sSettings->GetNewWordSound());
    if (_sSettings->GetNewWordSoundType() == Settings::NewWordSoundTypeSystem) {
        _usdSettingsDialog.qrbSoundSystem->click();
    } else {
        _usdSettingsDialog.qrbSoundCustom->click();
    } // if else
    _usdSettingsDialog.qleSoundCustom->setText(_sSettings->GetNewWordSoundFile());
	_usdSettingsDialog.qcbNewWordFlash->setChecked(_sSettings->GetNewWordFlash());
	_usdSettingsDialog.qcbLearnDisabledWords->setChecked(_sSettings->GetLearnDisabledWords());
#endif
	_usdSettingsDialog.qcbSwitchLearningDirection->setCheckState(_sSettings->GetSwitchLearningDirection());
#ifndef EDITION_FREE
	_usdSettingsDialog.qcbStartLearningOnStartup->setChecked(_sSettings->GetStartLearningOnStartup());
#endif

#ifndef EDITION_FREE
    // appearance
	_usdSettingsDialog.qcbMainWindowToolBar->setChecked(_sSettings->GetShowToolBar());
	_usdSettingsDialog.qcbMainWindowLanguageNames->setChecked(_sSettings->GetShowLanguageNames());
	_usdSettingsDialog.qcbMainWindowCategoryName->setChecked(_sSettings->GetShowCategoryName());
	_usdSettingsDialog.qcbMainWindowRecordControls->setChecked(_sSettings->GetShowRecordControls());
	_usdSettingsDialog.qcbMainWindowStatusBar->setChecked(_sSettings->GetShowStatusBar());
	FillColorFlash();
    _usdSettingsDialog.qcbVocabularyManagerCategoriesEnable->setChecked(_sSettings->GetCanEnableCategories());
	_usdSettingsDialog.qcbVocabularyManagerCategoryPriority->setChecked(_sSettings->GetCanChangeCategoryPriority());

# ifdef Q_OS_WIN
    // hotkeys
    // learning
	FillHotkey(_usdSettingsDialog.qleHotkeyNext, Settings::HotkeyNext);
    FillHotkey(_usdSettingsDialog.qleHotkeyAnswer, Settings::HotkeyAnswer);
    // window
	FillHotkey(_usdSettingsDialog.qleHotkeyMinimize, Settings::HotkeyMinimize);
	FillHotkey(_usdSettingsDialog.qleHotkeyRestore, Settings::HotkeyRestore);
# endif

    // plugins
    PreparePlugins(_usdSettingsDialog.qtvPluginsImp, &_pmImpPlugins);
    PreparePlugins(_usdSettingsDialog.qtvPluginsExp, &_pmExpPlugins);
    PreparePlugins(_usdSettingsDialog.qtvPluginsTTS, &_pmTTSPlugins);
#endif

	// network
	_usdSettingsDialog.qgbUseProxy->setChecked(_sSettings->GetUseProxy());
	_usdSettingsDialog.qleProxyHostname->setText(_sSettings->GetProxyHostname());
	_usdSettingsDialog.qsbProxyPort->setValue(_sSettings->GetProxyPort());
	_usdSettingsDialog.qleProxyUsername->setText(_sSettings->GetProxyUsername());
	_usdSettingsDialog.qleProxyPassword->setText(_sSettings->GetProxyPassword());
	switch (_sSettings->GetProxyType()) {
		case QNetworkProxy::HttpProxy:
			_usdSettingsDialog.qrbProxyTypeHttp->setChecked(true);
			break;
		case QNetworkProxy::Socks5Proxy:
			_usdSettingsDialog.qrbProxyTypeSocks5->setChecked(true);
			break;
		case QNetworkProxy::HttpCachingProxy:
			_usdSettingsDialog.qrbProxyTypeCachingOnlyHttp->setChecked(true);
	} // switch
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

#ifndef EDITION_FREE
const void SettingsDialog::on_qcbNewWordSound_stateChanged(int state) const
{
    _usdSettingsDialog.qrbSoundSystem->setEnabled(state == Qt::Checked);
    _usdSettingsDialog.qrbSoundCustom->setEnabled(state == Qt::Checked);
    on_qrbSoundCustom_clicked(state == Qt::Checked && _usdSettingsDialog.qrbSoundCustom->isChecked());
} // on_qcbNewWordSound_stateChanged

const void SettingsDialog::on_qcbSystemTrayIcon_stateChanged(int state) const
{
	_usdSettingsDialog.qcbMinimizeToTray->setEnabled(state == Qt::Checked);
	_usdSettingsDialog.qcbShowWordsInTrayBalloon->setEnabled(state == Qt::Checked);
} // on_qcbSystemTrayIcon_stateChanged

# ifdef Q_OS_WIN
const void SettingsDialog::on_qpbHotkeyAnswerClear_clicked(bool checked /* false */) const
{
	ClearHotkey(_usdSettingsDialog.qleHotkeyAnswer);
} // on_qpbHotkeyAnswerClear_clicked

const void SettingsDialog::on_qpbHotkeyMinimizeClear_clicked(bool checked /* false */) const
{
	ClearHotkey(_usdSettingsDialog.qleHotkeyMinimize);
} // on_qpbHotkeyMinimizeClear_clicked

const void SettingsDialog::on_qpbHotkeyNextClear_clicked(bool checked /* false */) const
{
	ClearHotkey(_usdSettingsDialog.qleHotkeyNext);
} // on_qpbHotkeyNextClear_clicked

const void SettingsDialog::on_qpbHotkeyRestoreClear_clicked(bool checked /* false */) const
{
	ClearHotkey(_usdSettingsDialog.qleHotkeyRestore);
} // on_qpbHotkeyRestoreClear_clicked

const void SettingsDialog::on_qpbShowLicense_clicked(bool checked /* false */)
{
    const QPushButton *qpbButton = qobject_cast<const QPushButton *>(sender());
    const QTreeView *qtvTreeView = qobject_cast<const QTreeView *>(qpbButton->parent()->parent());

    // find plugin
    int iRow;
    const PluginsModel *pmModel = qobject_cast<const PluginsModel *>(qtvTreeView->model());
    for (iRow = 0; iRow < pmModel->rowCount(); iRow++) {
        QModelIndex qmiIndex = pmModel->index(iRow, PluginsModel::ColumnLicense);
        if (qtvTreeView->indexWidget(qmiIndex) == qpbButton) {
            break;
        } // if
    } // for

    // license
    LicenseCommon::LicenseContentList tlclLicenses;
    if (qtvTreeView == _usdSettingsDialog.qtvPluginsTTS) {
        const Plugins::sTTSPlugin stpPlugin = _pPlugins->GetTTSPlugins().at(iRow);
        tlclLicenses = stpPlugin.tiInterface->licenseText();
    } // if

    // show license
    LicenseTextDialog ltdLicense(tlclLicenses, _sSettings, this);
    ltdLicense.exec();
} // on_qpbShowLicense_clicked
# endif

const void SettingsDialog::on_qbpSoundBrowse_clicked(bool checked /* false */)
{
    QString qsOldFile = _sSettings->GetNewWordSoundFile();
    QString qsFile = QFileDialog::getOpenFileName(this, tr("Sound file"), QFileInfo(qsOldFile).path()
#ifdef Q_OS_WIN
        , tr("sound files (*.wav)")
#endif
        );
    if (!qsFile.isEmpty()) {
        _usdSettingsDialog.qleSoundCustom->setText(qsFile);
    } // if
} // on_qbpSoundBrowse_clicked

const void SettingsDialog::on_qrbSoundCustom_clicked(bool checked /* false */) const
{
    _usdSettingsDialog.qleSoundCustom->setEnabled(checked);
    _usdSettingsDialog.qbpSoundBrowse->setEnabled(checked);
} // on_qrbSoundCustom_clicked

const void SettingsDialog::on_qrbSoundSystem_clicked(bool checked /* false */) const
{
    on_qrbSoundCustom_clicked(false);
} // on_qrbSoundSystem_clicked

const void SettingsDialog::on_qsbWordsFrequency_valueChanged(int i) const
{
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(i - 1);
} // on_qsbWordsFrequency_valueChanged

const void SettingsDialog::PrepareColorFlash()
{
	foreach (QString qsColor, QColor::colorNames()) {
		_usdSettingsDialog.qcbColorFlash->addItem(QString(), qsColor);
	} // foreach
} // PrepareColorFlash

const void SettingsDialog::PreparePlugins(QTreeView *pTreeView, PluginsModel *pModel) const
{
    pTreeView->setModel(pModel);
    for (int iRow = 0; iRow < pModel->rowCount(); iRow++) {
        QModelIndex qmiIndex = pModel->index(iRow, PluginsModel::ColumnLicense);

        QPushButton *qpbShow = new QPushButton(tr("Show"), pTreeView);
        pTreeView->setIndexWidget(qmiIndex, qpbShow);

        connect(qpbShow, SIGNAL(clicked(bool)), SLOT(on_qpbShowLicense_clicked(bool)));
    } // for
    pTreeView->header()->setSectionResizeMode(PluginsModel::ColumnName, QHeaderView::Stretch);
    pTreeView->header()->setSectionResizeMode(PluginsModel::ColumnLicense, QHeaderView::ResizeToContents);
} // PreparePlugins
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

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
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
#ifndef EDITION_FREE
    _sSettings->SetHorizontalLayout(_usdSettingsDialog.qcbHorizontalLayout->isChecked());
#endif
    _sSettings->SetAlwaysOnTop(_usdSettingsDialog.qcbAlwaysOnTop->isChecked());
#ifndef EDITION_FREE
    _sSettings->SetRememberWindowPosition(_usdSettingsDialog.qcbRememberWindowPosition->isChecked());
    _sSettings->SetSystemTrayIcon(_usdSettingsDialog.qcbSystemTrayIcon->isChecked());
	_sSettings->SetShowWordsInTrayBalloon(_usdSettingsDialog.qcbShowWordsInTrayBalloon->isChecked());
	_sSettings->SetMinimizeToTray(_usdSettingsDialog.qcbMinimizeToTray->isChecked());
#endif
	_sSettings->SetTranslation(_usdSettingsDialog.qcbLanguage->itemData(_usdSettingsDialog.qcbLanguage->currentIndex()).toString());
	_sSettings->SetUpdateCheck(_usdSettingsDialog.qcbUpdateCheck->isChecked());

    // learning
    _sSettings->SetWordsFrequency(_usdSettingsDialog.qsbWordsFrequency->value());
#ifndef EDITION_FREE
	_sSettings->SetWaitForAnswer(_usdSettingsDialog.qsbWaitForAnswer->value());
	_sSettings->SetNewWordSound(_usdSettingsDialog.qcbNewWordSound->isChecked());
    if (_usdSettingsDialog.qrbSoundSystem->isChecked()) {
        _sSettings->SetNewWordSoundType(Settings::NewWordSoundTypeSystem);
    } else {
        _sSettings->SetNewWordSoundType(Settings::NewWordSoundTypeCustom);
    } // if else
    _sSettings->SetNewWordSoundFile(_usdSettingsDialog.qleSoundCustom->text());
	_sSettings->SetNewWordFlash(_usdSettingsDialog.qcbNewWordFlash->isChecked());
	_sSettings->SetLearnDisabledWords(_usdSettingsDialog.qcbLearnDisabledWords->isChecked());
#endif
	_sSettings->SetSwitchLearningDirection(_usdSettingsDialog.qcbSwitchLearningDirection->checkState());
#ifndef EDITION_FREE
	_sSettings->SetStartLearningOnStartup(_usdSettingsDialog.qcbStartLearningOnStartup->isChecked());
#endif

#ifndef EDITION_FREE
    // appearance
	_sSettings->SetShowToolBar(_usdSettingsDialog.qcbMainWindowToolBar->isChecked());
	_sSettings->SetShowLanguageNames(_usdSettingsDialog.qcbMainWindowLanguageNames->isChecked());
	_sSettings->SetShowCategoryName(_usdSettingsDialog.qcbMainWindowCategoryName->isChecked());
	_sSettings->SetShowRecordControls(_usdSettingsDialog.qcbMainWindowRecordControls->isChecked());
	_sSettings->SetShowStatusBar(_usdSettingsDialog.qcbMainWindowStatusBar->isChecked());
	_sSettings->SetColorFlash(_usdSettingsDialog.qcbColorFlash->itemData(_usdSettingsDialog.qcbColorFlash->currentIndex()).toString());
    _sSettings->SetCanEnableCategories(_usdSettingsDialog.qcbVocabularyManagerCategoriesEnable->isChecked());
	_sSettings->SetCanChangeCategoryPriority(_usdSettingsDialog.qcbVocabularyManagerCategoryPriority->isChecked());

# ifdef Q_OS_WIN
	// hotkeys
    // learning
	SaveHotkey(_usdSettingsDialog.qleHotkeyNext, Settings::HotkeyNext);
	SaveHotkey(_usdSettingsDialog.qleHotkeyAnswer, Settings::HotkeyAnswer);
    // window
	SaveHotkey(_usdSettingsDialog.qleHotkeyMinimize, Settings::HotkeyMinimize);
	SaveHotkey(_usdSettingsDialog.qleHotkeyRestore, Settings::HotkeyRestore);
# endif
#endif

	// network
	_sSettings->SetUseProxy(_usdSettingsDialog.qgbUseProxy->isChecked());
	_sSettings->SetProxyHostname(_usdSettingsDialog.qleProxyHostname->text());
	_sSettings->SetProxyPort(_usdSettingsDialog.qsbProxyPort->value());
	_sSettings->SetProxyUsername(_usdSettingsDialog.qleProxyUsername->text());
	_sSettings->SetProxyPassword(_usdSettingsDialog.qleProxyPassword->text());
	if (_usdSettingsDialog.qrbProxyTypeHttp->isChecked()) {
		_sSettings->SetProxyType(QNetworkProxy::HttpProxy);
	} else {
		if (_usdSettingsDialog.qrbProxyTypeSocks5->isChecked()) {
			_sSettings->SetProxyType(QNetworkProxy::Socks5Proxy);
		} else {
			_sSettings->SetProxyType(QNetworkProxy::HttpCachingProxy);
		} // if else
	} // if else
} // SaveOptions

SettingsDialog::SettingsDialog(
#ifndef EDITION_FREE
    const Plugins *pPlugins,
#endif
    Settings *pSettings, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
#ifndef EDITION_FREE
    , _pmExpPlugins(pPlugins, PluginsModel::PluginTypeExp), _pmImpPlugins(pPlugins, PluginsModel::PluginTypeImp), _pmTTSPlugins(pPlugins, PluginsModel::PluginTypeTTS)
#endif
{
#ifndef EDITION_FREE
    _pPlugins = pPlugins;
#endif
	_sSettings = pSettings;

	_usdSettingsDialog.setupUi(this);
#ifdef EDITION_FREE
    // general
    delete _usdSettingsDialog.qcbHorizontalLayout;
    delete _usdSettingsDialog.qcbRememberWindowPosition;
    delete _usdSettingsDialog.qcbSystemTrayIcon;
    delete _usdSettingsDialog.qcbShowWordsInTrayBalloon;
    delete _usdSettingsDialog.qcbMinimizeToTray;

    // learning
    delete _usdSettingsDialog.qlWaitForAnswer;
    delete _usdSettingsDialog.qsbWaitForAnswer;
    delete _usdSettingsDialog.qcbNewWordSound;
    delete _usdSettingsDialog.qrbSoundSystem;
    delete _usdSettingsDialog.qrbSoundCustom;
    delete _usdSettingsDialog.qleSoundCustom;
    delete _usdSettingsDialog.qbpSoundBrowse;
    delete _usdSettingsDialog.qcbNewWordFlash;
	delete _usdSettingsDialog.qcbLearnDisabledWords;
    delete _usdSettingsDialog.qcbStartLearningOnStartup;

	// plugins
	_usdSettingsDialog.qtwTabs->removeTab(TabPlugins);

    // appearance
    _usdSettingsDialog.qtwTabs->removeTab(TabHotkey);
    _usdSettingsDialog.qtwTabs->removeTab(TabAppearance);
#elif !defined(Q_OS_WIN)
    _usdSettingsDialog.qtwTabs->removeTab(TabHotkey);
#else
	_usdSettingsDialog.qcbColorFlash->setItemDelegate(new ColorDelegate(_usdSettingsDialog.qcbColorFlash));

	PrepareColorFlash();
#endif
    PrepareTranslations();
    FillOptions();
} // SettingsDialog