#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

void VocabularySettingsDialog::accept()
{
	SaveOptions();

	QDialog::accept();
} // accept

const void VocabularySettingsDialog::FillOptions()
{
    // languages
    _qdvsdVocabularySettingsDialog.leLanguageLeft->setText(_vVocabulary->GetSettings(KEY_LANGUAGE1));
    _qdvsdVocabularySettingsDialog.leLanguageRight->setText(_vVocabulary->GetSettings(KEY_LANGUAGE2));
#ifndef FREE
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechLeft, KEY_SPEECH1, KEY_VOICE1);
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechRight, KEY_SPEECH2, KEY_VOICE2);

	// templates
	_qdvsdVocabularySettingsDialog.qteLearningLeft->setPlainText(_vVocabulary->GetSettings(KEY_LEARNINGTEMPLATE1));
	_qdvsdVocabularySettingsDialog.qteLearningRight->setPlainText(_vVocabulary->GetSettings(KEY_LEARNINGTEMPLATE2));
#endif
} // FillOptions

#ifndef FREE
const void VocabularySettingsDialog::FillSpeech(QComboBox *pComboBox, const QString &pSpeech, const QString &pVoice)
{
	int iSpeech = _vVocabulary->GetSettings(pSpeech).toInt();
	QString qsVoice = _vVocabulary->GetSettings(pVoice);

	for (int iI = 0; iI < pComboBox->count(); iI++) {
		sSpeechVoice spvVoice = _tvVoiceList.at(pComboBox->itemData(iI).toInt());
		if (spvVoice.etpPlugin == iSpeech && spvVoice.qsVoiceId == qsVoice) {
			pComboBox->setCurrentIndex(iI);
			return;
		} // if
	} // for

    // add unknown speech module when selected not found
    sSpeechVoice spvVoice;
    spvVoice.etpPlugin = static_cast<TTSInterface::eTTSPlugin>(iSpeech);
    spvVoice.qsVoiceId = qsVoice;
    _tvVoiceList.append(spvVoice);
    pComboBox->addItem(tr("Unknown"));
    pComboBox->setItemData(pComboBox->count() - 1, _tvVoiceList.size() - 1);
} // FillSpeech

const void VocabularySettingsDialog::on_qpbFieldAdd_clicked(bool checked /* false */)
{
    _fmFieldsModel.AddRow();
} // on_qpbFieldAdd_clicked

const void VocabularySettingsDialog::on_qpbFieldDown_clicked(bool checked /* false */)
{
	QModelIndex qmiCurrent = _qdvsdVocabularySettingsDialog.qtvFields->currentIndex();
	_fmFieldsModel.Swap(qmiCurrent.row(), qmiCurrent.row() + 1);

	_qdvsdVocabularySettingsDialog.qtvFields->setCurrentIndex(_fmFieldsModel.index(qmiCurrent.row() + 1, qmiCurrent.column()));
} // on_qpbFieldDown_clicked

const void VocabularySettingsDialog::on_qpbFieldRemove_clicked(bool checked /* false */)
{
	const QItemSelectionModel *qismSelection = _qdvsdVocabularySettingsDialog.qtvFields->selectionModel();
	_fmFieldsModel.RemoveRow(qismSelection->currentIndex().row());
} // on_qpbFieldRemove_clicked

const void VocabularySettingsDialog::on_qpbFieldUp_clicked(bool checked /* false */)
{
	QModelIndex qmiCurrent = _qdvsdVocabularySettingsDialog.qtvFields->currentIndex();
	_fmFieldsModel.Swap(qmiCurrent.row(), qmiCurrent.row() - 1);

	_qdvsdVocabularySettingsDialog.qtvFields->setCurrentIndex(_fmFieldsModel.index(qmiCurrent.row() - 1, qmiCurrent.column()));
} // on_qpbFieldUp_clicked

const void VocabularySettingsDialog::on_qtvFieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	const QItemSelectionModel *qismSelection = _qdvsdVocabularySettingsDialog.qtvFields->selectionModel();

	_qdvsdVocabularySettingsDialog.qpbFieldUp->setEnabled(qismSelection->hasSelection() && _qdvsdVocabularySettingsDialog.qtvFields->currentIndex().row() > 0);
	_qdvsdVocabularySettingsDialog.qpbFieldDown->setEnabled(qismSelection->hasSelection() && _qdvsdVocabularySettingsDialog.qtvFields->currentIndex().row() < _qdvsdVocabularySettingsDialog.qtvFields->model()->rowCount() - 1);
	_qdvsdVocabularySettingsDialog.qpbFieldRemove->setEnabled(qismSelection->hasSelection());
} // on_qtvFieldsSelectionModel_selectionChanged

const void VocabularySettingsDialog::PrepareFields()
{
    _qdvsdVocabularySettingsDialog.qtvFields->setModel(&_fmFieldsModel);
    _qdvsdVocabularySettingsDialog.qtvFields->setItemDelegateForColumn(FieldsModel::ColumnLanguage, &_lfdLanguageDelegate);
    connect(_qdvsdVocabularySettingsDialog.qtvFields->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvFieldsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

    _qdvsdVocabularySettingsDialog.qcbFieldType->addItem(tr("Text"));

    for (int iColumn = 0; iColumn < _qdvsdVocabularySettingsDialog.qtvFields->header()->count(); iColumn++) {
        _qdvsdVocabularySettingsDialog.qtvFields->header()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for
} // PrepareFields

const void VocabularySettingsDialog::PreparePlugins()
{
    PrepareSpeechPlugins(_qdvsdVocabularySettingsDialog.qcbSpeechLeft);
    PrepareSpeechPlugins(_qdvsdVocabularySettingsDialog.qcbSpeechRight);
} // PreparePlugins

const void VocabularySettingsDialog::PrepareSpeechPlugins(QComboBox *pComboBox)
{
	sSpeechVoice spvVoice;

	spvVoice.etpPlugin = TTSInterface::TTPluginNone;
	_tvVoiceList.append(spvVoice);
	pComboBox->addItem(tr("None"));
	pComboBox->setItemData(pComboBox->count() - 1, _tvVoiceList.size() - 1);

	foreach (const TTSInterface *tiPlugin, _pPlugins->GetTTSPlugins()) {
        TTSInterface::tVoiceInfoList tvilVoices = tiPlugin->GetVoicesInfo();
        foreach (TTSInterface::sVoiceInfo sviVoice, tvilVoices) {
			spvVoice.etpPlugin = tiPlugin->GetPluginId();
			spvVoice.qsVoiceId = sviVoice.qsId;
			_tvVoiceList.append(spvVoice);
			pComboBox->addItem(QString("%1 (%2)").arg(tiPlugin->GetPluginName()).arg(sviVoice.qsDescription));
			pComboBox->setItemData(pComboBox->count() - 1, _tvVoiceList.size() - 1);
		} // foreach
	} // foreach
} // PrepareSpeechPlugins
#endif

const void VocabularySettingsDialog::SaveOptions()
{
    // languages
    _vVocabulary->SetSettings(KEY_LANGUAGE1, _qdvsdVocabularySettingsDialog.leLanguageLeft->text());
    _vVocabulary->SetSettings(KEY_LANGUAGE2, _qdvsdVocabularySettingsDialog.leLanguageRight->text());
#ifndef FREE
	sSpeechVoice spvVoice = _tvVoiceList.at(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->currentIndex()).toInt());
	_vVocabulary->SetSettings(KEY_SPEECH1, QString::number(spvVoice.etpPlugin));
	_vVocabulary->SetSettings(KEY_VOICE1, spvVoice.qsVoiceId);
	spvVoice = _tvVoiceList.at(_qdvsdVocabularySettingsDialog.qcbSpeechRight->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechRight->currentIndex()).toInt());
	_vVocabulary->SetSettings(KEY_SPEECH2, QString::number(spvVoice.etpPlugin));
	_vVocabulary->SetSettings(KEY_VOICE2, spvVoice.qsVoiceId);

	// templates
	_vVocabulary->SetSettings(KEY_LEARNINGTEMPLATE1, _qdvsdVocabularySettingsDialog.qteLearningLeft->toPlainText());
	_vVocabulary->SetSettings(KEY_LEARNINGTEMPLATE2, _qdvsdVocabularySettingsDialog.qteLearningRight->toPlainText());
#endif
} // SaveOptions

VocabularySettingsDialog::VocabularySettingsDialog(Vocabulary *pVocabulary,
#ifndef FREE
    const Plugins *pPlugins,
#endif
    QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
#ifndef FREE
    , _fmFieldsModel(pVocabulary), _lfdLanguageDelegate(pVocabulary)
#endif
{
	_vVocabulary = pVocabulary;
#ifndef FREE
	_pPlugins = pPlugins;
#endif

	_qdvsdVocabularySettingsDialog.setupUi(this);
#ifdef FREE
    delete _qdvsdVocabularySettingsDialog.qgbSpeech;
    _qdvsdVocabularySettingsDialog.qtwTabs->removeTab(TabFields);
	_qdvsdVocabularySettingsDialog.qtwTabs->removeTab(TabTemplates);
#else

	PreparePlugins();
    PrepareFields();
#endif
	FillOptions();
} // VocabularySettingsDialog