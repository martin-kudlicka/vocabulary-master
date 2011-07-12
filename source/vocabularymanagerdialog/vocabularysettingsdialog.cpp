#include "vocabularymanagerdialog/vocabularysettingsdialog.h"

void VocabularySettingsDialog::accept()
{
	SaveOptions();

	QDialog::accept();
} // accept

const void VocabularySettingsDialog::FillOptions()
{
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechLeft, KEY_SPEECH1, KEY_VOICE1);
	FillSpeech(_qdvsdVocabularySettingsDialog.qcbSpeechRight, KEY_SPEECH2, KEY_VOICE2);
} // FillOptions

const void VocabularySettingsDialog::FillPlugins()
{
	FillSpeechPlugins(_qdvsdVocabularySettingsDialog.qcbSpeechLeft);
	FillSpeechPlugins(_qdvsdVocabularySettingsDialog.qcbSpeechRight);
} // FillPlugins

const void VocabularySettingsDialog::FillSpeech(QComboBox *pComboBox, const QString &pSpeech, const QString &pVoice)
{
	int iSpeech = _vVocabulary->GetSettings(pSpeech).toInt();
	QString qsVoice = _vVocabulary->GetSettings(pVoice);

	for (int iI = 0; iI < pComboBox->count(); iI++) {
		sSpeechVoice spvVoice = _tvVoiceList.at(pComboBox->itemData(iI).toInt());
		if (spvVoice.etpPlugin == iSpeech && spvVoice.qsVoice == qsVoice) {
			pComboBox->setCurrentIndex(iI);
			return;
		} // if
	} // for
} // FillSpeech

const void VocabularySettingsDialog::FillSpeechPlugins(QComboBox *pComboBox)
{
	sSpeechVoice spvVoice;

	spvVoice.etpPlugin = TTSInterface::TTPluginNone;
	_tvVoiceList.append(spvVoice);
	pComboBox->addItem(tr("None"));
	pComboBox->setItemData(pComboBox->count() - 1, _tvVoiceList.size() - 1);

	foreach (TTSInterface::eTTSPlugin etpPlugin, _pPlugins->GetPluginIds()) {
		const TTSInterface *tiPlugin = _pPlugins->GetPlugin(etpPlugin);

		QStringList qslVoicesIds = tiPlugin->GetVoicesIds();
		foreach (QString qsVoiceId, qslVoicesIds) {
			spvVoice.etpPlugin = etpPlugin;
			spvVoice.qsVoice = qsVoiceId;
			_tvVoiceList.append(spvVoice);
			pComboBox->addItem(QString("%1 (%2)").arg(tiPlugin->GetPluginName()).arg(qsVoiceId));
			pComboBox->setItemData(pComboBox->count() - 1, _tvVoiceList.size() - 1);
		} // foreach
	} // foreach
} // FillSpeechPlugins

const void VocabularySettingsDialog::SaveOptions()
{
	sSpeechVoice spvVoice = _tvVoiceList.at(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechLeft->currentIndex()).toInt());
	_vVocabulary->SetSettings(KEY_SPEECH1, QString::number(spvVoice.etpPlugin));
	_vVocabulary->SetSettings(KEY_VOICE1, spvVoice.qsVoice);
	spvVoice = _tvVoiceList.at(_qdvsdVocabularySettingsDialog.qcbSpeechRight->itemData(_qdvsdVocabularySettingsDialog.qcbSpeechRight->currentIndex()).toInt());
	_vVocabulary->SetSettings(KEY_SPEECH2, QString::number(spvVoice.etpPlugin));
	_vVocabulary->SetSettings(KEY_VOICE2, spvVoice.qsVoice);
} // SaveOptions

VocabularySettingsDialog::VocabularySettingsDialog(const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags)
{
	_vVocabulary = pVocabulary;
	_pPlugins = pPlugins;

	_qdvsdVocabularySettingsDialog.setupUi(this);

	FillPlugins();
	FillOptions();
} // VocabularySettingsDialog