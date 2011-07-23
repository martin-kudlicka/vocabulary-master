#ifndef VOCABULARYSETTINGSDIALOG_H
#define VOCABULARYSETTINGSDIALOG_H

#include <ui_vocabularysettingsdialog.h>

#include "vocabulary.h"
#ifndef FREE
# include "plugins.h"
#endif

class VocabularySettingsDialog : public QDialog
{
	Q_OBJECT

	public:
		VocabularySettingsDialog(const Vocabulary *pVocabulary,
#ifndef FREE
            const Plugins *pPlugins,
#endif
            QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
#ifdef FREE
		enum eTab {
			TabLanguages,
			TabTemplates
		}; // eTab
#else
		struct sSpeechVoice {
			TTSInterface::eTTSPlugin etpPlugin;
			QString qsVoiceId;
		};

		typedef QList<sSpeechVoice> tVoices;

		const Plugins *_pPlugins;
		tVoices _tvVoiceList;
#endif
		Ui::qdVocabularySettingsDialog _qdvsdVocabularySettingsDialog;
		const Vocabulary *_vVocabulary;

		void accept();
		const void FillOptions();
#ifndef FREE
		const void FillSpeech(QComboBox *pComboBox, const QString &pSpeech, const QString &pVoice);
        const void PreparePlugins();
		const void PrepareSpeechPlugins(QComboBox *pComboBox);
#endif
		const void SaveOptions();
}; // VocabularySettingsDialog

#endif // VOCABULARYSETTINGSDIALOG_H