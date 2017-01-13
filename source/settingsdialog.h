#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets/QDialog>
#include <ui_settingsdialog.h>

#include "settings.h"
#ifndef EDITION_FREE
# include "settingsdialog/pluginsmodel.h"
#endif

const QString LANG_SUFFIX = "qm";

class SettingsDialog : public QDialog
{
  Q_OBJECT

  public:
#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
    static const auto VIRTUALKEY_NONE = 0;
#endif

             SettingsDialog(
#ifndef EDITION_FREE
      const Plugins *plugins,
#endif
      Settings *settings, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~SettingsDialog() Q_DECL_OVERRIDE;

  private:
#if defined(EDITION_FREE) || !defined(Q_OS_WIN)
    enum class Tab
    {
      General,
      Learning,
      Appearance,
      Hotkey,
      Plugins,
      Network
    };
#endif

#ifndef EDITION_FREE
    const Plugins           *_plugins;
          PluginsModel       _expPluginsModel;
          PluginsModel       _impPluginsModel;
          PluginsModel       _ttsPluginsModel;
#endif
          Settings          *_settings;
          Ui::SettingsDialog _ui;

#ifndef EDITION_FREE
# ifdef Q_OS_WIN
    void clearHotkey        (HotkeyLineEdit *control)                                const;
# endif
    void fillColorFlash     ();
# ifdef Q_OS_WIN
    void fillHotkey         (HotkeyLineEdit *control, Settings::Hotkey hotkey)       const;
# endif
#endif
    void fillOptions        ();
    void fillTranslation    ();
#ifndef EDITION_FREE
    void prepareColorFlash  ();
    void preparePlugins     (QTreeView *treeView, PluginsModel *model)               const;
#endif
    void prepareTranslations();
#if !defined(EDITION_FREE) && defined(Q_OS_WIN)
    void saveHotkey         (const HotkeyLineEdit *control, Settings::Hotkey hotkey) const;
#endif
    void saveOptions        ();

    virtual void accept() Q_DECL_OVERRIDE;

  private Q_SLOTS:
#ifndef EDITION_TRY
    void on_cacheVocabulary_stateChanged(int state)            const;
#endif
#ifndef EDITION_FREE
# ifdef Q_OS_WIN
    void on_hotkeyAnswerClear_clicked   (bool checked = false) const;
    void on_hotkeyMinimizeClear_clicked (bool checked = false) const;
    void on_hotkeyNextClear_clicked     (bool checked = false) const;
    void on_hotkeyRestoreClear_clicked  (bool checked = false) const;
# endif
    void on_newWordSound_stateChanged   (int state)            const;
    void on_showLicense_clicked         (bool checked = false);
    void on_soundBrowse_clicked         (bool checked = false);
    void on_soundCustomRadio_clicked    (bool checked = false) const;
    void on_soundSystem_clicked         (bool checked = false) const;
    void on_systemTrayIcon_stateChanged (int state)            const;
    void on_wordsFrequency_valueChanged (int i)                const;
#endif
};

#endif