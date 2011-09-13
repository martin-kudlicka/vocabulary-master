#include "vocabularymanagerdialog/wordcopymovedialog.h"

const void WordsCopyMoveDialog::on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	_qdwcmCopyMove.qpbOk->setEnabled(_qdwcmCopyMove.qtvCategories->currentIndex() != QModelIndex());
} // on_vvVocabularyViewSelectionModel_selectionChanged

WordsCopyMoveDialog::WordsCopyMoveDialog(const Vocabulary *pVocabulary, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary)
{
	_vVocabulary = pVocabulary;

	_qdwcmCopyMove.setupUi(this);

	// categories
	_qdwcmCopyMove.qtvCategories->setModel(&_cmCategoriesModel);
	connect(_qdwcmCopyMove.qtvCategories->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // WordsCopyMoveDialog