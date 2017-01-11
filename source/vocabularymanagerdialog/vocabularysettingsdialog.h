#ifndef VOCABULARYSETTINGSDIALOG_H
#define VOCABULARYSETTINGSDIALOG_H

#include <ui_vocabularysettingsdialog.h>

#ifndef EDITION_FREE
# include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"
# include "../plugins/common/tts-interface.h"
# include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"
# include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"
#endif

class Plugins;

class VocabularySettingsDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularySettingsDialog(Vocabulary *vocabulary,
#ifndef EDITION_FREE
                                      const Plugins *plugins,
#endif
                                      QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~VocabularySettingsDialog() Q_DECL_OVERRIDE;

  private:
#ifdef EDITION_FREE
    enum class Tab
    {
      Languages,
      Templates,
      Fields
    };
#else
    struct SpeechVoice
    {
      TTSInterface::TTSPlugin ttsPlugin;
      QString                 voiceId;
    };

    using VoiceList = QList<SpeechVoice>;

          FieldsModel                  _fieldsModel;
          LanguageFieldDelegate        _languageFieldDelegate;
          LineEditPersistentDelegate   _lineEditDelegate;
    const Plugins                     *_plugins;
          VoiceList                    _voices;
#endif
          Ui::VocabularySettingsDialog _ui;
    const Vocabulary                  *_vocabulary;

    virtual void accept                   () Q_DECL_OVERRIDE;
#ifndef EDITION_FREE
            void actualizeFieldsEditor    ()             const;
            void actualizeFieldsEditor    (quintptr row) const;
#endif
            void fillOptions              ();
#ifndef EDITION_FREE
            void fillSpeech               (QComboBox *comboBox, const QString &speech, const QString &voice);
            void prepareFields            ();
            void preparePlugins           ();
            void prepareSpeechPlugins     (QComboBox *comboBox);
            void refreshLanguageNameFields()             const;
#endif
            void saveOptions              ();

#ifndef EDITION_FREE
    private Q_SLOTS:
      void on_fieldAdd_clicked                     (bool checked = false);
      void on_fieldDown_clicked                    (bool checked = false);
      void on_fieldRemove_clicked                  (bool checked = false);
      void on_fieldUp_clicked                      (bool checked = false);
      void on_fieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
      void on_languageLeft_textEdited              (const QString &text)                                              const;
      void on_languageRight_textEdited             (const QString &text)                                              const;
#endif
};

#endif