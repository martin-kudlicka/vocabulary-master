#ifndef WORDCOPYMOVEDIALOG_H
#define WORDCOPYMOVEDIALOG_H

#include <ui_wordcopymovedialog.h>

#include "common/categoriesmodel.h"

class WordsCopyMoveDialog : public QDialog
{
	Q_OBJECT

	public:
		WordsCopyMoveDialog(const Vocabulary *pVocabulary, QWidget *pParent = NULL, Qt::WindowFlags pFlags = 0);

	private:
		CategoriesModel _cmCategoriesModel;
		Ui::qdWordCopyMove _qdwcmCopyMove;
		const Vocabulary *_vVocabulary;

	private slots:
		const void on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // WordsCopyMoveDialog

#endif // WORDCOPYMOVEDIALOG_H