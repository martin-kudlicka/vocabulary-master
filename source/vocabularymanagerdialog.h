#ifndef VOCABULARYMANAGERDIALOG_H
#define VOCABULARYMANAGERDIALOG_H

#include <ui_vocabularymanagerdialog.h>

#ifndef EDITION_FREE
# include "settings.h"
# include "plugins.h"
#endif
#include "vocabularymanagerdialog/vocabularyview.h"

class VocabularyManagerDialog : public QDialog
{
    Q_OBJECT

    public:
                 VocabularyManagerDialog(Vocabulary *vocabulary,
#ifndef EDITION_FREE
            const Settings *settings,
            const Plugins *plugins,
#endif
            QWidget *parent = NULL, Qt::WindowFlags flags = 0);
		virtual ~VocabularyManagerDialog();

#ifndef EDITION_FREE
        void execOnRecord(quint32 recordId);
#endif

    private:
		enum EditorColumn
		{
			EditorColumnLeftLabel,
			EditorColumnLeftControl,
			EditorColumnRightLabel,
			EditorColumnRightControl
		}; // EditorColumn

#ifndef EDITION_FREE
		static const quint8 BUILTIN_COLUMN_SIZE = 30;
		static const qint8  COLUMNS_NONE        = -1;

		const Plugins *_plugins;
#endif
        QList<quint8>               _categories;
#ifndef EDITION_FREE
        const Settings             *_settings;
#endif
		Ui::VocabularyManagerDialog _ui;
        Vocabulary                 *_vocabulary;

		void addTab               (quint8 categoryId);
		void enableTabControls    ()                                const;
		void enableWordControls   ()                                const;
		void focusOnRecord        (quint32 recordId)                const;
#ifndef EDITION_FREE
		void hideColumns          ()                                const;
#endif
		void hideColumns          (VocabularyView *tableView)       const;
		void initEditor           ();
        void initTabs             ();
#ifndef EDITION_FREE
        void reassignModels       ()                                const;
        void selectFirstEnabledTab();
		void setPriorityDelegate  ();
		void setPriorityDelegate  (VocabularyView *tableView);
        void stretchColumns       ()                                const;
#endif
		void stretchColumns       (const VocabularyView *tableView) const;
#ifndef EDITION_FREE
        void uninitEditor         ()                                const;
#endif
		void updateEditor         ()                                const;
		void updateEditor         (EditorColumn controlsColumn)     const;

    private slots:
		void on_control_textEdited                           (const QString &text)                                              const;
        void on_categoryAdd_clicked                          (bool checked = false);
        void on_categoryRemove_clicked                       (bool checked = false);
        void on_searchButton_clicked                         (bool checked = false)                                             const;
		void on_searchEdit_textChanged                       (const QString &text)                                              const;
		void on_tabs_currentChanged                          (int index)                                                        const;
#ifndef EDITION_FREE
        void on_tabs_TabEnableChanged                        (quint8 index, Qt::CheckState state)                               const;
		void on_tabs_TabPriorityChanged                      (quint8 index, quint8 value)                                       const;
#endif
		void on_vocabularySettings_clicked                   (bool checked = false);
		void on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
        void on_wordAdd_clicked                              (bool checked = false);
#ifndef EDITION_FREE
		void on_wordCopyMove_clicked                         (bool checked = false);
		void on_wordExport_clicked                           (bool checked = false);
		void on_wordImport_clicked                           (bool checked = false);
#endif
        void on_wordRemove_clicked                           (bool checked = false);
}; // VocabularyManagerDialog

#endif // VOCABULARYMANAGERDIALOG_H