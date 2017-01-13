#ifndef WORDCOPYMOVEDIALOG_H
#define WORDCOPYMOVEDIALOG_H

#include <ui_wordcopymovedialog.h>

#include "common/categoriesmodel.h"

class WordsCopyMoveDialog : public QDialog
{
  Q_OBJECT

  public:
    using RowNumList = QList<quintptr>;

             WordsCopyMoveDialog(quintptr categoryId, const RowNumList &rowNums, Vocabulary *vocabulary, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    virtual ~WordsCopyMoveDialog() Q_DECL_OVERRIDE;

  private:
    CategoriesModel        _categoriesModel;
    quintptr               _oldCategoryId;
    Ui::WordCopyMoveDialog _ui;
    RowNumList             _rowNums;
    Vocabulary            *_vocabulary;

    virtual void accept() Q_DECL_OVERRIDE;

  private Q_SLOTS:
    void on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
};

#endif