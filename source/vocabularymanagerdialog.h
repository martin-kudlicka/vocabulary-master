#ifndef VOCABULARYMANAGERDIALOG_H
#define VOCABULARYMANAGERDIALOG_H

#include "ui_vocabularymanagerdialog.h"

#include "vocabulary.h"
#include "plugins.h"

class VocabularyManagerDialog : public QDialog
{
    Q_OBJECT

    public:
        VocabularyManagerDialog(const Vocabulary *pVocabulary, const Plugins *pPlugins, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
		const Plugins *_pPlugins;
        QList<int> _qlCategories;
        Ui::qdVocabularyManager _qdvmVocabularyManager;
        const Vocabulary *_vVocabulary;

        const void AddTab(const int &pCategoryId);
        const void EnableControls();
        const void InitTabs();

    private slots:
        const void on_qpbCategoryAdd_clicked(bool checked = false);
        const void on_qpbCategoryRemove_clicked(bool checked = false);
		const void on_qpbVocabularySettings_clicked(bool checked = false);
        const void on_qpbWordAdd_clicked(bool checked = false);
        const void on_qpbWordRemove_clicked(bool checked = false);
        const void on_TabEnableChanged(const int &pIndex, const Qt::CheckState &pState) const;
}; // VocabularyManagerDialog

#endif // VOCABULARYMANAGERDIALOG_H