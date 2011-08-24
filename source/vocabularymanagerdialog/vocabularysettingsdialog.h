#ifndef VOCABULARYSETTINGSDIALOG_H
#define VOCABULARYSETTINGSDIALOG_H

#include <ui_vocabularysettingsdialog.h>

#ifdef FREE
# include "vocabulary.h"
#else
# include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"
# include "plugins.h"
# include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"
# include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"
#endif

class VocabularySettingsDialog : public QDialog
{
	Q_OBJECT

	public:
		VocabularySettingsDialog(Vocabulary *pVocabulary,
#ifndef FREE
            const Plugins *pPlugins,
#endif
            QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
#ifdef FREE
		enum eTab {
			TabLanguages,
			TabTemplates,
            TabFields
		}; // eTab
#else
		struct sSpeechVoice {
			TTSInterface::eTTSPlugin etpPlugin;
			QString qsVoiceId;
		};

		typedef QList<sSpeechVoice> tVoices;

        FieldsModel _fmFieldsModel;
        LanguageFieldDelegate _lfdLanguageDelegate;
        LineEditPersistentDelegate _lepdLineEditDelegate;
		const Plugins *_pPlugins;
		tVoices _tvVoiceList;
#endif
		Ui::qdVocabularySettingsDialog _qdvsdVocabularySettingsDialog;
		const Vocabulary *_vVocabulary;

		virtual void accept();
		const void FillOptions();
#ifndef FREE
		const void FillSpeech(QComboBox *pComboBox, const QString &pSpeech, const QString &pVoice);
        const void PrepareFields();
        const void PreparePlugins();
		const void PrepareSpeechPlugins(QComboBox *pComboBox);
#endif
		const void SaveOptions();

#ifndef FREE
	private slots:
        const void on_qpbFieldAdd_clicked(bool checked = false);
		const void on_qpbFieldDown_clicked(bool checked = false);
		const void on_qpbFieldRemove_clicked(bool checked = false);
		const void on_qpbFieldUp_clicked(bool checked = false);
		const void on_qtvFieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
#endif
}; // VocabularySettingsDialog

#endif // VOCABULARYSETTINGSDIALOG_H