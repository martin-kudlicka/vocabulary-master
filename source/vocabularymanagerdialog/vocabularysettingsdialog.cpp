#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

VocabularySettingsDialog::VocabularySettingsDialog(Vocabulary *vocabulary,
#ifndef EDITION_FREE
    const Plugins *plugins,
#endif
    QWidget *parent /* nullptr */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags)
#ifndef EDITION_FREE
	, _fieldsModel(vocabulary), _languageFieldDelegate(vocabulary), _plugins(plugins)
#endif
	, _vocabulary(vocabulary)
{
	_ui.setupUi(this);
#ifdef EDITION_FREE
    delete _ui.speech;
    _ui.tabs->removeTab(TabFields);
	_ui.tabs->removeTab(TabTemplates);
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
    for (quint8 row = 0; row < _fieldsModel.rowCount(); row++)
	{
        actualizeFieldsEditor(row);
    }
}

void VocabularySettingsDialog::actualizeFieldsEditor(quint8 row) const
{
	const quint8 fieldId = _vocabulary->fieldId(row);

	QModelIndex index = _fieldsModel.index(row, FieldsModel::ColumnTemplateName);
	_ui.fields->openPersistentEditor(index);
	index = _fieldsModel.index(row, FieldsModel::ColumnName);
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
	{
		_ui.fields->closePersistentEditor(index);
	}
	else
	{
		_ui.fields->openPersistentEditor(index);
	}
	index = _fieldsModel.index(row, FieldsModel::ColumnLanguage);
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
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
    _ui.languageLeft->setText(_vocabulary->languageName(VocabularyDatabase::FieldLanguageLeft));
    _ui.languageRight->setText(_vocabulary->languageName(VocabularyDatabase::FieldLanguageRight));
#ifndef EDITION_FREE
    fillSpeech(_ui.speechLeft, QString::number(static_cast<quintptr>(_vocabulary->languageSpeech(VocabularyDatabase::FieldLanguageLeft))), _vocabulary->languageVoice(VocabularyDatabase::FieldLanguageLeft));
	fillSpeech(_ui.speechRight, QString::number(static_cast<quintptr>(_vocabulary->languageSpeech(VocabularyDatabase::FieldLanguageRight))), _vocabulary->languageVoice(VocabularyDatabase::FieldLanguageRight));

	// templates
	_ui.learningLeft->setPlainText(_vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguageLeft));
	_ui.learningRight->setPlainText(_vocabulary->languageLearningTemplate(VocabularyDatabase::FieldLanguageRight));
    _ui.trayLeft->setPlainText(_vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguageLeft));
    _ui.trayRight->setPlainText(_vocabulary->languageTrayTemplate(VocabularyDatabase::FieldLanguageRight));
#endif
}

#ifndef EDITION_FREE
void VocabularySettingsDialog::fillSpeech(QComboBox *comboBox, const QString &speech, const QString &voice)
{
  const TTSInterface::TTSPlugin speechPlugin = static_cast<TTSInterface::TTSPlugin>(_vocabulary->settings(speech).toUInt());
	const QString voiceId     = _vocabulary->settings(voice);

	for (quint8 voiceIndex = 0; voiceIndex < comboBox->count(); voiceIndex++)
	{
		const SpeechVoice speechVoice = _voices.at(comboBox->itemData(voiceIndex).toUInt());
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
    _ui.fields->setItemDelegateForColumn(FieldsModel::ColumnTemplateName, &_lineEditDelegate);
    _ui.fields->setItemDelegateForColumn(FieldsModel::ColumnName, &_lineEditDelegate);
    _ui.fields->setItemDelegateForColumn(FieldsModel::ColumnLanguage, &_languageFieldDelegate);
    connect(_ui.fields->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_fieldsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

    _ui.fieldType->addItem(tr("Text"));

    actualizeFieldsEditor();

    for (quint8 column = 0; column < _ui.fields->header()->count(); column++)
	{
        if (column == FieldsModel::ColumnSpeech || column == FieldsModel::ColumnShow)
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

    for (const Plugins::TTSPlugin &ttsPlugin : _plugins->ttsPlugins())
	{
        const TTSInterface *plugin = ttsPlugin.ttsInterface;

        const TTSInterface::VoiceInfoList voices = plugin->voicesInfo();
        for (const TTSInterface::VoiceInfo &voiceInfo : voices)
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
    for (quint8 row = 0; row < _fieldsModel.rowCount(); row++)
	{
        _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguageLeft, _ui.languageLeft->text());
        _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguageRight, _ui.languageRight->text());

        const QModelIndex index = _fieldsModel.index(row, FieldsModel::ColumnLanguage);
        _ui.fields->closePersistentEditor(index);
        _ui.fields->openPersistentEditor(index);
    }
}
#endif

void VocabularySettingsDialog::saveOptions()
{
    // languages
#ifdef EDITION_FREE
    _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguageLeft, _ui.languageLeft->text());
    _vocabulary->setLanguageName(VocabularyDatabase::FieldLanguageRight, _ui.languageRight->text());
#else
	SpeechVoice speechVoice = _voices.at(_ui.speechLeft->itemData(_ui.speechLeft->currentIndex()).toUInt());
	_vocabulary->setLanguageSpeech(VocabularyDatabase::FieldLanguageLeft, speechVoice.ttsPlugin);
	_vocabulary->setLanguageVoice(VocabularyDatabase::FieldLanguageLeft, speechVoice.voiceId);
	speechVoice = _voices.at(_ui.speechRight->itemData(_ui.speechRight->currentIndex()).toUInt());
	_vocabulary->setLanguageSpeech(VocabularyDatabase::FieldLanguageRight, speechVoice.ttsPlugin);
	_vocabulary->setLanguageVoice(VocabularyDatabase::FieldLanguageRight, speechVoice.voiceId);

	// templates
	_vocabulary->setLanguageLearningTemplate(VocabularyDatabase::FieldLanguageLeft, _ui.learningLeft->toPlainText());
	_vocabulary->setLanguageLearningTemplate(VocabularyDatabase::FieldLanguageRight, _ui.learningRight->toPlainText());
    _vocabulary->setLanguageTrayTemplate(VocabularyDatabase::FieldLanguageLeft, _ui.trayLeft->toPlainText());
    _vocabulary->setLanguageTrayTemplate(VocabularyDatabase::FieldLanguageRight, _ui.trayRight->toPlainText());
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
	const QModelIndex current = _ui.fields->currentIndex();
	_fieldsModel.swap(current.row(), current.row() + 1);
    actualizeFieldsEditor();

	_ui.fields->setCurrentIndex(_fieldsModel.index(current.row() + 1, current.column()));
}

void VocabularySettingsDialog::on_fieldRemove_clicked(bool checked /* false */)
{
	const QItemSelectionModel *selection = _ui.fields->selectionModel();
	_fieldsModel.removeRow(selection->currentIndex().row());
    actualizeFieldsEditor();
}

void VocabularySettingsDialog::on_fieldUp_clicked(bool checked /* false */)
{
	const QModelIndex current = _ui.fields->currentIndex();
	_fieldsModel.swap(current.row(), current.row() - 1);
    actualizeFieldsEditor();

	_ui.fields->setCurrentIndex(_fieldsModel.index(current.row() - 1, current.column()));
}

void VocabularySettingsDialog::on_fieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	const QItemSelectionModel *selection = _ui.fields->selectionModel();
    bool builtIn;
    if (selection->hasSelection())
	{
        const quint8 fieldId = _vocabulary->fieldId(_ui.fields->currentIndex().row());
        builtIn = _vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn);
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