#include "ankiimportwidget.h"

#include "../../common/marklineedit.h"
#include "../common/imp-interface.h"

AnkiImportWidget::AnkiImportWidget(const QSqlDatabase *database, QWidget *parent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QWidget(parent, pFlags), _decksModel(database), _fieldsModel(database), _modelsModel(database), _database(database)
{
    _ui.setupUi(this);

    // decks
    prepareTreeView(_ui.decks, &_decksModel);
    connect(_ui.decks->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_decks_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    // models
    prepareTreeView(_ui.models, &_modelsModel);
    connect(_ui.models->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_models_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    // fields
    prepareTreeView(_ui.fields, &_fieldsModel);
} // AnkiImportWidget

AnkiImportWidget::~AnkiImportWidget()
{
} // ~AnkiImportWidget

const qlonglong AnkiImportWidget::fieldId(const quint8 &position) const
{
	return _fieldsModel.fieldId(position);
} // fieldId

const QStringList AnkiImportWidget::marks() const
{
	QStringList marks;
	for (quint8 rowIndex = 0; rowIndex < _fieldsModel.rowCount(); rowIndex++)
	{
		const QModelIndex editorIndex = _fieldsModel.index(rowIndex, FieldsModel::ColumnMark);
		const MarkLineEdit *markEditor = qobject_cast<const MarkLineEdit *>(_ui.fields->indexWidget(editorIndex));
		marks.append(markEditor->text());
	} // for

	return marks;
} // marks

const void AnkiImportWidget::prepareTreeView(QTreeView *treeView, QAbstractItemModel *itemModel)
{
	treeView->setModel(itemModel);
	treeView->setItemDelegateForColumn(FieldsModel::ColumnMark, &_markEditDelegate);
	for (quint8 columnIndex = 0; columnIndex < treeView->header()->count(); columnIndex++)
	{
		treeView->header()->setSectionResizeMode(columnIndex, QHeaderView::Stretch);
	} // for
} // prepareTreeView

const void AnkiImportWidget::on_decks_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const quint8 deckId = _decksModel.deckId(_ui.decks->currentIndex().row());
    _modelsModel.deckId(deckId);
    _ui.models->reset();
} // on_decks_selectionModel_selectionChanged

const void AnkiImportWidget::on_models_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const qlonglong modelId = _modelsModel.modelId(_ui.models->currentIndex().row());
    _fieldsModel.setModelId(modelId);

    _ui.fields->reset();

    for (quint8 rowIndex = 0; rowIndex < _fieldsModel.rowCount(); rowIndex++)
	{
        const QModelIndex modelIndex = _fieldsModel.index(rowIndex, FieldsModel::ColumnMark);
        _ui.fields->openPersistentEditor(modelIndex);
    } // for
} // on_models_selectionModel_selectionChanged