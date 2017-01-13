#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

#include "vocabulary.h"
#include "plugins.h"

VocabularySettingsDialog::VocabularySettingsDialog(Vocabulary *vocabulary,
#ifndef EDITION_FREE
                                                   const Plugins *plugins,
#endif
                                                   QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags)
#ifndef EDITION_FREE
                                                   , _fieldsModel(vocabulary), _languageFieldDelegate(vocabulary), _plugins(plugins)
#endif
                                                   , _vocabulary(vocabulary)
{
  _ui.setupUi(this);
#ifdef EDITION_FREE
  delete _ui.speech;
  _ui.tabs->removeTab(Tab::Fields);
  _ui.tabs->removeTab(Tab::Templates);
#else

  preparePlugins();
  prepareFields();
#endif
  fillOptions();
}

VocabularySettingsDialog::~VocabularySettingsDialog()
{
}

void VocabularySettingsDialog::accept()
{
  saveOptions();

  QDialog::accept();
}

#ifndef EDITION_FREE
void VocabularySettingsDialog::actualizeFieldsEditor() const
{
  for (auto row = 0; row < _fieldsModel.rowCount(); row++)
  {
    actualizeFieldsEditor(row);
  }
}

void VocabularySettingsDialog::actualizeFieldsEditor(quintptr row) const
{
  const auto fieldId = _vocabulary->fieldId(row);

  auto index = _fieldsModel.index(row, static_cast<int>(FieldsModel::Column::TemplateName));
  _ui.fields->openPersistentEditor(index);
  index = _fieldsModel.index(row, static_cast<int>(FieldsModel::Column::Name));
  if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
  {
    _ui.fields->closePersistentEditor(index);
  }
  else
  {
    _ui.fields->openPersistentEditor(index);
  }
  index = _fieldsModel.index(row, static_cast<int>(FieldsModel::Column::Language));
  if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
  {
    _ui.fields->closePersistentEditor(index);
  }
  else
  {
    _ui.fields->openPersistentEditor(index);
  }
}
#endif

void VocabularySettingsDialog::fillOptions()
{
  // languages
  _ui.languageLeft->setText(_vocabulary->languageName(VocabularyDatabase::FieldLanguage::Left));
  _ui.languageRight->setText(_vocabulary->languageName(VocabularyDatabase::FieldLanguage::Right));
#ifndef EDITION_FREE
  fillSpeech(_ui.speechLeft, QString::number(static_cast<quintptr>(_vocabulary->languageSpeech(VocabularyDatabase::FieldLanguage::Left))), _vocabulary->languageVoice(VocabularyDatabase::FieldLanguage::Left));
  fillSpeech(_ui.speechRight, QString::number(static_cast<quintptr>(_vocabulary->languageSpeech(VocabularyDatabase::FieldLanguage::Right))), _vocabulary->languageVoice(VocabularyDatabase::FieldLanguage::Right));

  // templates
  _ui.learningLeft->setPlainText(_vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguage::Left));
  _ui.learningRight->setPlainText(_vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguage::Right));
  _ui.trayLeft->setPlainText(_vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguage::Left));
  _ui.trayRight->setPlainText(_vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguage::Right));
#endif
}

#ifndef EDITION_FREE
void VocabularySettingsDialog::fillSpeech(QComboBox *comboBox, const QString &speech, const QString &voice)
{
  const auto speechPlugin = static_cast<TTSInterface::TTSPlugin>(_vocabulary->settings(speech).toUInt());
  const auto voiceId      = _vocabulary->settings(voice);

  for (auto voiceIndex = 0; voiceIndex < comboBox->count(); voiceIndex++)
  {
    const auto speechVoice = _voices.at(comboBox->itemData(voiceIndex).toUInt());
    if (speechVoice.ttsPlugin == speechPlugin && speechVoice.voiceId == voiceId)
    {
      comboBox->setCurrentIndex(voiceIndex);
      return;
    }
  }

  // add unknown speech module when selected not found
  SpeechVoice speechVoice;
  speechVoice.ttsPlugin = static_cast<TTSInterface::TTSPlugin>(speechPlugin);
  speechVoice.voiceId   = voiceId;
  _voices.append(speechVoice);
  comboBox->addItem(tr("Unknown"));
  comboBox->setItemData(comboBox->count() - 1, _voices.size() - 1);
}

void VocabularySettingsDialog::prepareFields()
{
  _ui.fields->setModel(&_fieldsModel);
  _ui.fields->setItemDelegateForColumn(static_cast<int>(FieldsModel::Column::TemplateName), &_lineEditDelegate);
  _ui.fields->setItemDelegateForColumn(static_cast<int>(FieldsModel::Column::Name),         &_lineEditDelegate);
  _ui.fields->setItemDelegateForColumn(static_cast<int>(FieldsModel::Column::Language),     &_languageFieldDelegate);
  connect(_ui.fields->selectionModel(), &QItemSelectionModel::selectionChanged, this, &VocabularySettingsDialog::on_fieldsSelectionModel_selectionChanged);

  _ui.fieldType->addItem(tr("Text"));

  actualizeFieldsEditor();

  for (auto column = 0; column < _ui.fields->header()->count(); column++)
  {
    if (column == static_cast<int>(FieldsModel::Column::Speech) || column == static_cast<int>(FieldsModel::Column::Show))
    {
      _ui.fields->header()->setSectionResizeMode(column, QHeaderView::ResizeToContents);
    }
    else
    {
      _ui.fields->header()->setSectionResizeMode(column, QHeaderView::Stretch);
    }
  }
}

void VocabularySettingsDialog::preparePlugins()
{
  prepareSpeechPlugins(_ui.speechLeft);
  prepareSpeechPlugins(_ui.speechRight);
}

void VocabularySettingsDialog::prepareSpeechPlugins(QComboBox *comboBox)
{
  SpeechVoice speechVoice;

  speechVoice.ttsPlugin = TTSInterface::TTSPlugin::None;
  _voices.append(speechVoice);
  comboBox->addItem(tr("None"));
  comboBox->setItemData(comboBox->count() - 1, _voices.size() - 1);

  for (const auto &ttsPlugin : _plugins->ttsPlugins())
  {
    const auto plugin = ttsPlugin.ttsInterface;

    const auto voices = plugin->voicesInfo();
    for (const auto &voiceInfo : voices)
    {
      speechVoice.ttsPlugin = plugin->pluginId();
      speechVoice.voiceId   = voiceInfo.id;
      _voices.append(speechVoice);
      comboBox->addItem(QString("%1 (%2)").arg(plugin->pluginName()).arg(voiceInfo.description));
      comboBox->setItemData(comboBox->count() - 1, _voices.size() - 1);
    }
  }
}

void VocabularySettingsDialog::refreshLanguageNameFields() const
{
  for (auto row = 0; row < _fieldsModel.rowCount(); row++)
  {
    _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguage::Left, _ui.languageLeft->text());
    _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguage::Right, _ui.languageRight->text());

    const auto index = _fieldsModel.index(row, static_cast<int>(FieldsModel::Column::Language));
    _ui.fields->closePersistentEditor(index);
    _ui.fields->openPersistentEditor(index);
  }
}
#endif

void VocabularySettingsDialog::saveOptions()
{
  // languages
#ifdef EDITION_FREE
  _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguage::Left, _ui.languageLeft->text());
  _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguage::Right, _ui.languageRight->text());
#else
  SpeechVoice speechVoice = _voices.at(_ui.speechLeft->itemData(_ui.speechLeft->currentIndex()).toUInt());
  _vocabulary->setLanguageSpeech(VocabularyDatabase::FieldLanguage::Left, speechVoice.ttsPlugin);
  _vocabulary->setLanguageVoice(VocabularyDatabase::FieldLanguage::Left, speechVoice.voiceId);
  speechVoice = _voices.at(_ui.speechRight->itemData(_ui.speechRight->currentIndex()).toUInt());
  _vocabulary->setLanguageSpeech(VocabularyDatabase::FieldLanguage::Right, speechVoice.ttsPlugin);
  _vocabulary->setLanguageVoice(VocabularyDatabase::FieldLanguage::Right, speechVoice.voiceId);

  // templates
  _vocabulary->setLanguageLearningTemplate(VocabularyDatabase::FieldLanguage::Left, _ui.learningLeft->toPlainText());
  _vocabulary->setLanguageLearningTemplate(VocabularyDatabase::FieldLanguage::Right, _ui.learningRight->toPlainText());
  _vocabulary->setLanguageTrayTemplate(VocabularyDatabase::FieldLanguage::Left, _ui.trayLeft->toPlainText());
  _vocabulary->setLanguageTrayTemplate(VocabularyDatabase::FieldLanguage::Right, _ui.trayRight->toPlainText());
#endif
}

#ifndef EDITION_FREE
void VocabularySettingsDialog::on_fieldAdd_clicked(bool checked /* false */)
{
  _fieldsModel.addRow();
  actualizeFieldsEditor(_fieldsModel.rowCount() - 1);
}

void VocabularySettingsDialog::on_fieldDown_clicked(bool checked /* false */)
{
  const auto current = _ui.fields->currentIndex();
  _fieldsModel.swap(current.row(), current.row() + 1);
  actualizeFieldsEditor();

  _ui.fields->setCurrentIndex(_fieldsModel.index(current.row() + 1, current.column()));
}

void VocabularySettingsDialog::on_fieldRemove_clicked(bool checked /* false */)
{
  const auto selection = _ui.fields->selectionModel();
  _fieldsModel.removeRow(selection->currentIndex().row());
  actualizeFieldsEditor();
}

void VocabularySettingsDialog::on_fieldUp_clicked(bool checked /* false */)
{
  const auto current = _ui.fields->currentIndex();
  _fieldsModel.swap(current.row(), current.row() - 1);
  actualizeFieldsEditor();

  _ui.fields->setCurrentIndex(_fieldsModel.index(current.row() - 1, current.column()));
}

void VocabularySettingsDialog::on_fieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  const auto selection = _ui.fields->selectionModel();
  bool builtIn;
  if (selection->hasSelection())
  {
    const auto fieldId = _vocabulary->fieldId(_ui.fields->currentIndex().row());
    builtIn            = _vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn);
  }

  _ui.fieldUp->setEnabled(selection->hasSelection() && _ui.fields->currentIndex().row() > 0);
  _ui.fieldDown->setEnabled(selection->hasSelection() && _ui.fields->currentIndex().row() < _ui.fields->model()->rowCount() - 1);
  _ui.fieldRemove->setEnabled(selection->hasSelection() && !builtIn);
}

void VocabularySettingsDialog::on_languageLeft_textEdited(const QString &text) const
{
  refreshLanguageNameFields();
}

void VocabularySettingsDialog::on_languageRight_textEdited(const QString &text) const
{
  refreshLanguageNameFields();
}
#endif