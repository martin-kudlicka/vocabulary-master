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
	, _plugins(plugins), _expPluginsModel(plugins, PluginsModel::PluginTypeExp), _impPluginsModel(plugins, PluginsModel::PluginTypeImp), _ttsPluginsModel(plugins, PluginsModel::PluginTypeTTS)
#endif
	, _settings(settings)
{
	_ui.setupUi(this);
#ifdef EDITION_FREE
    // general
    delete _ui.horizontalLayout;
    delete _ui.rememberWindowPosition;
    delete _ui.systemTrayIcon;
    delete _ui.showWordsInTrayBalloon;
    delete _ui.minimizeToTray;

    // learning
    delete _ui.waitForAnswerLabel;
    delete _ui.waitForAnswerValue;
	delete _ui.newWordSound;
	delete _ui.soundSystem;
    delete _ui.soundCustomRadio;
    delete _ui.soundCustomEdit;
	delete _ui.soundBrowse;
    delete _ui.newWordFlash;
	delete _ui.learnDisabledWords;
    delete _ui.startLearningOnStartup;

	// plugins
	_ui.tabs->removeTab(TabPlugins);

    // appearance
    _ui.tabs->removeTab(TabHotkey);
    _ui.tabs->removeTab(TabAppearance);
#elif !defined(Q_OS_WIN)
    _ui.tabs->removeTab(TabHotkey);
#else
	_ui.colorFlashCombo->setItemDelegate(new ColorDelegate(_ui.colorFlashCombo));

	prepareColorFlash();
#endif
    prepareTranslations();
    fillOptions();
} // SettingsDialog

SettingsDialog::~SettingsDialog()
{
} // ~SettingsDialog

void SettingsDialog::accept()
{
    saveOptions();

    QDialog::accept();
} // accept

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
void SettingsDialog::clearHotkey(HotkeyLineEdit *control) const
{
	control->clear();
	control->setProperty(PROPERTY_VIRTUALKEY, VIRTUALKEY_NONE);
} // clearHotkey
# endif

void SettingsDialog::fillColorFlash()
{
    for (int colorIndex = 0; colorIndex < _ui.colorFlashCombo->count(); colorIndex++)
	{
        if (_ui.colorFlashCombo->itemData(colorIndex, Qt::UserRole).toString() == _settings->colorFlash())
		{
            _ui.colorFlashCombo->setCurrentIndex(colorIndex);
            return;
        } // if
    } // for
} // fillColorFlash

# ifdef Q_OS_WIN
void SettingsDialog::fillHotkey(HotkeyLineEdit *control, Settings::Hotkey hotkey) const
{
	const Settings::HotkeyInfo hotkeyInfo = _settings->hotkey(hotkey);
	control->setText(hotkeyInfo.text);
	control->setProperty(PROPERTY_VIRTUALKEY, hotkeyInfo.virtualKey);
} // FillHotkey
# endif
#endif

void SettingsDialog::fillOptions()
{
    // general
#ifndef EDITION_FREE
    _ui.horizontalLayout->setChecked(_settings->horizontalLayout());
#endif
    _ui.alwaysOnTop->setChecked(_settings->alwaysOnTop());
#ifndef EDITION_FREE
    _ui.rememberWindowPosition->setChecked(_settings->rememberWindowPosition());
    _ui.systemTrayIcon->setChecked(_settings->systemTrayIcon());
	on_systemTrayIcon_stateChanged(_ui.systemTrayIcon->checkState());
	_ui.showWordsInTrayBalloon->setChecked(_settings->showWordsInTrayBalloon());
	_ui.minimizeToTray->setChecked(_settings->minimizeToTray());
#endif
	fillTranslation();
	_ui.updateCheck->setChecked(_settings->updateCheck());

    // learning
    _ui.wordsFrequency->setValue(_settings->wordsFrequency());
#ifndef EDITION_FREE
	_ui.waitForAnswerValue->setMaximum(_settings->wordsFrequency() - 1);
	_ui.waitForAnswerValue->setValue(_settings->waitForAnswer());
	_ui.newWordSound->setChecked(_settings->newWordSound());
    if (_settings->newWordSoundType() == Settings::NewWordSoundTypeSystem)
	{
        _ui.soundSystem->click();
    }
	else
	{
        _ui.soundCustomRadio->click();
    } // if else
    _ui.soundCustomEdit->setText(_settings->newWordSoundFile());
	_ui.newWordFlash->setChecked(_settings->newWordFlash());
	_ui.learnDisabledWords->setChecked(_settings->learnDisabledWords());
#endif
	_ui.switchLearningDirection->setCheckState(_settings->switchLearningDirection());
#ifndef EDITION_FREE
	_ui.startLearningOnStartup->setChecked(_settings->startLearningOnStartup());
#endif

#ifndef EDITION_FREE
    // appearance
	_ui.mainWindowToolBar->setChecked(_settings->showToolBar());
	_ui.mainWindowLanguageNames->setChecked(_settings->showLanguageNames());
	_ui.mainWindowCategoryName->setChecked(_settings->showCategoryName());
	_ui.mainWindowRecordControls->setChecked(_settings->showRecordControls());
	_ui.mainWindowStatusBar->setChecked(_settings->showStatusBar());
	fillColorFlash();
    _ui.vocabularyManagerCategoriesEnable->setChecked(_settings->canEnableCategories());
	_ui.vocabularyManagerCategoryPriority->setChecked(_settings->canChangeCategoryPriority());

# ifdef Q_OS_WIN
    // hotkeys
    // learning
	fillHotkey(_ui.hotkeyNext, Settings::HotkeyNext);
    fillHotkey(_ui.hotkeyAnswer, Settings::HotkeyAnswer);
    // window
	fillHotkey(_ui.hotkeyMinimize, Settings::HotkeyMinimize);
	fillHotkey(_ui.hotkeyRestore, Settings::HotkeyRestore);
# endif

    // plugins
    preparePlugins(_ui.pluginsImp, &_impPluginsModel);
    preparePlugins(_ui.pluginsExp, &_expPluginsModel);
    preparePlugins(_ui.pluginsTTS, &_ttsPluginsModel);
#endif

	// network
	_ui.useProxy->setChecked(_settings->useProxy());
	_ui.proxyHostname->setText(_settings->proxyHostname());
	_ui.proxyPort->setValue(_settings->proxyPort());
	_ui.proxyUsername->setText(_settings->proxyUsername());
	_ui.proxyPassword->setText(_settings->proxyPassword());
	switch (_settings->proxyType())
	{
		case QNetworkProxy::HttpProxy:
			_ui.proxyTypeHttp->setChecked(true);
			break;
		case QNetworkProxy::Socks5Proxy:
			_ui.proxyTypeSocks5->setChecked(true);
			break;
		case QNetworkProxy::HttpCachingProxy:
			_ui.proxyTypeCachingOnlyHttp->setChecked(true);
	} // switch
} // fillOptions

void SettingsDialog::fillTranslation()
{
	for (int languageIndex = 0; languageIndex < _ui.language->count(); languageIndex++)
	{
		if (_ui.language->itemData(languageIndex) == _settings->translation())
		{
			_ui.language->setCurrentIndex(languageIndex);
			return;
		} // if
	} // for
} // fillTranslation

#ifndef EDITION_FREE
void SettingsDialog::prepareColorFlash()
{
	foreach (const QString &qsColor, QColor::colorNames())
	{
		_ui.colorFlashCombo->addItem(QString(), qsColor);
	} // foreach
} // prepareColorFlash

void SettingsDialog::preparePlugins(QTreeView *treeView, PluginsModel *model) const
{
    treeView->setModel(model);
    for (int row = 0; row < model->rowCount(); row++)
	{
        const QModelIndex modelIndex = model->index(row, PluginsModel::ColumnLicense);

        QPushButton *showButton = new QPushButton(tr("Show"), treeView);
        treeView->setIndexWidget(modelIndex, showButton);

        connect(showButton, SIGNAL(clicked(bool)), SLOT(on_showLicense_clicked(bool)));
    } // for
    treeView->header()->setSectionResizeMode(PluginsModel::ColumnName,    QHeaderView::Stretch);
    treeView->header()->setSectionResizeMode(PluginsModel::ColumnLicense, QHeaderView::ResizeToContents);
} // preparePlugins
#endif

void SettingsDialog::prepareTranslations()
{
    // add default language
    _ui.language->addItem(tr("English"), QLocale(QLocale::English).name() + '.' + LANG_SUFFIX);

    // get installed languages
    QDir dir;
    dir.cd(DIR_LANG);
    const QFileInfoList files = dir.entryInfoList(QStringList() << "*." + LANG_SUFFIX, QDir::Files);

    foreach (const QFileInfo &file, files)
	{
        const QLocale locale(file.completeBaseName());
        _ui.language->addItem(QLocale::languageToString(locale.language()) + " (" + QLocale::countryToString(locale.country()) + ')', file.fileName());
    } // foreach
} // prepareTranslations

#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
void SettingsDialog::saveHotkey(const HotkeyLineEdit *control, Settings::Hotkey hotkey) const
{
	Settings::HotkeyInfo hotkeyInfo;

	hotkeyInfo.text = control->text();
	hotkeyInfo.virtualKey = control->property(PROPERTY_VIRTUALKEY).toUInt();

	_settings->setHotkey(hotkey, hotkeyInfo);
} // saveHotkey
#endif

void SettingsDialog::saveOptions()
{
    // general
#ifndef EDITION_FREE
    _settings->setHorizontalLayout(_ui.horizontalLayout->isChecked());
#endif
    _settings->setAlwaysOnTop(_ui.alwaysOnTop->isChecked());
#ifndef EDITION_FREE
    _settings->setRememberWindowPosition(_ui.rememberWindowPosition->isChecked());
    _settings->setSystemTrayIcon(_ui.systemTrayIcon->isChecked());
	_settings->setShowWordsInTrayBalloon(_ui.showWordsInTrayBalloon->isChecked());
	_settings->setMinimizeToTray(_ui.minimizeToTray->isChecked());
#endif
	_settings->setTranslation(_ui.language->itemData(_ui.language->currentIndex()).toString());
	_settings->setUpdateCheck(_ui.updateCheck->isChecked());

    // learning
    _settings->setWordsFrequency(_ui.wordsFrequency->value());
#ifndef EDITION_FREE
	_settings->setWaitForAnswer(_ui.waitForAnswerValue->value());
	_settings->setNewWordSound(_ui.newWordSound->isChecked());
    if (_ui.soundSystem->isChecked())
	{
        _settings->setNewWordSoundType(Settings::NewWordSoundTypeSystem);
    }
	else 
	{
        _settings->setNewWordSoundType(Settings::NewWordSoundTypeCustom);
    } // if else
    _settings->setNewWordSoundFile(_ui.soundCustomEdit->text());
	_settings->setNewWordFlash(_ui.newWordFlash->isChecked());
	_settings->setLearnDisabledWords(_ui.learnDisabledWords->isChecked());
#endif
	_settings->setSwitchLearningDirection(_ui.switchLearningDirection->checkState());
#ifndef EDITION_FREE
	_settings->setStartLearningOnStartup(_ui.startLearningOnStartup->isChecked());
#endif

#ifndef EDITION_FREE
    // appearance
	_settings->setShowToolBar(_ui.mainWindowToolBar->isChecked());
	_settings->setShowLanguageNames(_ui.mainWindowLanguageNames->isChecked());
	_settings->setShowCategoryName(_ui.mainWindowCategoryName->isChecked());
	_settings->setShowRecordControls(_ui.mainWindowRecordControls->isChecked());
	_settings->setShowStatusBar(_ui.mainWindowStatusBar->isChecked());
	_settings->setColorFlash(_ui.colorFlashCombo->itemData(_ui.colorFlashCombo->currentIndex()).toString());
    _settings->setCanEnableCategories(_ui.vocabularyManagerCategoriesEnable->isChecked());
	_settings->setCanChangeCategoryPriority(_ui.vocabularyManagerCategoryPriority->isChecked());

# ifdef Q_OS_WIN
	// hotkeys
    // learning
	saveHotkey(_ui.hotkeyNext, Settings::HotkeyNext);
	saveHotkey(_ui.hotkeyAnswer, Settings::HotkeyAnswer);
    // window
	saveHotkey(_ui.hotkeyMinimize, Settings::HotkeyMinimize);
	saveHotkey(_ui.hotkeyRestore, Settings::HotkeyRestore);
# endif
#endif

	// network
	_settings->setUseProxy(_ui.useProxy->isChecked());
	_settings->setProxyHostname(_ui.proxyHostname->text());
	_settings->setProxyPort(_ui.proxyPort->value());
	_settings->setProxyUsername(_ui.proxyUsername->text());
	_settings->setProxyPassword(_ui.proxyPassword->text());
	if (_ui.proxyTypeHttp->isChecked())
	{
		_settings->setProxyType(QNetworkProxy::HttpProxy);
	}
	else
	{
		if (_ui.proxyTypeSocks5->isChecked())
		{
			_settings->setProxyType(QNetworkProxy::Socks5Proxy);
		}
		else
		{
			_settings->setProxyType(QNetworkProxy::HttpCachingProxy);
		} // if else
	} // if else
} // saveOptions

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
void SettingsDialog::on_hotkeyAnswerClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.hotkeyAnswer);
} // on_hotkeyAnswerClear_clicked

void SettingsDialog::on_hotkeyMinimizeClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.hotkeyMinimize);
} // on_hotkeyMinimizeClear_clicked

void SettingsDialog::on_hotkeyNextClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.hotkeyNext);
} // on_hotkeyNextClear_clicked

void SettingsDialog::on_hotkeyRestoreClear_clicked(bool checked /* false */) const
{
	clearHotkey(_ui.hotkeyRestore);
} // on_hotkeyRestoreClear_clicked

void SettingsDialog::on_newWordSound_stateChanged(int state) const
{
    _ui.soundSystem->setEnabled(state == Qt::Checked);
    _ui.soundCustomRadio->setEnabled(state == Qt::Checked);
    on_soundCustomRadio_clicked(state == Qt::Checked && _ui.soundCustomRadio->isChecked());
} // on_newWordSound_stateChanged

void SettingsDialog::on_showLicense_clicked(bool checked /* false */)
{
    const QPushButton *button = qobject_cast<const QPushButton *>(sender());
    const QTreeView *treeView = qobject_cast<const QTreeView *>(button->parent()->parent());

    // find plugin
    int row;
    const PluginsModel *pluginsModel = qobject_cast<const PluginsModel *>(treeView->model());
    for (row = 0; row < pluginsModel->rowCount(); row++)
	{
        const QModelIndex modelIndex = pluginsModel->index(row, PluginsModel::ColumnLicense);
        if (treeView->indexWidget(modelIndex) == button)
		{
            break;
        } // if
    } // for

    // license
    LicenseCommon::LicenseContentList licenses;
    if (treeView == _ui.pluginsTTS)
	{
        const Plugins::TTSPlugin ttsPlugin = _plugins->ttsPlugins().at(row);
        licenses = ttsPlugin.ttsInterface->licenseText();
    } // if

    // show license
    LicenseTextDialog licenseTextDialog(licenses, _settings, this);
    licenseTextDialog.exec();
} // on_showLicense_clicked
# endif

void SettingsDialog::on_soundBrowse_clicked(bool checked /* false */)
{
    const QString oldFile = _settings->newWordSoundFile();
    const QString newFile = QFileDialog::getOpenFileName(this, tr("Sound file"), QFileInfo(oldFile).path()
# ifdef Q_OS_WIN
        , tr("sound files (*.wav)")
# endif
        );
    if (!newFile.isEmpty()) {
        _ui.soundCustomEdit->setText(newFile);
    } // if
} // on_soundBrowse_clicked

void SettingsDialog::on_soundCustomRadio_clicked(bool checked /* false */) const
{
    _ui.soundCustomEdit->setEnabled(checked);
    _ui.soundBrowse->setEnabled(checked);
} // on_soundCustomRadio_clicked

void SettingsDialog::on_soundSystem_clicked(bool checked /* false */) const
{
    on_soundCustomRadio_clicked(false);
} // on_soundSystem_clicked

void SettingsDialog::on_systemTrayIcon_stateChanged(int state) const
{
	_ui.minimizeToTray->setEnabled(state == Qt::Checked);
	_ui.showWordsInTrayBalloon->setEnabled(state == Qt::Checked);
} // on_systemTrayIcon_stateChanged

void SettingsDialog::on_wordsFrequency_valueChanged(int i) const
{
	_ui.waitForAnswerValue->setMaximum(i - 1);
} // on_wordsFrequency_valueChanged
#endif