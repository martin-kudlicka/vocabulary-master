#ifndef WORDCOPYMOVEDIALOG_H
#define WORDCOPYMOVEDIALOG_H

#include <ui_wordcopymovedialog.h>

#include "common/categoriesmodel.h"

class WordsCopyMoveDialog : public QDialog
{
	Q_OBJECT

	public:
        typedef QList<int> tRowNumList;

		WordsCopyMoveDialog(const int &pCategoryId, const tRowNumList &pRowNums, Vocabulary *pVocabulary, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		CategoriesModel _cmCategoriesModel;
        int _iOldCategoryId;
		Ui::qdWordCopyMove _qdwcmCopyMove;
		tRowNumList _trnlRowNums;
		Vocabulary *_vVocabulary;

		virtual void accept();

	private slots:
		const void on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // WordsCopyMoveDialog

#endif // WORDCOPYMOVEDIALOG_H