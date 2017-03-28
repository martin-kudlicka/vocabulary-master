#include "settingsdialog.h"

#include "settings/colordelegate.h"
#include <QtCore/QDir>
#include "common/licensetextdialog.h"
#include <QtWidgets/QFileDialog>

SettingsDialog::SettingsDialog(const Plugins *plugins, Settings *settings, QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _plugins(plugins), _expPluginsModel(plugins, PluginsModel::PluginType::Exp), _impPluginsModel(plugins, PluginsModel::PluginType::Imp), _ttsPluginsModel(plugins, PluginsModel::PluginType::TTS), _settings(settings)
{
  _ui.setupUi(this);

#ifndef Q_OS_WIN
  _ui.tabs->removeTab(Tab::Hotkey);
#else
  _ui.colorFlashCombo->setItemDelegate(new ColorDelegate(_ui.colorFlashCombo));

  prepareColorFlash();
#endif
  prepareTranslations();
  fillOptions();
}

SettingsDialog::~SettingsDialog()
{
}

#ifdef Q_OS_WIN
void SettingsDialog::clearHotkey(HotkeyLineEdit *control) const
{
  control->clear();
  control->setProperty(PROPERTY_VIRTUALKEY, VIRTUALKEY_NONE);
}
#endif

void SettingsDialog::fillColorFlash()
{
  for (auto colorIndex = 0; colorIndex < _ui.colorFlashCombo->count(); colorIndex++)
  {
    if (_ui.colorFlashCombo->itemData(colorIndex, Qt::UserRole).toString() == _settings->colorFlash())
    {
      _ui.colorFlashCombo->setCurrentIndex(colorIndex);
      return;
    }
  }
}

#ifdef Q_OS_WIN
void SettingsDialog::fillHotkey(HotkeyLineEdit *control, Settings::Hotkey hotkey) const
{
  const auto hotkeyInfo = _settings->hotkey(hotkey);
  control->setText(hotkeyInfo.text);
  control->setProperty(PROPERTY_VIRTUALKEY, hotkeyInfo.virtualKey);
}
#endif

void SettingsDialog::fillOptions()
{
  // general
  _ui.alwaysOnTop->setChecked(_settings->alwaysOnTop());
  _ui.rememberWindowPosition->setChecked(_settings->rememberWindowPosition());
  _ui.systemTrayIcon->setChecked(_settings->systemTrayIcon());
  _ui.systemTrayIcon->setCheckState(_ui.systemTrayIcon->checkState());
  _ui.showWordsInTrayBalloon->setChecked(_settings->showWordsInTrayBalloon());
  _ui.minimizeToTray->setChecked(_settings->minimizeToTray());
  fillTranslation();
  _ui.updateCheck->setChecked(_settings->updateCheck());
  _ui.cacheVocabulary->setChecked(_settings->cacheVocabulary());
  _ui.cacheVocabulary->setCheckState(_ui.cacheVocabulary->checkState());
  _ui.recordsToCache->setValue(_settings->recordsToCache());

  // learning
  _ui.wordsFrequency->setValue(_settings->wordsFrequency());
  _ui.waitForAnswerValue->setMaximum(_settings->wordsFrequency() - 1);
  _ui.waitForAnswerValue->setValue(_settings->waitForAnswer());
  _ui.newWordSound->setChecked(_settings->newWordSound());
  if (_settings->newWordSoundType() == Settings::NewWordSoundType::System)
  {
    _ui.soundSystem->click();
  }
  else
  {
    _ui.soundCustomRadio->click();
  }
  _ui.soundCustomEdit->setText(_settings->newWordSoundFile());
  _ui.newWordFlash->setChecked(_settings->newWordFlash());
  _ui.learnDisabledWords->setChecked(_settings->learnDisabledWords());
  _ui.switchLearningDirection->setCheckState(_settings->switchLearningDirection());
  _ui.startLearningOnStartup->setChecked(_settings->startLearningOnStartup());

  // appearance
  _ui.horizontalLayout->setChecked(_settings->horizontalLayout());
  _ui.mainWindowToolBar->setChecked(_settings->showToolBar());
  _ui.mainWindowLanguageNames->setChecked(_settings->showLanguageNames());
  _ui.mainWindowCategoryName->setChecked(_settings->showCategoryName());
  _ui.mainWindowRecordControls->setChecked(_settings->showRecordControls());
  _ui.mainWindowStatusBar->setChecked(_settings->showStatusBar());
  fillColorFlash();
  _ui.vocabularyManagerCategoriesEnable->setChecked(_settings->canEnableCategories());
  _ui.vocabularyManagerCategoryPriority->setChecked(_settings->canChangeCategoryPriority());

#ifdef Q_OS_WIN
  // hotkeys
  // learning
  fillHotkey(_ui.hotkeyNext,   Settings::Hotkey::Next);
  fillHotkey(_ui.hotkeyAnswer, Settings::Hotkey::Answer);
  // window
  fillHotkey(_ui.hotkeyMinimize, Settings::Hotkey::Minimize);
  fillHotkey(_ui.hotkeyRestore,  Settings::Hotkey::Restore);
#endif

  // plugins
  preparePlugins(_ui.pluginsImp, &_impPluginsModel);
  preparePlugins(_ui.pluginsExp, &_expPluginsModel);
  preparePlugins(_ui.pluginsTTS, &_ttsPluginsModel);

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
  }
}

void SettingsDialog::fillTranslation()
{
  for (auto languageIndex = 0; languageIndex < _ui.language->count(); languageIndex++)
  {
    if (_ui.language->itemData(languageIndex) == _settings->translation())
    {
      _ui.language->setCurrentIndex(languageIndex);
      return;
    }
  }
}

void SettingsDialog::prepareColorFlash()
{
  for (const auto &qsColor : QColor::colorNames())
  {
    _ui.colorFlashCombo->addItem(QString(), qsColor);
  }
}

void SettingsDialog::preparePlugins(QTreeView *treeView, PluginsModel *model) const
{
  treeView->setModel(model);
  for (auto row = 0; row < model->rowCount(); row++)
  {
    const auto modelIndex = model->index(row, static_cast<int>(PluginsModel::Column::License));

    auto showButton = new QPushButton(tr("Show"), treeView);
    treeView->setIndexWidget(modelIndex, showButton);

    connect(showButton, &QPushButton::clicked, this, &SettingsDialog::on_showLicense_clicked);
  }
  treeView->header()->setSectionResizeMode(static_cast<int>(PluginsModel::Column::Name),    QHeaderView::Stretch);
  treeView->header()->setSectionResizeMode(static_cast<int>(PluginsModel::Column::License), QHeaderView::ResizeToContents);
}

void SettingsDialog::prepareTranslations()
{
  // add default language
  _ui.language->addItem(tr("English"), QLocale(QLocale::English).name() + '.' + LANG_SUFFIX);

  // get installed languages
  QDir dir;
  dir.cd(DIR_LANG);
  const auto files = dir.entryInfoList(QStringList() << "*." + LANG_SUFFIX, QDir::Files);

  for (const auto &file : files)
  {
    const QLocale locale(file.completeBaseName());
    _ui.language->addItem(QLocale::languageToString(locale.language()) + " (" + QLocale::countryToString(locale.country()) + ')', file.fileName());
  }
}

#ifdef Q_OS_WIN
void SettingsDialog::saveHotkey(const HotkeyLineEdit *control, Settings::Hotkey hotkey) const
{
  Settings::HotkeyInfo hotkeyInfo;

  hotkeyInfo.text       = control->text();
  hotkeyInfo.virtualKey = control->property(PROPERTY_VIRTUALKEY).toUInt();

  _settings->setHotkey(hotkey, hotkeyInfo);
}
#endif

void SettingsDialog::saveOptions()
{
  // general
  _settings->setAlwaysOnTop(_ui.alwaysOnTop->isChecked());
  _settings->setRememberWindowPosition(_ui.rememberWindowPosition->isChecked());
  _settings->setSystemTrayIcon(_ui.systemTrayIcon->isChecked());
  _settings->setShowWordsInTrayBalloon(_ui.showWordsInTrayBalloon->isChecked());
  _settings->setMinimizeToTray(_ui.minimizeToTray->isChecked());
  _settings->setTranslation(_ui.language->itemData(_ui.language->currentIndex()).toString());
  _settings->setUpdateCheck(_ui.updateCheck->isChecked());
  _settings->setCacheVocabulary(_ui.cacheVocabulary->isChecked());
  _settings->setRecordsToCache(_ui.recordsToCache->value());

  // learning
  _settings->setWordsFrequency(_ui.wordsFrequency->value());
  _settings->setWaitForAnswer(_ui.waitForAnswerValue->value());
  _settings->setNewWordSound(_ui.newWordSound->isChecked());
  if (_ui.soundSystem->isChecked())
  {
    _settings->setNewWordSoundType(Settings::NewWordSoundType::System);
  }
  else
  {
    _settings->setNewWordSoundType(Settings::NewWordSoundType::Custom);
  }
  _settings->setNewWordSoundFile(_ui.soundCustomEdit->text());
  _settings->setNewWordFlash(_ui.newWordFlash->isChecked());
  _settings->setLearnDisabledWords(_ui.learnDisabledWords->isChecked());
  _settings->setSwitchLearningDirection(_ui.switchLearningDirection->checkState());
  _settings->setStartLearningOnStartup(_ui.startLearningOnStartup->isChecked());

  // appearance
  _settings->setHorizontalLayout(_ui.horizontalLayout->isChecked());
  _settings->setShowToolBar(_ui.mainWindowToolBar->isChecked());
  _settings->setShowLanguageNames(_ui.mainWindowLanguageNames->isChecked());
  _settings->setShowCategoryName(_ui.mainWindowCategoryName->isChecked());
  _settings->setShowRecordControls(_ui.mainWindowRecordControls->isChecked());
  _settings->setShowStatusBar(_ui.mainWindowStatusBar->isChecked());
  _settings->setColorFlash(_ui.colorFlashCombo->itemData(_ui.colorFlashCombo->currentIndex()).toString());
  _settings->setCanEnableCategories(_ui.vocabularyManagerCategoriesEnable->isChecked());
  _settings->setCanChangeCategoryPriority(_ui.vocabularyManagerCategoryPriority->isChecked());

#ifdef Q_OS_WIN
  // hotkeys
    // learning
  saveHotkey(_ui.hotkeyNext,   Settings::Hotkey::Next);
  saveHotkey(_ui.hotkeyAnswer, Settings::Hotkey::Answer);
  // window
  saveHotkey(_ui.hotkeyMinimize, Settings::Hotkey::Minimize);
  saveHotkey(_ui.hotkeyRestore,  Settings::Hotkey::Restore);
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
    }
  }
}

void SettingsDialog::accept()
{
  saveOptions();

  QDialog::accept();
}

void SettingsDialog::on_cacheVocabulary_stateChanged(int state) const
{
  _ui.recordsToCache->setEnabled(state == Qt::Checked);
}

#ifdef Q_OS_WIN
void SettingsDialog::on_hotkeyAnswerClear_clicked(bool checked /* false */) const
{
  clearHotkey(_ui.hotkeyAnswer);
}

void SettingsDialog::on_hotkeyMinimizeClear_clicked(bool checked /* false */) const
{
  clearHotkey(_ui.hotkeyMinimize);
}

void SettingsDialog::on_hotkeyNextClear_clicked(bool checked /* false */) const
{
  clearHotkey(_ui.hotkeyNext);
}

void SettingsDialog::on_hotkeyRestoreClear_clicked(bool checked /* false */) const
{
  clearHotkey(_ui.hotkeyRestore);
}

void SettingsDialog::on_newWordSound_stateChanged(int state) const
{
  _ui.soundSystem->setEnabled(state == Qt::Checked);
  _ui.soundCustomRadio->setEnabled(state == Qt::Checked);
  on_soundCustomRadio_clicked(state == Qt::Checked && _ui.soundCustomRadio->isChecked());
}

void SettingsDialog::on_showLicense_clicked(bool checked /* false */)
{
  const auto button   = qobject_cast<const QPushButton *>(sender());
  const auto treeView = qobject_cast<const QTreeView *>(button->parent()->parent());

  // find plugin
  int row;
  const auto pluginsModel = qobject_cast<const PluginsModel *>(treeView->model());
  for (row = 0; row < pluginsModel->rowCount(); row++)
  {
    const auto modelIndex = pluginsModel->index(row, static_cast<int>(PluginsModel::Column::License));
    if (treeView->indexWidget(modelIndex) == button)
    {
      break;
    }
  }

  // license
  LicenseCommon::LicenseContentList licenses;
  if (treeView == _ui.pluginsTTS)
  {
    const auto ttsPlugin = _plugins->ttsPlugins().at(row);
    licenses             = ttsPlugin.ttsInterface->licenseText();
  }

  // show license
  LicenseTextDialog licenseTextDialog(licenses, _settings, this);
  licenseTextDialog.exec();
}
#endif

void SettingsDialog::on_soundBrowse_clicked(bool checked /* false */)
{
  const auto oldFile = _settings->newWordSoundFile();
  const auto newFile = QFileDialog::getOpenFileName(this, tr("Sound file"), QFileInfo(oldFile).path()
#ifdef Q_OS_WIN
                                                    , tr("sound files (*.wav)")
#endif
                                                    );
  if (!newFile.isEmpty())
  {
    _ui.soundCustomEdit->setText(newFile);
  }
}

void SettingsDialog::on_soundCustomRadio_clicked(bool checked /* false */) const
{
  _ui.soundCustomEdit->setEnabled(checked);
  _ui.soundBrowse->setEnabled(checked);
}

void SettingsDialog::on_soundSystem_clicked(bool checked /* false */) const
{
  on_soundCustomRadio_clicked(false);
}

void SettingsDialog::on_systemTrayIcon_stateChanged(int state) const
{
  _ui.minimizeToTray->setEnabled(state == Qt::Checked);
  _ui.showWordsInTrayBalloon->setEnabled(state == Qt::Checked);
}

void SettingsDialog::on_wordsFrequency_valueChanged(int i) const
{
  _ui.waitForAnswerValue->setMaximum(i - 1);
}