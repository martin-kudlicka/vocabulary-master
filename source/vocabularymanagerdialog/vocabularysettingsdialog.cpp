#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

void VocabularySettingsDialog::accept()
{
	SaveOptions();

	QDialog::accept();
} // accept

const void VocabularySettingsDialog::FillOptions()
{
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechLeft, COLUMN_SPEECH1);
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechRight, COLUMN_SPEECH2);
} // FillOptions

const void VocabularySettingsDialog::FillPlugins()
{
	FillSpeechPlugins(_qdvsdVocabularySettingsDialog.qcbSpeechLeft);
	FillSpeechPlugins(_qdvsdVocabularySettingsDialog.qcbSpeechRight);
} // FillPlugins

const void VocabularySettingsDialog::FillSpeech(QComboBox *pComboBox, const QString &pSpeech)
{
	int iSpeech = _vVocabulary->GetSettings(pSpeech).toInt();
	for (int iI = 0; iI < pComboBox->count(); iI++) {
		if (pComboBox->itemData(iI) == iSpeech) {
			pComboBox->setCurrentIndex(iI);
			return;
		} // if
	} // for
} // FillSpeech

const void VocabularySettingsDialog::FillSpeechPlugins(QComboBox *pComboBox)
{
	pComboBox->addItem(tr("None"));
	pComboBox->setItemData(pComboBox->count() - 1, TTSInterface::TTPluginNone);

	foreach (TTSInterface::eTTSPlugin etpPlugin, _pPlugins->GetPluginList()) {
		pComboBox->addItem(_pPlugins->GetPlugin(etpPlugin)->GetPluginName());
		pComboBox->setItemData(pComboBox->count() - 1, etpPlugin);
	} // foreach
} // FillSpeechPlugins

const void VocabularySettingsDialog::SaveOptions()
{
	_vVocabulary->SetSettings(COLUMN_SPEECH1, _qdvsdVocabularySettingsDialog.qcbSpeechLeft->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->currentIndex()).toString());
	_vVocabulary->SetSettings(COLUMN_SPEECH2, _qdvsdVocabularySettingsDialog.qcbSpeechRight->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechRight->currentIndex()).toString());
} // SaveOptions

VocabularySettingsDialog::VocabularySettingsDialog(const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_vVocabulary = pVocabulary;
	_pPlugins = pPlugins;

	_qdvsdVocabularySettingsDialog.setupUi(this);

	FillPlugins();
	FillOptions();
} // VocabularySettingsDialog