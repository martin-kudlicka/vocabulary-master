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
        ~VocabularyManagerDialog();
        VocabularyManagerDialog(Vocabulary *pVocabulary,
#ifndef EDITION_FREE
            const Settings *pSettings,
            const Plugins *pPlugins,
#endif
            QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

#ifndef EDITION_FREE
        int ExecOnRecord(const int &pRecordId);
#endif

    private:
		enum eEditorColumn {
			EditorColumnLeftLabel,
			EditorColumnLeftControl,
			EditorColumnRightLabel,
			EditorColumnRightControl
		}; // eEditorColumn

#ifndef EDITION_FREE
		static const int BUILTIN_COLUMN_SIZE = 30;
		static const int COLUMNS_NONE = -1;

		const Plugins *_pPlugins;
#endif
        QList<int> _qlCategories;
#ifndef EDITION_FREE
        const Settings *_sSettings;
#endif
        Ui::qdVocabularyManager _qdvmVocabularyManager;
        Vocabulary *_vVocabulary;

        const void AddTab(const int &pCategoryId);
		const void EnableTabControls() const;
		const void EnableWordControls() const;
        const void FocusOnRecord(const int &pRecordId) const;
#ifndef EDITION_FREE
		const void HideColumns() const;
#endif
		const void HideColumns(VocabularyView *pTableView) const;
		const void InitEditor();
        const void InitTabs();
#ifndef EDITION_FREE
        const void ReassignModels() const;
        const void SelectFirstEnabledTab();
		const void SetPriorityDelegate();
		const void SetPriorityDelegate(VocabularyView *pTableView);
        const void StretchColumns() const;
#endif
		const void StretchColumns(const VocabularyView *pTableView) const;
#ifndef EDITION_FREE
        const void UninitEditor() const;
#endif
		const void UpdateEditor() const;
		const void UpdateEditor(const eEditorColumn &pControlsColumn) const;

    private slots:
		const void on_qleControl_textEdited(const QString &pText) const;
        const void on_qleSearch_textChanged(const QString &text) const;
        const void on_qpbCategoryAdd_clicked(bool checked = false);
        const void on_qpbCategoryRemove_clicked(bool checked = false);
        const void on_qpbSearch_clicked(bool checked = false) const;
		const void on_qpbVocabularySettings_clicked(bool checked = false);
        const void on_qpbWordAdd_clicked(bool checked = false);
#ifndef EDITION_FREE
		const void on_qpbWordCopyMove_clicked(bool checked = false);
		const void on_qpbWordExport_clicked(bool checked = false);
		const void on_qpbWordImport_clicked(bool checked = false);
#endif
        const void on_qpbWordRemove_clicked(bool checked = false);
		const void on_vtwTabs_currentChanged(int index) const;
#ifndef EDITION_FREE
        const void on_vtwTabs_TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const;
		const void on_vtwTabs_TabPriorityChanged(const int &pIndex, const int &pValue) const;
#endif
        const void on_vvVocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // VocabularyManagerDialog

#endif // VOCABULARYMANAGERDIALOG_H