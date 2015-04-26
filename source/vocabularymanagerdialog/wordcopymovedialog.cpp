#include "vocabularymanagerdialog/wordcopymovedialog.h"

WordsCopyMoveDialog::WordsCopyMoveDialog(quint8 categoryId, const RowNumList &rowNums, Vocabulary *vocabulary, QWidget *parent /* nullptr */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _categoriesModel(vocabulary), _oldCategoryId(categoryId), _rowNums(rowNums), _vocabulary(vocabulary)
{
	_ui.setupUi(this);

	// categories
	_ui.categories->setModel(&_categoriesModel);
	connect(_ui.categories->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_categoriesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // WordsCopyMoveDialog

WordsCopyMoveDialog::~WordsCopyMoveDialog()
{
} // ~WordsCopyMoveDialog

void WordsCopyMoveDialog::accept()
{
    const QModelIndex index    = _ui.categories->currentIndex();
    const quint8 newCategoryId = _vocabulary->categoryId(index.row());

    if (_ui.copy->isChecked())
	{
        for (RowNumList::const_iterator row = _rowNums.constBegin(); row != _rowNums.constEnd(); row++)
		{
            const QStringList data = _vocabulary->record(*row);
            _vocabulary->addRecord(newCategoryId, data);
        } // for
    }
	else
	{
        for (RowNumList::const_iterator row = _rowNums.constEnd(); row != _rowNums.constBegin();)
		{
            row--;
            _vocabulary->setRecordByRowCategory(_oldCategoryId, *row, newCategoryId);
        } // for
    } // if else

    QDialog::accept();
} // accept

void WordsCopyMoveDialog::on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	_ui.okButton->setEnabled(_ui.categories->currentIndex() != QModelIndex());
} // on_vvVocabularyViewSelectionModel_selectionChanged