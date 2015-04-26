#ifndef WORDCOPYMOVEDIALOG_H
#define WORDCOPYMOVEDIALOG_H

#include <ui_wordcopymovedialog.h>

#include "common/categoriesmodel.h"

class WordsCopyMoveDialog : public QDialog
{
	Q_OBJECT

	public:
        typedef QList<quint32> RowNumList;

		         WordsCopyMoveDialog(quint8 categoryId, const RowNumList &rowNums, Vocabulary *vocabulary, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
		virtual ~WordsCopyMoveDialog() override;

	private:
		CategoriesModel        _categoriesModel;
        quint8                 _oldCategoryId;
		Ui::WordCopyMoveDialog _ui;
		RowNumList             _rowNums;
		Vocabulary            *_vocabulary;

		virtual void accept() override;

	private slots:
		void on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
}; // WordsCopyMoveDialog

#endif // WORDCOPYMOVEDIALOG_H