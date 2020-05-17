#include "vocabularymanagerdialog.h"

#include "settings.h"
#include "vocabularymanagerdialog/vocabularyview.h"
#include <QtWidgets/QInputDialog>
#include "vocabulary.h"
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"
#include "../plugins/common/imp-interface.h"
#include "vocabularymanagerdialog/wordsimportdialog.h"
#include <QtWidgets/QFileDialog>
#include "common/vocabularyopenprogressdialog.h"
#include "vocabularymanagerdialog/wordsexportdialog.h"
#include "vocabularymanagerdialog/prioritydelegate.h"
#include "vocabularymanagerdialog/wordcopymovedialog.h"

const auto BUILTIN_COLUMN_SIZE = 30;
const auto PROPERTY_COLUMN     = "Column";

VocabularyManagerDialog::VocabularyManagerDialog(const QSharedPointer<Vocabulary> &vocabulary, const Settings *settings, const Plugins *plugins, QWidget *parent) : QDialog(parent, Qt::WindowMaximizeButtonHint), _vocabulary(vocabulary), _settings(settings), _plugins(plugins)
{
  _ui.setupUi(this);

  setWindowTitle(windowTitle() + " - " + vocabulary->name());

  initTabs();
  initEditor();
  selectFirstEnabledTab();

  enableTabControls();

  connect(_ui.tabs, &VocabularyTabWidget::tabEnableChanged,   this, &VocabularyManagerDialog::on_tabs_tabEnableChanged);
  connect(_ui.tabs, &VocabularyTabWidget::tabPriorityChanged, this, &VocabularyManagerDialog::on_tabs_tabPriorityChanged);

  vocabulary->beginEdit();
}

VocabularyManagerDialog::~VocabularyManagerDialog()
{
  _vocabulary->endEdit();
}

void VocabularyManagerDialog::execOnRecord(quintptr recordId)
{
  focusOnRecord(recordId);
  exec();
}

void VocabularyManagerDialog::addTab(quintptr categoryId)
{
  auto vocabularyView = new VocabularyView(_vocabulary, _ui.tabs);
  vocabularyView->setSelectionBehavior(QAbstractItemView::SelectRows);
  vocabularyView->setModel(new VocabularyModel(_vocabulary, categoryId, vocabularyView));
  hideColumns(vocabularyView);
  setPriorityDelegate(vocabularyView);
  vocabularyView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  connect(vocabularyView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &VocabularyManagerDialog::on_vocabularyViewSelectionModel_selectionChanged);

  stretchColumns(vocabularyView);

  auto tabs           = _ui.tabs;
  const auto tabIndex = tabs->addTab(vocabularyView, _vocabulary->categoryName(categoryId), _vocabulary->categoryEnabled(categoryId), _vocabulary->categoryPriority(categoryId));
  tabs->setTabEnabled(tabIndex, _vocabulary->categoryEnabled(categoryId));
}

void VocabularyManagerDialog::enableTabControls() const
{
  _ui.categoryRemove->setEnabled(_ui.tabs->currentWidget());
  _ui.wordAdd->setEnabled(_ui.tabs->currentWidget() && _ui.tabs->isTabEnabled(_ui.tabs->currentIndex()));

  _ui.wordImport->setEnabled(_ui.tabs->currentWidget());
  _ui.wordExport->setEnabled(_ui.tabs->currentWidget());
}

void VocabularyManagerDialog::enableWordControls() const
{
  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  const QItemSelectionModel *selection;
  if (vocabularyView)
  {
    selection = vocabularyView->selectionModel();
  }
  else
  {
    selection = Q_NULLPTR;
  }

  _ui.wordRemove->setEnabled(selection && vocabularyView->isEnabled() && selection->hasSelection());
  _ui.wordCopyMove->setEnabled(selection && vocabularyView->isEnabled() && selection->hasSelection());
}

void VocabularyManagerDialog::focusOnRecord(quintptr recordId) const
{
  // get found word category
  const auto category = _vocabulary->recordCategory(recordId);

  // get tab for category
  decltype(_ui.tabs->count()) tabIndex;
  for (tabIndex = 0; tabIndex < _ui.tabs->count(); ++tabIndex)
  {
    if (_categories.at(tabIndex) == category)
    {
      break;
    }
  }

  // switch tabs
  _ui.tabs->setCurrentIndex(tabIndex);

  // focus on word
        auto vocabularyView  = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
  const auto vocabularyModel = qobject_cast<const VocabularyModel *>(vocabularyView->model());
  vocabularyView->setCurrentIndex(vocabularyModel->index(_vocabulary->row(recordId, category), 0));
}

void VocabularyManagerDialog::hideColumns() const
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); ++tabIndex)
  {
    auto vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
    hideColumns(vocabularyView);
  }
}

void VocabularyManagerDialog::hideColumns(VocabularyView *tableView) const
{
  auto column = 0;
  for (auto fieldId : _vocabulary->fieldIds())
  {
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::Show))
    {
      tableView->showColumn(column);
    }
    else
    {
      tableView->hideColumn(column);
    }

    ++column;
  }
}

void VocabularyManagerDialog::initEditor()
{
  auto fieldsLeft  = 0;
  auto fieldsRight = 0;
  auto posLeft     = 0;
  auto posRight    = 0;

  for (auto fieldId : _vocabulary->fieldIds())
  {
    quintptr column, row;

    // update field count
    if (_vocabulary->fieldLanguage(fieldId) == VocabularyDatabase::FieldLanguage::Left)
    {
      ++fieldsLeft;
    }
    else
    {
      ++fieldsRight;
    }

    // check if visible or builtin field
    const auto attributes = _vocabulary->fieldAttributes(fieldId);
    if (!(attributes & VocabularyDatabase::FieldAttribute::Show) || attributes & VocabularyDatabase::FieldAttribute::BuiltIn)
    {
      continue;
    }

    // get field language
    if (_vocabulary->fieldLanguage(fieldId) == VocabularyDatabase::FieldLanguage::Left)
    {
      row = posLeft++;
      column = static_cast<quintptr>(EditorColumn::LeftLabel);
    }
    else
    {
      row = posRight++;
      column = static_cast<quintptr>(EditorColumn::RightLabel);
    }

    // label
    auto label = new QLabel(_vocabulary->fieldName(fieldId) + ':', _ui.editorGroup);
    _ui.editorLayout->addWidget(label, row, column);

    // control
    auto control = new QLineEdit(_ui.editorGroup);
    control->setProperty(PROPERTY_COLUMN, fieldsLeft + fieldsRight - 1);
    connect(control, &QLineEdit::textEdited, this, &VocabularyManagerDialog::on_control_textEdited);
    _ui.editorLayout->addWidget(control, row, column + 1);
  }
}

void VocabularyManagerDialog::initTabs()
{
  auto tabs = _ui.tabs;
  tabs->setShowEnabled(_settings->canEnableCategories());
  tabs->setShowPriorities(_settings->canChangeCategoryPriority());

  const auto categories = _vocabulary->categoryIds();
  VocabularyDatabase::CategoryIdList::const_iterator categoryId;
  for (categoryId = categories.constBegin(); categoryId != categories.constEnd(); ++categoryId)
  {
    addTab(*categoryId);
    _categories.append(*categoryId);
  }
}

void VocabularyManagerDialog::reassignModels() const
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); ++tabIndex)
  {
    auto vocabularyView  = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
    auto vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
    vocabularyView->setModel(Q_NULLPTR);
    vocabularyView->setModel(vocabularyModel);
    connect(vocabularyView->selectionModel(), &QItemSelectionModel::selectionChanged,  this, &VocabularyManagerDialog::on_vocabularyViewSelectionModel_selectionChanged);
  }
}

void VocabularyManagerDialog::selectFirstEnabledTab()
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); ++tabIndex)
  {
    if (_ui.tabs->isTabEnabled(tabIndex))
    {
      _ui.tabs->setCurrentIndex(tabIndex);
      return;
    }
  }
}

void VocabularyManagerDialog::setPriorityDelegate()
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); ++tabIndex)
  {
    auto vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
    setPriorityDelegate(vocabularyView);
  }
}

void VocabularyManagerDialog::setPriorityDelegate(VocabularyView *tableView)
{
  for (auto column = 0; column < tableView->horizontalHeader()->count(); ++column)
  {
    const auto fieldId = _vocabulary->fieldId(column);
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      const auto builtIn = _vocabulary->fieldBuiltIn(fieldId);
      if (builtIn == VocabularyDatabase::FieldBuiltIn::Priority)
      {
        auto priorityDelegate = new PriorityDelegate(tableView);
        tableView->setItemDelegateForColumn(column, priorityDelegate);
      }
      else
      {
        tableView->setItemDelegateForColumn(column, Q_NULLPTR);
      }
    }
    else
    {
      tableView->setItemDelegateForColumn(column, Q_NULLPTR);
    }
  }
}

void VocabularyManagerDialog::stretchColumns() const
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); ++tabIndex)
  {
    const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->widget(tabIndex));
    stretchColumns(vocabularyView);
  }
}

void VocabularyManagerDialog::stretchColumns(const VocabularyView *tableView) const
{
  for (auto column = 0; column < tableView->horizontalHeader()->count(); ++column)
  {
    const auto fieldId = _vocabulary->fieldId(column);
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Fixed);
      tableView->horizontalHeader()->resizeSection(column, BUILTIN_COLUMN_SIZE);
    }
    else
    {
      tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
    }
  }
}

void VocabularyManagerDialog::uninitEditor() const
{
  QLayoutItem *item;
  while ((item = _ui.editorLayout->takeAt(0)) != Q_NULLPTR)
  {
    item->widget()->deleteLater();
  }
}

void VocabularyManagerDialog::updateEditor() const
{
  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  const QItemSelectionModel *selection;
  if (vocabularyView)
  {
    selection = vocabularyView->selectionModel();
  }
  else
  {
    selection = Q_NULLPTR;
  }
  _ui.editorGroup->setEnabled(selection && selection->hasSelection());

  updateEditor(EditorColumn::LeftControl);
  updateEditor(EditorColumn::RightControl);
}

void VocabularyManagerDialog::updateEditor(EditorColumn controlsColumn) const
{
  for (auto row = 0; row < _ui.editorLayout->rowCount(); ++row)
  {
    auto item = _ui.editorLayout->itemAtPosition(row, static_cast<int>(controlsColumn));
    if (item)
    {
      auto widget  = item->widget();
      auto control = qobject_cast<QLineEdit *>(widget);

      const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
      const QItemSelectionModel *selection;
      if (vocabularyView)
      {
        selection = vocabularyView->selectionModel();
      }
      else
      {
        selection = Q_NULLPTR;
      }

      if (selection && selection->hasSelection())
      {
        const auto tabIndex = _ui.tabs->currentIndex();
        const auto index    = vocabularyView->currentIndex();
        auto column         = control->property(PROPERTY_COLUMN).toUInt();
        control->setText(_vocabulary->dataText(_categories.at(tabIndex), index.row(), _vocabulary->fieldId(column)));
      }
      else
      {
        control->clear();
      }
    }
  }
}

void VocabularyManagerDialog::on_control_textEdited(const QString &text) const
{
  const auto column = sender()->property(PROPERTY_COLUMN).toUInt();

  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  const auto current        = vocabularyView->currentIndex();

  auto vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
  vocabularyModel->setData(vocabularyModel->index(current.row(), column), text);
}

void VocabularyManagerDialog::on_categoryAdd_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  const auto category = QInputDialog::getText(this, tr("Add category"), tr("New category name"));
  if (!category.isEmpty())
  {
    const auto categoryId = _vocabulary->addCategory(category);
    addTab(categoryId);
    _categories.append(categoryId);

    _ui.tabs->setCurrentIndex(_ui.tabs->count() - 1);
    enableTabControls();
  }
}

void VocabularyManagerDialog::on_categoryRemove_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  const auto tabIndex = _ui.tabs->currentIndex();
  _ui.tabs->removeTab(tabIndex);
  _vocabulary->removeCategory(_categories.takeAt(tabIndex));

  enableTabControls();
}

void VocabularyManagerDialog::on_searchButton_clicked(bool checked /* false */) const
{
  Q_UNUSED(checked);

  // get current word id
  const auto vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
  const auto selection      = vocabularyView->selectionModel();
  const auto currentRecord  = _vocabulary->recordId(_categories.at(_ui.tabs->currentIndex()), selection->currentIndex().row());

  // search for next word
  auto recordId = _vocabulary->search(_ui.searchEdit->text(), currentRecord + 1);
  if (recordId == VocabularyDatabase::NOT_FOUND)
  {
    return;
  }

  focusOnRecord(recordId);
}

void VocabularyManagerDialog::on_searchEdit_textChanged(const QString &text) const
{
  _ui.searchButton->setEnabled(!text.isEmpty());
}

void VocabularyManagerDialog::on_tabs_currentChanged(int index) const
{
  Q_UNUSED(index);

  updateEditor();
  enableWordControls();
}

void VocabularyManagerDialog::on_tabs_tabEnableChanged(quintptr index, Qt::CheckState state) const
{
  _vocabulary->setCategoryEnabled(_categories.at(index), state);
  _ui.tabs->setTabEnabled(index, state);

  enableTabControls();
  enableWordControls();
}

void VocabularyManagerDialog::on_tabs_tabPriorityChanged(quintptr index, quintptr value) const
{
  _vocabulary->setCategoryPriority(_categories.at(index), value);
}

void VocabularyManagerDialog::on_vocabularySettings_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  VocabularySettingsDialog vocabularySettings(_vocabulary, _plugins, this);

  const auto oldColumnCount = _vocabulary->fieldCount();

  _vocabulary->endEdit();
  _vocabulary->beginEdit();

  if (vocabularySettings.exec() == QDialog::Accepted)
  {
    if (oldColumnCount != _vocabulary->fieldCount())
    {
      reassignModels();
    }
    setPriorityDelegate();
    stretchColumns();
    uninitEditor();
    initEditor();
    updateEditor();
    hideColumns();
  }
  else
  {
    _vocabulary->endEdit(false);
    _vocabulary->beginEdit();
  }
}

void VocabularyManagerDialog::on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  const auto currentTabView = qobject_cast<const VocabularyView *>(_ui.tabs->widget(_ui.tabs->currentIndex()));
  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  if (currentTabView != vocabularyView)
  {
    // tab switching in progress
    return;
  }

  updateEditor();
  enableWordControls();
}

void VocabularyManagerDialog::on_wordAdd_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  auto vocabularyView  = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
  auto vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
  vocabularyModel->addRow();

  vocabularyView->setCurrentIndex(vocabularyModel->index(vocabularyModel->rowCount() - 1, 0));
  vocabularyView->setFocus(Qt::OtherFocusReason);

  if (!_ui.editorLayout->isEmpty())
  {
    auto control = _ui.editorLayout->itemAtPosition(0, static_cast<int>(EditorColumn::LeftControl));
    control->widget()->setFocus(Qt::OtherFocusReason);
  }
}

void VocabularyManagerDialog::on_wordCopyMove_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  // get selected records
  WordsCopyMoveDialog::RowNumList rowNums;
  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  const auto selection      = vocabularyView->selectionModel();
  for (const auto &qmiIndex : selection->selectedRows())
  {
    rowNums.append(qmiIndex.row());
  }

  // copy/move dialog
  WordsCopyMoveDialog copyMoveDialog(_categories.at(_ui.tabs->currentIndex()), rowNums, _vocabulary, this);
  if (copyMoveDialog.exec() == QDialog::Accepted)
  {
    reassignModels();
    stretchColumns();
    hideColumns();
  }
}

void VocabularyManagerDialog::on_wordExport_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  WordsExportDialog wedExport(_vocabulary, _plugins->explugins(), this);
  wedExport.exec();
}

void VocabularyManagerDialog::on_wordImport_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  // prepare filter
  QStringList filters;
  for (const auto &plugin : _plugins->impPlugins())
  {
    const auto pluginInterface = plugin.impInterface;
    filters.append(pluginInterface->filter());
  }

  QString filterText;
  const auto filePath = QFileDialog::getOpenFileName(this, tr("Import words"), QString(), filters.join(";;"), &filterText);
  if (!filePath.isEmpty())
  {
    const auto filterIndex = filters.indexOf(filterText);
    auto plugin            = _plugins->impPlugins().at(filterIndex).impInterface;
    WordsImportDialog importDialog(filePath, _vocabulary, plugin, this);

    _vocabulary->endEdit();
    _vocabulary->beginEdit();

    if (importDialog.exec() == QDialog::Accepted)
    {
      _vocabulary->endEdit();

      VocabularyOpenProgressDialog openProgress(_vocabulary.data(), this);
      openProgress.show();
      _vocabulary->close();
      _vocabulary->open(_vocabulary->vocabularyFile());
      openProgress.hide();

      _vocabulary->beginEdit();
      reassignModels();
      stretchColumns();
      hideColumns();
    }
    else
    {
      _vocabulary->endEdit(false);
      _vocabulary->beginEdit();
    }
  }
}

void VocabularyManagerDialog::on_wordRemove_clicked(bool checked /* false */)
{
  Q_UNUSED(checked);

  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  auto vocabularyModel      = qobject_cast<VocabularyModel *>(vocabularyView->model());
  const auto selection      = vocabularyView->selectionModel();
  vocabularyModel->removeRow(selection->currentIndex().row());
}