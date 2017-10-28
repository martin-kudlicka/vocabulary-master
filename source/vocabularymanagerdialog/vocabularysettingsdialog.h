#ifndef VOCABULARYSETTINGSDIALOG_H
#define VOCABULARYSETTINGSDIALOG_H

#include <ui_vocabularysettingsdialog.h>

#include "vocabularymanagerdialog/vocabularysettingsdialog/fieldsmodel.h"
#include "../plugins/common/tts-interface.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/languagefielddelegate.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog/lineeditpersistentdelegate.h"

class Plugins;

class VocabularySettingsDialog : public QDialog
{
  Q_OBJECT

  public:
             VocabularySettingsDialog(const QSharedPointer<Vocabulary> &vocabulary, const Plugins *plugins, QWidget *parent);
    virtual ~VocabularySettingsDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
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
          Ui::VocabularySettingsDialog _ui;
    const QSharedPointer<Vocabulary>   _vocabulary;

    virtual void accept                   () Q_DECL_OVERRIDE;
            void actualizeFieldsEditor    ()             const;
            void actualizeFieldsEditor    (quintptr row) const;
            void fillOptions              ();
            void fillSpeech               (QComboBox *comboBox, TTSInterface::TTSPlugin speechPlugin, const QString &voiceId);
            void prepareFields            ();
            void preparePlugins           ();
            void prepareSpeechPlugins     (QComboBox *comboBox);
            void refreshLanguageNameFields()             const;
            void saveOptions              ();

  private Q_SLOTS:
    void on_fieldAdd_clicked                     (bool checked = false);
    void on_fieldDown_clicked                    (bool checked = false);
    void on_fieldRemove_clicked                  (bool checked = false);
    void on_fieldUp_clicked                      (bool checked = false);
    void on_fieldsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
    void on_languageLeft_textEdited              (const QString &text)                                              const;
    void on_languageRight_textEdited             (const QString &text)                                              const;
};

#endif