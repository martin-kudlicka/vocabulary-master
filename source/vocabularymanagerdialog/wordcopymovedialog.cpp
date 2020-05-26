#include "pch.h"
#include "vocabularymanagerdialog/wordcopymovedialog.h"

#include "vocabulary.h"

WordsCopyMoveDialog::WordsCopyMoveDialog(quintptr categoryId, const RowNumList &rowNums, const QSharedPointer<Vocabulary> &vocabulary, QWidget *parent) : QDialog(parent), _categoriesModel(vocabulary), _oldCategoryId(categoryId), _rowNums(rowNums), _vocabulary(vocabulary)
{
  _ui.setupUi(this);

  // categories
  _ui.categories->setModel(&_categoriesModel);
  connect(_ui.categories->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WordsCopyMoveDialog::on_categoriesSelectionModel_selectionChanged);
}

void WordsCopyMoveDialog::accept()
{
  const auto index         = _ui.categories->currentIndex();
  const auto newCategoryId = _vocabulary->categoryId(index.row());

  if (_ui.copy->isChecked())
  {
    for (auto row = _rowNums.constBegin(); row != _rowNums.constEnd(); ++row)
    {
      const auto data = _vocabulary->record(*row);
      _vocabulary->addRecord(newCategoryId, data);
    }
  }
  else
  {
    for (auto row = _rowNums.constEnd(); row != _rowNums.constBegin();)
    {
      row--;
      _vocabulary->setRecordByRowCategory(_oldCategoryId, *row, newCategoryId);
    }
  }

  QDialog::accept();
}

void WordsCopyMoveDialog::on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  _ui.okButton->setEnabled(_ui.categories->currentIndex().isValid());
}