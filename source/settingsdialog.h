#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>
#include <ui_settingsdialog.h>

#include "settings.h"

const QString LANG_SUFFIX = "qm";

class SettingsDialog : public QDialog
{
	Q_OBJECT

	public:
		SettingsDialog(Settings *pSettings, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
#if defined(FREE) || !defined(Q_WS_WIN)
		enum eTab {
			TabGeneral,
			TabLearning,
			TabAppearance,
            TabHotkey
		}; // eTab
#endif

		Settings *_sSettings;
		Ui::qdUiSettings _usdSettingsDialog;

        void accept();
#ifndef FREE
        const void FillColorFlash();
#endif
        const void FillOptions();
        const void FillTranslation();
#ifndef FREE
		const void PrepareColorFlash();
#endif
        const void PrepareTranslations();
        const void SaveOptions();

	private slots:
#ifndef FREE
		const void on_qcbSystemTrayIcon_stateChanged(int state);
		const void on_qsbWordsFrequency_valueChanged(int i);
#endif
}; // MainWindow

#endif // SETTINGSDIALOG_H