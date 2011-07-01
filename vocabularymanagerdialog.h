#ifndef VOCABULARYMANAGERDIALOG_H
#define VOCABULARYMANAGERDIALOG_H

#include "ui_vocabularymanagerdialog.h"

#include "vocabulary.h"

class VocabularyManagerDialog : public QDialog
{
    Q_OBJECT

    public:
        VocabularyManagerDialog(const Vocabulary *pVocabulary, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

    private:
        QHash<int, int> _qhTabCategory;
        Ui::qdVocabularyManager _qdvmVocabularyManager;
        const Vocabulary *_vVocabulary;

        const int AddTab(const int &pCategoryId);
        const void EnableControls();
        const void InitTabs();

    private slots:
        const void on_qpbCategoryAdd_clicked(bool checked = false);
        const void on_qpbWordAdd_clicked(bool checked = false);
}; // VocabularyManagerDialog

#endif // VOCABULARYMANAGERDIALOG_H