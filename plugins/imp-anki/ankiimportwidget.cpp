#include "pch.h"
#include "ankiimportwidget.h"

#include "../../common/marklineedit.h"
#include "../common/imp-interface.h"

AnkiImportWidget::AnkiImportWidget(const QSqlDatabase *database, QWidget *parent) : QWidget(parent), _decksModel(database), _fieldsModel(database), _modelsModel(database), _database(database)
{
  _ui.setupUi(this);

  // decks
  prepareTreeView(_ui.decks, &_decksModel);
  connect(_ui.decks->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AnkiImportWidget::on_decks_selectionModel_selectionChanged);
  // models
  prepareTreeView(_ui.models, &_modelsModel);
  connect(_ui.models->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AnkiImportWidget::on_models_selectionModel_selectionChanged);
  // fields
  prepareTreeView(_ui.fields, &_fieldsModel);
}

qlonglong AnkiImportWidget::fieldId(quintptr position) const
{
  return _fieldsModel.fieldId(position);
}

QStringList AnkiImportWidget::marks() const
{
  QStringList marks;
  for (auto rowIndex = 0; rowIndex < _fieldsModel.rowCount(); ++rowIndex)
  {
    const auto editorIndex = _fieldsModel.index(rowIndex, FieldsModel::Column::Mark);
    const auto markEditor  = qobject_cast<const MarkLineEdit *>(_ui.fields->indexWidget(editorIndex));
    marks.append(markEditor->text());
  }

  return marks;
}

void AnkiImportWidget::prepareTreeView(QTreeView *treeView, QAbstractItemModel *itemModel)
{
  treeView->setModel(itemModel);
  treeView->setItemDelegateForColumn(FieldsModel::Column::Mark, &_markEditDelegate);
  for (auto columnIndex = 0; columnIndex < treeView->header()->count(); ++columnIndex)
  {
    treeView->header()->setSectionResizeMode(columnIndex, QHeaderView::Stretch);
  }
}

void AnkiImportWidget::on_decks_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  const auto deckId = _decksModel.deckId(_ui.decks->currentIndex().row());
  _modelsModel.deckId(deckId);
  _ui.models->reset();
}

void AnkiImportWidget::on_models_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  const auto modelId = _modelsModel.modelId(_ui.models->currentIndex().row());
  _fieldsModel.setModelId(modelId);

  _ui.fields->reset();

  for (auto rowIndex = 0; rowIndex < _fieldsModel.rowCount(); ++rowIndex)
  {
    const auto modelIndex = _fieldsModel.index(rowIndex, FieldsModel::Column::Mark);
    _ui.fields->openPersistentEditor(modelIndex);
  }
}