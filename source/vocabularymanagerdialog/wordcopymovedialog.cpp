#include "vocabularymanagerdialog/wordcopymovedialog.h"

void WordsCopyMoveDialog::accept()
{
    QModelIndex qmiIndex = _qdwcmCopyMove.qtvCategories->currentIndex();
    int iNewCategoryId = _vVocabulary->GetCategoryId(qmiIndex.row());

    if (_qdwcmCopyMove.qrbCopy->isChecked()) {
        for (tRowNumList::const_iterator ciRow = _trnlRowNums.constBegin(); ciRow != _trnlRowNums.constEnd(); ciRow++) {
            QStringList qslData = _vVocabulary->GetRecord(*ciRow);
            _vVocabulary->AddRecord(iNewCategoryId, qslData);
        } // for
    } else {
        for (tRowNumList::const_iterator ciRow = _trnlRowNums.constEnd(); ciRow != _trnlRowNums.constBegin();) {
            ciRow--;
            _vVocabulary->SetRecordByRowCategory(_iOldCategoryId, *ciRow, iNewCategoryId);
        } // for
    } // if else

    QDialog::accept();
} // accept

const void WordsCopyMoveDialog::on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	_qdwcmCopyMove.qpbOk->setEnabled(_qdwcmCopyMove.qtvCategories->currentIndex() != QModelIndex());
} // on_vvVocabularyViewSelectionModel_selectionChanged

WordsCopyMoveDialog::WordsCopyMoveDialog(const int &pCategoryId, const tRowNumList &pRowNums, Vocabulary *pVocabulary, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _cmCategoriesModel(pVocabulary)
{
    _iOldCategoryId = pCategoryId;
	_trnlRowNums = pRowNums;
	_vVocabulary = pVocabulary;

	_qdwcmCopyMove.setupUi(this);

	// categories
	_qdwcmCopyMove.qtvCategories->setModel(&_cmCategoriesModel);
	connect(_qdwcmCopyMove.qtvCategories->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvCategoriesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // WordsCopyMoveDialog