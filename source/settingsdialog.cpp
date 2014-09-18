#include "settingsdialog.h"

#ifndef EDITION_FREE
# include "settings/colordelegate.h"
#endif
#include <QtCore/QDir>
#ifndef EDITION_FREE
# include "common/licensetextdialog.h"
# include <QtWidgets/QFileDialog>
#endif

SettingsDialog::SettingsDialog(
#ifndef EDITION_FREE
    const Plugins *plugins,
#endif
    Settings *settings, QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags)
#ifndef EDITION_FREE
    , _expPluginsModel(plugins, PluginsModel::PluginTypeExp), _impPluginsModel(plugins, PluginsModel::PluginTypeImp), _ttsPluginsModel(plugins, PluginsModel::PluginTypeTTS)
#endif
{
#ifndef EDITION_FREE
    _plugins = plugins;
#endif
	_settings = settings;

	_ui.setupUi(this);
#ifdef EDITION_FREE
    // general
    delete _ui.qcbHorizontalLayout;
    delete _ui.qcbRememberWindowPosition;
    delete _ui.qcbSystemTrayIcon;
    delete _ui.qcbShowWordsInTrayBalloon;
    delete _ui.qcbMinimizeToTray;

    // learning
    delete _ui.qlWaitForAnswer;
    delete _ui.qsbWaitForAnswer;
    delete _ui.qcbNewWordSound;
    delete _ui.qrbSoundSystem;
    delete _ui.qrbSoundCustom;
    delete _ui.qleSoundCustom;
    delete _ui.qbpSoundBrowse;
    delete _ui.qcbNewWordFlash;
	delete _ui.qcbLearnDisabledWords;
    delete _ui.qcbStartLearningOnStartup;

	// plugins
	_ui.qtwTabs->removeTab(TabPlugins);

    // appearance
    _ui.qtwTabs->removeTab(TabHotkey);
    _ui.qtwTabs->removeTab(TabAppearance);
#elif !defined(Q_OS_WIN)
    _ui.qtwTabs->removeTab(TabHotkey);
#else
	_ui.qcbColorFlash->setItemDelegate(new ColorDelegate(_ui.qcbColorFlash));

	prepareColorFlash();
#endif
    prepareTranslations();
    fillOptions();
} // SettingsDialog

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    saveOptions();

    QDialog::accept();
} // accept

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
void SettingsDialog::clearHotkey(HotkeyLineEdit *pControl) const
{
	pControl->clear();
	pControl->setProperty(PROPERTY_VIRTUALKEY, VIRTUALKEY_NONE);
} // clearHotkey
# endif

void SettingsDialog::fillColorFlash()
{
    for (int iI = 0; iI < _ui.qcbColorFlash->count(); iI++) {
        if (_ui.qcbColorFlash->itemData(iI, Qt::UserRole).toString() == _settings->colorFlash()) {
            _ui.qcbColorFlash->setCurrentIndex(iI);
            return;
        } // if
    } // for
} // fillColorFlash

# ifdef Q_OS_WIN
void SettingsDialog::fillHotkey(HotkeyLineEdit *pControl, Settings::Hotkey pHotkey) const
{
	Settings::HotkeyInfo shkiHotkey = _settings->hotkey(pHotkey);
	pControl->setText(shkiHotkey.text);
	pControl->setProperty(PROPERTY_VIRTUALKEY, shkiHotkey.virtualKey);
} // FillHotkey
# endif
#endif

void SettingsDialog::fillOptions()
{
    // general
#ifndef EDITION_FREE
    _ui.qcbHorizontalLayout->setChecked(_settings->horizontalLayout());
#endif
    _ui.qcbAlwaysOnTop->setChecked(_settings->alwaysOnTop());
#ifndef EDITION_FREE
    _ui.qcbRememberWindowPosition->setChecked(_settings->rememberWindowPosition());
    _ui.systemTrayIcon->setChecked(_settings->systemTrayIcon());
	on_systemTrayIcon_stateChanged(_ui.systemTrayIcon->checkState());
	_ui.qcbShowWordsInTrayBalloon->setChecked(_settings->showWordsInTrayBalloon());
	_ui.qcbMinimizeToTray->setChecked(_settings->minimizeToTray());
#endif
	fillTranslation();
	_ui.qcbUpdateCheck->setChecked(_settings->updateCheck());

    // learning
    _ui.wordsFrequency->setValue(_settings->wordsFrequency());
#ifndef EDITION_FREE
	_ui.qsbWaitForAnswer->setMaximum(_settings->wordsFrequency() - 1);
	_ui.qsbWaitForAnswer->setValue(_settings->waitForAnswer());
	_ui.newWordSound->setChecked(_settings->newWordSound());
    if (_settings->newWordSoundType() == Settings::NewWordSoundTypeSystem) {
        _ui.soundSystem->click();
    } else {
        _ui.soundCustomRadio->click();
    } // if else
    _ui.qleSoundCustom->setText(_settings->newWordSoundFile());
	_ui.qcbNewWordFlash->setChecked(_settings->newWordFlash());
	_ui.qcbLearnDisabledWords->setChecked(_settings->learnDisabledWords());
#endif
	_ui.qcbSwitchLearningDirection->setCheckState(_settings->switchLearningDirection());
#ifndef EDITION_FREE
	_ui.qcbStartLearningOnStartup->setChecked(_settings->startLearningOnStartup());
#endif

#ifndef EDITION_FREE
    // appearance
	_ui.qcbMainWindowToolBar->setChecked(_settings->showToolBar());
	_ui.qcbMainWindowLanguageNames->setChecked(_settings->showLanguageNames());
	_ui.qcbMainWindowCategoryName->setChecked(_settings->showCategoryName());
	_ui.qcbMainWindowRecordControls->setChecked(_settings->showRecordControls());
	_ui.qcbMainWindowStatusBar->setChecked(_settings->showStatusBar());
	fillColorFlash();
    _ui.qcbVocabularyManagerCategoriesEnable->setChecked(_settings->canEnableCategories());
	_ui.qcbVocabularyManagerCategoryPriority->setChecked(_settings->canChangeCategoryPriority());

# ifdef Q_OS_WIN
    // hotkeys
    // learning
	fillHotkey(_ui.qleHotkeyNext, Settings::HotkeyNext);
    fillHotkey(_ui.qleHotkeyAnswer, Settings::HotkeyAnswer);
    // window
	fillHotkey(_ui.qleHotkeyMinimize, Settings::HotkeyMinimize);
	fillHotkey(_ui.qleHotkeyRestore, Settings::HotkeyRestore);
# endif

    // plugins
    preparePlugins(_ui.qtvPluginsImp, &_impPluginsModel);
    preparePlugins(_ui.qtvPluginsExp, &_expPluginsModel);
    preparePlugins(_ui.qtvPluginsTTS, &_ttsPluginsModel);
#endif

	// network
	_ui.qgbUseProxy->setChecked(_settings->useProxy());
	_ui.qleProxyHostname->setText(_settings->proxyHostname());
	_ui.qsbProxyPort->setValue(_settings->proxyPort());
	_ui.qleProxyUsername->setText(_settings->proxyUsername());
	_ui.qleProxyPassword->setText(_settings->proxyPassword());
	switch (_settings->proxyType()) {
		case QNetworkProxy::HttpProxy:
			_ui.qrbProxyTypeHttp->setChecked(true);
			break;
		case QNetworkProxy::Socks5Proxy:
			_ui.qrbProxyTypeSocks5->setChecked(true);
			break;
		case QNetworkProxy::HttpCachingProxy:
			_ui.qrbProxyTypeCachingOnlyHttp->setChecked(true);
	} // switch
} // fillOptions

void SettingsDialog::fillTranslation()
{
	for (int iI = 0; iI < _ui.qcbLanguage->count(); iI++) {
		if (_ui.qcbLanguage->itemData(iI) == _settings->translation()) {
			_ui.qcbLanguage->setCurrentIndex(iI);
			return;
		} // if
	} // for
} // fillTranslation

#ifndef EDITION_FREE
void SettingsDialog::on_newWordSound_stateChanged(int state) const
{
    _ui.soundSystem->setEnabled(state == Qt::Checked);
    _ui.soundCustomRadio->setEnabled(state == Qt::Checked);
    on_soundCustomRadio_clicked(state == Qt::Checked && _ui.soundCustomRadio->isChecked());
} // on_newWordSound_stateChanged

void SettingsDialog::on_systemTrayIcon_stateChanged(int state) const
{
	_ui.qcbMinimizeToTray->setEnabled(state == Qt::Checked);
	_ui.qcbShowWordsInTrayBalloon->setEnabled(state == Qt::Checked);
} // on_systemTrayIcon_stateChanged

# ifdef Q_OS_WIN
void SettingsDialog::on_hotkeyAnswerClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.qleHotkeyAnswer);
} // on_hotkeyAnswerClear_clicked

void SettingsDialog::on_hotkeyMinimizeClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.qleHotkeyMinimize);
} // on_hotkeyMinimizeClear_clicked

void SettingsDialog::on_hotkeyNextClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.qleHotkeyNext);
} // on_hotkeyNextClear_clicked

void SettingsDialog::on_hotkeyRestoreClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.qleHotkeyRestore);
} // on_hotkeyRestoreClear_clicked

void SettingsDialog::on_showLicense_clicked(bool checked /* false */)
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
    if (qtvTreeView == _ui.qtvPluginsTTS) {
        const Plugins::TTSPlugin stpPlugin = _plugins->ttsPlugins().at(iRow);
        tlclLicenses = stpPlugin.ttsInterface->licenseText();
    } // if

    // show license
    LicenseTextDialog ltdLicense(tlclLicenses, _settings, this);
    ltdLicense.exec();
} // on_showLicense_clicked
# endif

void SettingsDialog::on_soundBrowse_clicked(bool checked /* false */)
{
    QString qsOldFile = _settings->newWordSoundFile();
    QString qsFile = QFileDialog::getOpenFileName(this, tr("Sound file"), QFileInfo(qsOldFile).path()
#ifdef Q_OS_WIN
        , tr("sound files (*.wav)")
#endif
        );
    if (!qsFile.isEmpty()) {
        _ui.qleSoundCustom->setText(qsFile);
    } // if
} // on_soundBrowse_clicked

void SettingsDialog::on_soundCustomRadio_clicked(bool checked /* false */) const
{
    _ui.qleSoundCustom->setEnabled(checked);
    _ui.soundBrowse->setEnabled(checked);
} // on_soundCustomRadio_clicked

void SettingsDialog::on_soundSystem_clicked(bool checked /* false */) const
{
    on_soundCustomRadio_clicked(false);
} // on_soundSystem_clicked

void SettingsDialog::on_wordsFrequency_valueChanged(int i) const
{
	_ui.qsbWaitForAnswer->setMaximum(i - 1);
} // on_wordsFrequency_valueChanged

void SettingsDialog::prepareColorFlash()
{
	foreach (QString qsColor, QColor::colorNames()) {
		_ui.qcbColorFlash->addItem(QString(), qsColor);
	} // foreach
} // prepareColorFlash

void SettingsDialog::preparePlugins(QTreeView *pTreeView, PluginsModel *pModel) const
{
    pTreeView->setModel(pModel);
    for (int iRow = 0; iRow < pModel->rowCount(); iRow++) {
        QModelIndex qmiIndex = pModel->index(iRow, PluginsModel::ColumnLicense);

        QPushButton *qpbShow = new QPushButton(tr("Show"), pTreeView);
        pTreeView->setIndexWidget(qmiIndex, qpbShow);

        connect(qpbShow, SIGNAL(clicked(bool)), SLOT(on_showLicense_clicked(bool)));
    } // for
    pTreeView->header()->setSectionResizeMode(PluginsModel::ColumnName, QHeaderView::Stretch);
    pTreeView->header()->setSectionResizeMode(PluginsModel::ColumnLicense, QHeaderView::ResizeToContents);
} // preparePlugins
#endif

void SettingsDialog::prepareTranslations()
{
    // add default language
    _ui.qcbLanguage->addItem(tr("English"), QLocale(QLocale::English).name() + '.' + LANG_SUFFIX);

    // get installed languages
    QDir qdDir;
    qdDir.cd(DIR_LANG);
    QFileInfoList qfilFiles = qdDir.entryInfoList(QStringList() << "*." + LANG_SUFFIX, QDir::Files);

    foreach (QFileInfo qfiFile, qfilFiles) {
        QLocale qlLocale(qfiFile.completeBaseName());
        _ui.qcbLanguage->addItem(QLocale::languageToString(qlLocale.language()) + " (" + QLocale::countryToString(qlLocale.country()) + ')', qfiFile.fileName());
    } // foreach
} // prepareTranslations

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
void SettingsDialog::saveHotkey(const HotkeyLineEdit *pControl, Settings::Hotkey pHotkey) const
{
	Settings::HotkeyInfo shkiHotkey;

	shkiHotkey.text = pControl->text();
	shkiHotkey.virtualKey = pControl->property(PROPERTY_VIRTUALKEY).toUInt();

	_settings->setHotkey(pHotkey, shkiHotkey);
} // saveHotkey
#endif

void SettingsDialog::saveOptions()
{
    // general
#ifndef EDITION_FREE
    _settings->setHorizontalLayout(_ui.qcbHorizontalLayout->isChecked());
#endif
    _settings->setAlwaysOnTop(_ui.qcbAlwaysOnTop->isChecked());
#ifndef EDITION_FREE
    _settings->setRememberWindowPosition(_ui.qcbRememberWindowPosition->isChecked());
    _settings->setSystemTrayIcon(_ui.systemTrayIcon->isChecked());
	_settings->setShowWordsInTrayBalloon(_ui.qcbShowWordsInTrayBalloon->isChecked());
	_settings->setMinimizeToTray(_ui.qcbMinimizeToTray->isChecked());
#endif
	_settings->setTranslation(_ui.qcbLanguage->itemData(_ui.qcbLanguage->currentIndex()).toString());
	_settings->setUpdateCheck(_ui.qcbUpdateCheck->isChecked());

    // learning
    _settings->setWordsFrequency(_ui.wordsFrequency->value());
#ifndef EDITION_FREE
	_settings->setWaitForAnswer(_ui.qsbWaitForAnswer->value());
	_settings->setNewWordSound(_ui.newWordSound->isChecked());
    if (_ui.soundSystem->isChecked()) {
        _settings->setNewWordSoundType(Settings::NewWordSoundTypeSystem);
    } else {
        _settings->setNewWordSoundType(Settings::NewWordSoundTypeCustom);
    } // if else
    _settings->setNewWordSoundFile(_ui.qleSoundCustom->text());
	_settings->setNewWordFlash(_ui.qcbNewWordFlash->isChecked());
	_settings->setLearnDisabledWords(_ui.qcbLearnDisabledWords->isChecked());
#endif
	_settings->setSwitchLearningDirection(_ui.qcbSwitchLearningDirection->checkState());
#ifndef EDITION_FREE
	_settings->setStartLearningOnStartup(_ui.qcbStartLearningOnStartup->isChecked());
#endif

#ifndef EDITION_FREE
    // appearance
	_settings->setShowToolBar(_ui.qcbMainWindowToolBar->isChecked());
	_settings->setShowLanguageNames(_ui.qcbMainWindowLanguageNames->isChecked());
	_settings->setShowCategoryName(_ui.qcbMainWindowCategoryName->isChecked());
	_settings->setShowRecordControls(_ui.qcbMainWindowRecordControls->isChecked());
	_settings->setShowStatusBar(_ui.qcbMainWindowStatusBar->isChecked());
	_settings->setColorFlash(_ui.qcbColorFlash->itemData(_ui.qcbColorFlash->currentIndex()).toString());
    _settings->setCanEnableCategories(_ui.qcbVocabularyManagerCategoriesEnable->isChecked());
	_settings->setCanChangeCategoryPriority(_ui.qcbVocabularyManagerCategoryPriority->isChecked());

# ifdef Q_OS_WIN
	// hotkeys
    // learning
	saveHotkey(_ui.qleHotkeyNext, Settings::HotkeyNext);
	saveHotkey(_ui.qleHotkeyAnswer, Settings::HotkeyAnswer);
    // window
	saveHotkey(_ui.qleHotkeyMinimize, Settings::HotkeyMinimize);
	saveHotkey(_ui.qleHotkeyRestore, Settings::HotkeyRestore);
# endif
#endif

	// network
	_settings->setUseProxy(_ui.qgbUseProxy->isChecked());
	_settings->setProxyHostname(_ui.qleProxyHostname->text());
	_settings->setProxyPort(_ui.qsbProxyPort->value());
	_settings->setProxyUsername(_ui.qleProxyUsername->text());
	_settings->setProxyPassword(_ui.qleProxyPassword->text());
	if (_ui.qrbProxyTypeHttp->isChecked()) {
		_settings->setProxyType(QNetworkProxy::HttpProxy);
	} else {
		if (_ui.qrbProxyTypeSocks5->isChecked()) {
			_settings->setProxyType(QNetworkProxy::Socks5Proxy);
		} else {
			_settings->setProxyType(QNetworkProxy::HttpCachingProxy);
		} // if else
	} // if else
} // saveOptions