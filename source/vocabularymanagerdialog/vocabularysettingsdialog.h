#ifndef VOCABULARYSETTINGSDIALOG_H
#define VOCABULARYSETTINGSDIALOG_H

#include "ui_vocabularysettingsdialog.h"

#include "vocabulary.h"
#include "plugins.h"

class VocabularySettingsDialog : public QDialog
{
	Q_OBJECT

	public:
		VocabularySettingsDialog(const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		const Plugins *_pPlugins;
		Ui::qdVocabularySettingsDialog _qdvsdVocabularySettingsDialog;
		const Vocabulary *_vVocabulary;

		void accept();
		const void FillOptions();
		const void FillPlugins();
		const void FillSpeech(QComboBox *pComboBox, const QString &pSpeech);
		const void FillSpeechPlugins(QComboBox *pComboBox);
		const void SaveOptions();
}; // VocabularySettingsDialog

#endif // VOCABULARYSETTINGSDIALOG_H