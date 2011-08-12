#include "ankiimportwidget.h"

#include "ankiimportwidget/marklineedit.h"
#include "../common/imp-interface.h"

AnkiImportWidget::AnkiImportWidget(const QSqlDatabase *pAnki, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(pParent, pFlags), _dmDecksModel(pAnki), _fmFieldsModel(pAnki), _mmModelsModel(pAnki)
{
    _qsdAnki = pAnki;

    _qwaiAnkiImport.setupUi(this);

    // decks
    PrepareTreeView(_qwaiAnkiImport.qtvDecks, &_dmDecksModel);
    connect(_qwaiAnkiImport.qtvDecks->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvDecksSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    // models
    PrepareTreeView(_qwaiAnkiImport.qtvModels, &_mmModelsModel);
    connect(_qwaiAnkiImport.qtvModels->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_qtvModelsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    // fields
    PrepareTreeView(_qwaiAnkiImport.qtvFields, &_fmFieldsModel);
} // AnkiImportWidget

const qlonglong AnkiImportWidget::GetFieldId(const int &pPosition) const
{
	return _fmFieldsModel.GetFieldId(pPosition);
} // GetFieldId

const QStringList AnkiImportWidget::GetMarks() const
{
	QStringList qslMarks;
	for (int iI = 0; iI < _fmFieldsModel.rowCount(); iI++) {
		QModelIndex qmiEditorIndex = _fmFieldsModel.index(iI, FieldsModel::ColumnMark);
		const MarkLineEdit *mleEditor = qobject_cast<const MarkLineEdit *>(_qwaiAnkiImport.qtvFields->indexWidget(qmiEditorIndex));
		qslMarks.append(mleEditor->text());
	} // for

	return qslMarks;
} // GetMarks

const void AnkiImportWidget::on_qtvDecksSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    int iDeckId = _dmDecksModel.GetDeckId(_qwaiAnkiImport.qtvDecks->currentIndex().row());
    _mmModelsModel.SetDeckId(iDeckId);
    _qwaiAnkiImport.qtvModels->reset();
} // on_qtvDecksSelectionModel_selectionChanged

const void AnkiImportWidget::on_qtvModelsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    qlonglong qllModelId = _mmModelsModel.GetModelId(_qwaiAnkiImport.qtvModels->currentIndex().row());
    _fmFieldsModel.SetModelId(qllModelId);

    _qwaiAnkiImport.qtvFields->reset();

    // assign editors
    for (int iI = 0; iI < _fmFieldsModel.rowCount(); iI++) {
        QModelIndex qmiNameIndex = _fmFieldsModel.index(iI, FieldsModel::ColumnName);
        QString qsName = _fmFieldsModel.data(qmiNameIndex).toString();

        QModelIndex qmiEditorIndex = _fmFieldsModel.index(iI, FieldsModel::ColumnMark);
        MarkLineEdit *mleEditor = new MarkLineEdit(TEMPLATE_MARK.arg(qsName), _qwaiAnkiImport.qtvFields);
        _qwaiAnkiImport.qtvFields->setIndexWidget(qmiEditorIndex, mleEditor);
    } // for
} // on_qtvModelsSelectionModel_selectionChanged

const void AnkiImportWidget::PrepareTreeView(QTreeView *pTreeView, QAbstractItemModel *pItemModel) const
{
    pTreeView->setModel(pItemModel);
    for (int iColumn = 0; iColumn < pTreeView->header()->count(); iColumn++) {
        pTreeView->header()->setResizeMode(iColumn, QHeaderView::Stretch);
    } // for
} // PrepareTreeView