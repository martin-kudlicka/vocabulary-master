#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

void VocabularySettingsDialog::accept()
{
	SaveOptions();

	QDialog::accept();
} // accept

#ifndef EDITION_FREE
const void VocabularySettingsDialog::ActualizeFieldsEditor() const
{
    for (int iRow = 0; iRow < _fmFieldsModel.rowCount(); iRow++) {
        ActualizeFieldsEditor(iRow);
    } // for
} // ActualizeFieldsEditor

const void VocabularySettingsDialog::ActualizeFieldsEditor(const int &pRow) const
{
	int iFieldId = _vVocabulary->GetFieldId(pRow);

	QModelIndex qmiIndex = _fmFieldsModel.index(pRow, FieldsModel::ColumnTemplateName);
	_qdvsdVocabularySettingsDialog.qtvFields->openPersistentEditor(qmiIndex);
	qmiIndex = _fmFieldsModel.index(pRow, FieldsModel::ColumnName);
	if (_vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
		_qdvsdVocabularySettingsDialog.qtvFields->closePersistentEditor(qmiIndex);
	} else {
		_qdvsdVocabularySettingsDialog.qtvFields->openPersistentEditor(qmiIndex);
	} // if else
	qmiIndex = _fmFieldsModel.index(pRow, FieldsModel::ColumnLanguage);
	if (_vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
		_qdvsdVocabularySettingsDialog.qtvFields->closePersistentEditor(qmiIndex);
	} else {
		_qdvsdVocabularySettingsDialog.qtvFields->openPersistentEditor(qmiIndex);
	} // if else
} // ActualizeFieldsEditor
#endif

const void VocabularySettingsDialog::FillOptions()
{
    // languages
    _qdvsdVocabularySettingsDialog.leLanguageLeft->setText(_vVocabulary->GetLanguageName(VocabularyDatabase::FieldLanguageLeft));
    _qdvsdVocabularySettingsDialog.leLanguageRight->setText(_vVocabulary->GetLanguageName(VocabularyDatabase::FieldLanguageRight));
#ifndef EDITION_FREE
    FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechLeft, QString::number(_vVocabulary->GetLanguageSpeech(VocabularyDatabase::FieldLanguageLeft)), _vVocabulary->GetLanguageVoice(VocabularyDatabase::FieldLanguageLeft));
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechRight, QString::number(_vVocabulary->GetLanguageSpeech(VocabularyDatabase::FieldLanguageRight)), _vVocabulary->GetLanguageVoice(VocabularyDatabase::FieldLanguageRight));

	// templates
	_qdvsdVocabularySettingsDialog.qteLearningLeft->setPlainText(_vVocabulary->GetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageLeft));
	_qdvsdVocabularySettingsDialog.qteLearningRight->setPlainText(_vVocabulary->GetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageRight));
    _qdvsdVocabularySettingsDialog.qteTrayLeft->setPlainText(_vVocabulary->GetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageLeft));
    _qdvsdVocabularySettingsDialog.qteTrayRight->setPlainText(_vVocabulary->GetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageRight));
#endif
} // FillOptions

#ifndef EDITION_FREE
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

const void VocabularySettingsDialog::on_leLanguageLeft_textEdited(const QString &text) const
{
    RefreshLanguageNameFields();
} // on_leLanguageLeft_textEdited

const void VocabularySettingsDialog::on_leLanguageRight_textEdited(const QString &text) const
{
    RefreshLanguageNameFields();
} // on_leLanguageRight_textEdited

const void VocabularySettingsDialog::on_qpbFieldAdd_clicked(bool checked /* false */)
{
    _fmFieldsModel.AddRow();
	ActualizeFieldsEditor(_fmFieldsModel.rowCount() - 1);
} // on_qpbFieldAdd_clicked

const void VocabularySettingsDialog::on_qpbFieldDown_clicked(bool checked /* false */)
{
	QModelIndex qmiCurrent = _qdvsdVocabularySettingsDialog.qtvFields->currentIndex();
	_fmFieldsModel.Swap(qmiCurrent.row(), qmiCurrent.row() + 1);
    ActualizeFieldsEditor();

	_qdvsdVocabularySettingsDialog.qtvFields->setCurrentIndex(_fmFieldsModel.index(qmiCurrent.row() + 1, qmiCurrent.column()));
} // on_qpbFieldDown_clicked

const void VocabularySettingsDialog::on_qpbFieldRemove_clicked(bool checked /* false */)
{
	const QItemSelectionModel *qismSelection = _qdvsdVocabularySettingsDialog.qtvFields->selectionModel();
	_fmFieldsModel.RemoveRow(qismSelection->currentIndex().row());
    ActualizeFieldsEditor();
} // on_qpbFieldRemove_clicked

const void VocabularySettingsDialog::on_qpbFieldUp_clicked(bool checked /* false */)
{
	QModelIndex qmiCurrent = _qdvsdVocabularySettingsDialog.qtvFields->currentIndex();
	_fmFieldsModel.Swap(qmiCurrent.row(), qmiCurrent.row() - 1);
    ActualizeFieldsEditor();

	_qdvsdVocabularySettingsDialog.qtvFields->setCurrentIndex(_fmFieldsModel.index(qmiCurrent.row() - 1, qmiCurrent.column()));
} // on_qpbFieldUp_clicked

const void VocabularySettingsDialog::on_qtvFieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	const QItemSelectionModel *qismSelection = _qdvsdVocabularySettingsDialog.qtvFields->selectionModel();
    bool bBuiltIn;
    if (qismSelection->hasSelection()) {
        int iFieldId = _vVocabulary->GetFieldId(_qdvsdVocabularySettingsDialog.qtvFields->currentIndex().row());
        bBuiltIn = _vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn);
    } // if

	_qdvsdVocabularySettingsDialog.qpbFieldUp->setEnabled(qismSelection->hasSelection() && _qdvsdVocabularySettingsDialog.qtvFields->currentIndex().row() > 0);
	_qdvsdVocabularySettingsDialog.qpbFieldDown->setEnabled(qismSelection->hasSelection() && _qdvsdVocabularySettingsDialog.qtvFields->currentIndex().row() < _qdvsdVocabularySettingsDialog.qtvFields->model()->rowCount() - 1);
	_qdvsdVocabularySettingsDialog.qpbFieldRemove->setEnabled(qismSelection->hasSelection() && !bBuiltIn);
} // on_qtvFieldsSelectionModel_selectionChanged

const void VocabularySettingsDialog::PrepareFields()
{
    _qdvsdVocabularySettingsDialog.qtvFields->setModel(&_fmFieldsModel);
    _qdvsdVocabularySettingsDialog.qtvFields->setItemDelegateForColumn(FieldsModel::ColumnTemplateName, &_lepdLineEditDelegate);
    _qdvsdVocabularySettingsDialog.qtvFields->setItemDelegateForColumn(FieldsModel::ColumnName, &_lepdLineEditDelegate);
    _qdvsdVocabularySettingsDialog.qtvFields->setItemDelegateForColumn(FieldsModel::ColumnLanguage, &_lfdLanguageDelegate);
    connect(_qdvsdVocabularySettingsDialog.qtvFields->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvFieldsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

    _qdvsdVocabularySettingsDialog.qcbFieldType->addItem(tr("Text"));

    ActualizeFieldsEditor();

    for (int iColumn = 0; iColumn < _qdvsdVocabularySettingsDialog.qtvFields->header()->count(); iColumn++) {
        if (iColumn == FieldsModel::ColumnSpeech || iColumn == FieldsModel::ColumnShow) {
            _qdvsdVocabularySettingsDialog.qtvFields->header()->setSectionResizeMode(iColumn, QHeaderView::ResizeToContents);
        } else {
            _qdvsdVocabularySettingsDialog.qtvFields->header()->setSectionResizeMode(iColumn, QHeaderView::Stretch);
        } // if else
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

    foreach (Plugins::sTTSPlugin stpPlugin, _pPlugins->GetTTSPlugins()) {
        const TTSInterface *tiPlugin = stpPlugin.tiInterface;

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

const void VocabularySettingsDialog::RefreshLanguageNameFields() const
{
    for (int iRow = 0; iRow < _fmFieldsModel.rowCount(); iRow++) {
        _vVocabulary->SetLanguageName(VocabularyDatabase::FieldLanguageLeft, _qdvsdVocabularySettingsDialog.leLanguageLeft->text());
        _vVocabulary->SetLanguageName(VocabularyDatabase::FieldLanguageRight, _qdvsdVocabularySettingsDialog.leLanguageRight->text());

        QModelIndex qmiIndex = _fmFieldsModel.index(iRow, FieldsModel::ColumnLanguage);
        _qdvsdVocabularySettingsDialog.qtvFields->closePersistentEditor(qmiIndex);
        _qdvsdVocabularySettingsDialog.qtvFields->openPersistentEditor(qmiIndex);
    } // for
} // RefreshLanguageNameFields
#endif

const void VocabularySettingsDialog::SaveOptions()
{
    // languages
#ifdef EDITION_FREE
    _vVocabulary->SetLanguageName(VocabularyDatabase::FieldLanguageLeft, _qdvsdVocabularySettingsDialog.leLanguageLeft->text());
    _vVocabulary->SetLanguageName(VocabularyDatabase::FieldLanguageRight, _qdvsdVocabularySettingsDialog.leLanguageRight->text());
#else
	sSpeechVoice spvVoice = _tvVoiceList.at(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->currentIndex()).toInt());
	_vVocabulary->SetLanguageSpeech(VocabularyDatabase::FieldLanguageLeft, spvVoice.etpPlugin);
	_vVocabulary->SetLanguageVoice(VocabularyDatabase::FieldLanguageLeft, spvVoice.qsVoiceId);
	spvVoice = _tvVoiceList.at(_qdvsdVocabularySettingsDialog.qcbSpeechRight->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechRight->currentIndex()).toInt());
	_vVocabulary->SetLanguageSpeech(VocabularyDatabase::FieldLanguageRight, spvVoice.etpPlugin);
	_vVocabulary->SetLanguageVoice(VocabularyDatabase::FieldLanguageRight, spvVoice.qsVoiceId);

	// templates
	_vVocabulary->SetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageLeft, _qdvsdVocabularySettingsDialog.qteLearningLeft->toPlainText());
	_vVocabulary->SetLanguageLearningTemplate(VocabularyDatabase::FieldLanguageRight, _qdvsdVocabularySettingsDialog.qteLearningRight->toPlainText());
    _vVocabulary->SetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageLeft, _qdvsdVocabularySettingsDialog.qteTrayLeft->toPlainText());
    _vVocabulary->SetLanguageTrayTemplate(VocabularyDatabase::FieldLanguageRight, _qdvsdVocabularySettingsDialog.qteTrayRight->toPlainText());
#endif
} // SaveOptions

VocabularySettingsDialog::VocabularySettingsDialog(Vocabulary *pVocabulary,
#ifndef EDITION_FREE
    const Plugins *pPlugins,
#endif
    QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
#ifndef EDITION_FREE
    , _fmFieldsModel(pVocabulary), _lfdLanguageDelegate(pVocabulary)
#endif
{
	_vVocabulary = pVocabulary;
#ifndef EDITION_FREE
	_pPlugins = pPlugins;
#endif

	_qdvsdVocabularySettingsDialog.setupUi(this);
#ifdef EDITION_FREE
    delete _qdvsdVocabularySettingsDialog.qgbSpeech;
    _qdvsdVocabularySettingsDialog.qtwTabs->removeTab(TabFields);
	_qdvsdVocabularySettingsDialog.qtwTabs->removeTab(TabTemplates);
#else

	PreparePlugins();
    PrepareFields();
#endif
	FillOptions();
} // VocabularySettingsDialog