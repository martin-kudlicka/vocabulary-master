#ifndef VOCABULARYMANAGERDIALOG_H
#define VOCABULARYMANAGERDIALOG_H

#include <ui_vocabularymanagerdialog.h>

#include "vocabulary.h"
#ifndef FREE
# include "plugins.h"
#endif

class VocabularyManagerDialog : public QDialog
{
    Q_OBJECT

    public:
        VocabularyManagerDialog(const Vocabulary *pVocabulary,
#ifndef FREE
            const Plugins *pPlugins,
#endif
            QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
		enum eEditorColumn {
			EditorColumnLeftLabel,
			EditorColumnLeftControl,
			EditorColumnRightLabel,
			EditorColumnRightControl
		}; // eEditorColumn

#ifndef FREE
		const Plugins *_pPlugins;
#endif
        QList<int> _qlCategories;
        Ui::qdVocabularyManager _qdvmVocabularyManager;
        const Vocabulary *_vVocabulary;

        const void AddTab(const int &pCategoryId);
		const void InitEditor();
        const void InitTabs();
#ifndef FREE
        const void SelectFirstEnabledTab();
#endif
		const void UpdateEditor() const;
		const void UpdateEditor(const QGridLayout *pGridLayout, const eEditorColumn &pControlsColumn) const;

    private slots:
		const void on_qleControl_textEdited(const QString &pText) const;
        const void on_qleSearch_textChanged(const QString &text) const;
        const void on_qpbCategoryAdd_clicked(bool checked = false);
        const void on_qpbCategoryRemove_clicked(bool checked = false);
        const void on_qpbSearch_clicked(bool checked = false) const;
		const void on_qpbVocabularySettings_clicked(bool checked = false);
        const void on_qpbWordAdd_clicked(bool checked = false);
#ifndef FREE
		const void on_qpbWordImport_clicked(bool checked = false);
#endif
        const void on_qpbWordRemove_clicked(bool checked = false);
		const void on_qtvTableViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
		const void on_vtwTabs_currentChanged(int index) const;
#ifndef FREE
        const void on_vtwTabs_TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const;
#endif
}; // VocabularyManagerDialog

#endif // VOCABULARYMANAGERDIALOG_H