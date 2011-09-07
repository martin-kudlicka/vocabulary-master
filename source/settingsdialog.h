#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>
#include <ui_settingsdialog.h>

#include "settings.h"
#ifndef FREE
# include "settingsdialog/pluginsmodel.h"
#endif

const QString LANG_SUFFIX = "qm";

class SettingsDialog : public QDialog
{
	Q_OBJECT

	public:
#if !defined(FREE) && defined(Q_WS_WIN)
		static const quint32 VIRTUALKEY_NONE = 0;
#endif

		SettingsDialog(
#ifndef FREE
            const Plugins *pPlugins,
#endif
            Settings *pSettings, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
#if defined(FREE) || !defined(Q_WS_WIN)
		enum eTab {
			TabGeneral,
			TabLearning,
			TabAppearance,
            TabHotkey,
            TabPlugins
		}; // eTab
#endif

#ifndef FREE
        const Plugins *_pPlugins;
        PluginsModel _pmExpPlugins;
        PluginsModel _pmImpPlugins;
        PluginsModel _pmTTSPlugins;
#endif
		Settings *_sSettings;
		Ui::qdUiSettings _usdSettingsDialog;

        virtual void accept();
#ifndef FREE
# ifdef Q_WS_WIN
		const void ClearHotkey(HotkeyLineEdit *pControl) const;
# endif
        const void FillColorFlash();
# ifdef Q_WS_WIN
		const void FillHotkey(HotkeyLineEdit *pControl, const Settings::eHotkey &pHotkey) const;
# endif
#endif
        const void FillOptions();
        const void FillTranslation();
#ifndef FREE
		const void PrepareColorFlash();
        const void PreparePlugins(QTreeView *pTreeView, PluginsModel *pModel) const;
#endif
        const void PrepareTranslations();
#if !defined(FREE) && defined(Q_WS_WIN)
		const void SaveHotkey(const HotkeyLineEdit *pControl, const Settings::eHotkey &pHotkey) const;
#endif
        const void SaveOptions();

	private slots:
#ifndef FREE
        const void on_qcbNewWordSound_stateChanged(int state) const;
		const void on_qcbSystemTrayIcon_stateChanged(int state) const;
# ifdef Q_WS_WIN
		const void on_qpbHotkeyAnswerClear_clicked(bool checked = false) const;
		const void on_qpbHotkeyMinimizeClear_clicked(bool checked = false) const;
		const void on_qpbHotkeyNextClear_clicked(bool checked = false) const;
		const void on_qpbHotkeyRestoreClear_clicked(bool checked = false) const;
# endif
        const void on_qpbShowLicense_clicked(bool checked = false);
        const void on_qbpSoundBrowse_clicked(bool checked = false);
        const void on_qrbSoundCustom_clicked(bool checked = false) const;
        const void on_qrbSoundSystem_clicked(bool checked = false) const;
		const void on_qsbWordsFrequency_valueChanged(int i) const;
#endif
}; // MainWindow

#endif // SETTINGSDIALOG_H