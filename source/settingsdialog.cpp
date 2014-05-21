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
        if (_usdSettingsDialog.qcbColorFlash->itemData(iI, Qt::UserRole).toString() == _sSettings->colorFlash()) {
            _usdSettingsDialog.qcbColorFlash->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // FillColorFlash

# ifdef Q_OS_WIN
const void SettingsDialog::FillHotkey(HotkeyLineEdit *pControl, const Settings::Hotkey &pHotkey) const
{
	Settings::HotKeyInfo shkiHotkey = _sSettings->hotkey(pHotkey);
	pControl->setText(shkiHotkey.text);
	pControl->setProperty(PROPERTY_VIRTUALKEY, shkiHotkey.virtualKey);
} // FillHotkey
# endif
#endif

const void SettingsDialog::FillOptions()
{
    // general
#ifndef EDITION_FREE
    _usdSettingsDialog.qcbHorizontalLayout->setChecked(_sSettings->horizontalLayout());
#endif
    _usdSettingsDialog.qcbAlwaysOnTop->setChecked(_sSettings->alwaysOnTop());
#ifndef EDITION_FREE
    _usdSettingsDialog.qcbRememberWindowPosition->setChecked(_sSettings->rememberWindowPosition());
    _usdSettingsDialog.qcbSystemTrayIcon->setChecked(_sSettings->systemTrayIcon());
	on_qcbSystemTrayIcon_stateChanged(_usdSettingsDialog.qcbSystemTrayIcon->checkState());
	_usdSettingsDialog.qcbShowWordsInTrayBalloon->setChecked(_sSettings->showWordsInTrayBalloon());
	_usdSettingsDialog.qcbMinimizeToTray->setChecked(_sSettings->minimizeToTray());
#endif
	FillTranslation();
	_usdSettingsDialog.qcbUpdateCheck->setChecked(_sSettings->updateCheck());

    // learning
    _usdSettingsDialog.qsbWordsFrequency->setValue(_sSettings->wordsFrequency());
#ifndef EDITION_FREE
	_usdSettingsDialog.qsbWaitForAnswer->setMaximum(_sSettings->wordsFrequency() - 1);
	_usdSettingsDialog.qsbWaitForAnswer->setValue(_sSettings->waitForAnswer());
	_usdSettingsDialog.qcbNewWordSound->setChecked(_sSettings->newWordSound());
    if (_sSettings->newWordSoundType() == Settings::NewWordSoundTypeSystem) {
        _usdSettingsDialog.qrbSoundSystem->click();
    } else {
        _usdSettingsDialog.qrbSoundCustom->click();
    } // if else
    _usdSettingsDialog.qleSoundCustom->setText(_sSettings->newWordSoundFile());
	_usdSettingsDialog.qcbNewWordFlash->setChecked(_sSettings->newWordFlash());
	_usdSettingsDialog.qcbLearnDisabledWords->setChecked(_sSettings->learnDisabledWords());
#endif
	_usdSettingsDialog.qcbSwitchLearningDirection->setCheckState(_sSettings->switchLearningDirection());
#ifndef EDITION_FREE
	_usdSettingsDialog.qcbStartLearningOnStartup->setChecked(_sSettings->startLearningOnStartup());
#endif

#ifndef EDITION_FREE
    // appearance
	_usdSettingsDialog.qcbMainWindowToolBar->setChecked(_sSettings->showToolBar());
	_usdSettingsDialog.qcbMainWindowLanguageNames->setChecked(_sSettings->showLanguageNames());
	_usdSettingsDialog.qcbMainWindowCategoryName->setChecked(_sSettings->showCategoryName());
	_usdSettingsDialog.qcbMainWindowRecordControls->setChecked(_sSettings->showRecordControls());
	_usdSettingsDialog.qcbMainWindowStatusBar->setChecked(_sSettings->showStatusBar());
	FillColorFlash();
    _usdSettingsDialog.qcbVocabularyManagerCategoriesEnable->setChecked(_sSettings->canEnableCategories());
	_usdSettingsDialog.qcbVocabularyManagerCategoryPriority->setChecked(_sSettings->canChangeCategoryPriority());

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
	_usdSettingsDialog.qgbUseProxy->setChecked(_sSettings->useProxy());
	_usdSettingsDialog.qleProxyHostname->setText(_sSettings->proxyHostname());
	_usdSettingsDialog.qsbProxyPort->setValue(_sSettings->proxyPort());
	_usdSettingsDialog.qleProxyUsername->setText(_sSettings->proxyUsername());
	_usdSettingsDialog.qleProxyPassword->setText(_sSettings->proxyPassword());
	switch (_sSettings->proxyType()) {
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
		if (_usdSettingsDialog.qcbLanguage->itemData(iI) == _sSettings->translation()) {
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
        const Plugins::TTSPlugin stpPlugin = _pPlugins->ttsPlugins().at(iRow);
        tlclLicenses = stpPlugin.ttsInterface->licenseText();
    } // if

    // show license
    LicenseTextDialog ltdLicense(tlclLicenses, _sSettings, this);
    ltdLicense.exec();
} // on_qpbShowLicense_clicked
# endif

const void SettingsDialog::on_qbpSoundBrowse_clicked(bool checked /* false */)
{
    QString qsOldFile = _sSettings->newWordSoundFile();
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
const void SettingsDialog::SaveHotkey(const HotkeyLineEdit *pControl, const Settings::Hotkey &pHotkey) const
{
	Settings::HotKeyInfo shkiHotkey;

	shkiHotkey.text = pControl->text();
	shkiHotkey.virtualKey = pControl->property(PROPERTY_VIRTUALKEY).toUInt();

	_sSettings->setHotkey(pHotkey, shkiHotkey);
} // SaveHotkey
#endif

const void SettingsDialog::SaveOptions()
{
    // general
#ifndef EDITION_FREE
    _sSettings->setHorizontalLayout(_usdSettingsDialog.qcbHorizontalLayout->isChecked());
#endif
    _sSettings->setAlwaysOnTop(_usdSettingsDialog.qcbAlwaysOnTop->isChecked());
#ifndef EDITION_FREE
    _sSettings->setRememberWindowPosition(_usdSettingsDialog.qcbRememberWindowPosition->isChecked());
    _sSettings->setSystemTrayIcon(_usdSettingsDialog.qcbSystemTrayIcon->isChecked());
	_sSettings->setShowWordsInTrayBalloon(_usdSettingsDialog.qcbShowWordsInTrayBalloon->isChecked());
	_sSettings->setMinimizeToTray(_usdSettingsDialog.qcbMinimizeToTray->isChecked());
#endif
	_sSettings->setTranslation(_usdSettingsDialog.qcbLanguage->itemData(_usdSettingsDialog.qcbLanguage->currentIndex()).toString());
	_sSettings->setUpdateCheck(_usdSettingsDialog.qcbUpdateCheck->isChecked());

    // learning
    _sSettings->setWordsFrequency(_usdSettingsDialog.qsbWordsFrequency->value());
#ifndef EDITION_FREE
	_sSettings->setWaitForAnswer(_usdSettingsDialog.qsbWaitForAnswer->value());
	_sSettings->setNewWordSound(_usdSettingsDialog.qcbNewWordSound->isChecked());
    if (_usdSettingsDialog.qrbSoundSystem->isChecked()) {
        _sSettings->setNewWordSoundType(Settings::NewWordSoundTypeSystem);
    } else {
        _sSettings->setNewWordSoundType(Settings::NewWordSoundTypeCustom);
    } // if else
    _sSettings->setNewWordSoundFile(_usdSettingsDialog.qleSoundCustom->text());
	_sSettings->setNewWordFlash(_usdSettingsDialog.qcbNewWordFlash->isChecked());
	_sSettings->setLearnDisabledWords(_usdSettingsDialog.qcbLearnDisabledWords->isChecked());
#endif
	_sSettings->setSwitchLearningDirection(_usdSettingsDialog.qcbSwitchLearningDirection->checkState());
#ifndef EDITION_FREE
	_sSettings->setStartLearningOnStartup(_usdSettingsDialog.qcbStartLearningOnStartup->isChecked());
#endif

#ifndef EDITION_FREE
    // appearance
	_sSettings->setShowToolBar(_usdSettingsDialog.qcbMainWindowToolBar->isChecked());
	_sSettings->setShowLanguageNames(_usdSettingsDialog.qcbMainWindowLanguageNames->isChecked());
	_sSettings->setShowCategoryName(_usdSettingsDialog.qcbMainWindowCategoryName->isChecked());
	_sSettings->setShowRecordControls(_usdSettingsDialog.qcbMainWindowRecordControls->isChecked());
	_sSettings->setShowStatusBar(_usdSettingsDialog.qcbMainWindowStatusBar->isChecked());
	_sSettings->setColorFlash(_usdSettingsDialog.qcbColorFlash->itemData(_usdSettingsDialog.qcbColorFlash->currentIndex()).toString());
    _sSettings->setCanEnableCategories(_usdSettingsDialog.qcbVocabularyManagerCategoriesEnable->isChecked());
	_sSettings->setCanChangeCategoryPriority(_usdSettingsDialog.qcbVocabularyManagerCategoryPriority->isChecked());

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
	_sSettings->setUseProxy(_usdSettingsDialog.qgbUseProxy->isChecked());
	_sSettings->setProxyHostname(_usdSettingsDialog.qleProxyHostname->text());
	_sSettings->setProxyPort(_usdSettingsDialog.qsbProxyPort->value());
	_sSettings->setProxyUsername(_usdSettingsDialog.qleProxyUsername->text());
	_sSettings->setProxyPassword(_usdSettingsDialog.qleProxyPassword->text());
	if (_usdSettingsDialog.qrbProxyTypeHttp->isChecked()) {
		_sSettings->setProxyType(QNetworkProxy::HttpProxy);
	} else {
		if (_usdSettingsDialog.qrbProxyTypeSocks5->isChecked()) {
			_sSettings->setProxyType(QNetworkProxy::Socks5Proxy);
		} else {
			_sSettings->setProxyType(QNetworkProxy::HttpCachingProxy);
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