#include "vocabularymanagerdialog.h"

#include <QtWidgets/QInputDialog>
#include "vocabularymanagerdialog/vocabularymodel.h"
#include "vocabularymanagerdialog/vocabularysettingsdialog.h"
#ifndef EDITION_FREE
# include "vocabularymanagerdialog/wordsimportdialog.h"
#endif
#include <QtWidgets/QFileDialog>
#ifndef EDITION_FREE
# include "common/vocabularyopenprogressdialog.h"
# include "vocabularymanagerdialog/wordsexportdialog.h"
# include "vocabularymanagerdialog/prioritydelegate.h"
# include "vocabularymanagerdialog/wordcopymovedialog.h"
#endif

#ifndef EDITION_FREE
const auto BUILTIN_COLUMN_SIZE = 30;
#endif
const char *PROPERTY_COLUMN = "Column";

VocabularyManagerDialog::VocabularyManagerDialog(Vocabulary *vocabulary,
#ifndef EDITION_FREE
                                                 const Settings *settings, const Plugins *plugins,
#endif
                                                 QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags | Qt::WindowMaximizeButtonHint), _vocabulary(vocabulary)
#ifndef EDITION_FREE
                                                 , _settings(settings), _plugins(plugins)
#endif
{
  _ui.setupUi(this);
#ifdef EDITION_FREE
  delete _ui.wordCopyMove;
  delete _ui.wordImport;
  delete _ui.wordExport;
#endif

  setWindowTitle(windowTitle() + " - " + vocabulary->name());

  initTabs();
  initEditor();
#ifndef EDITION_FREE
  selectFirstEnabledTab();
#endif

  enableTabControls();

#ifndef EDITION_FREE
  connect(_ui.tabs, SIGNAL(tabEnableChanged(quintptr, Qt::CheckState)), SLOT(on_tabs_tabEnableChanged(quintptr, Qt::CheckState)));
  connect(_ui.tabs, SIGNAL(tabPriorityChanged(quintptr, quintptr)),     SLOT(on_tabs_tabPriorityChanged(quintptr, quintptr)));
#endif

  vocabulary->beginEdit();
}

VocabularyManagerDialog::~VocabularyManagerDialog()
{
  _vocabulary->endEdit();
}

#ifndef EDITION_FREE
void VocabularyManagerDialog::execOnRecord(quintptr recordId)
{
  focusOnRecord(recordId);
  exec();
}
#endif

void VocabularyManagerDialog::addTab(quintptr categoryId)
{
  auto vocabularyView = new VocabularyView(
#ifndef EDITION_FREE
                                                      _vocabulary,
#endif
                                                      _ui.tabs);
  vocabularyView->setSelectionBehavior(QAbstractItemView::SelectRows);
  vocabularyView->setModel(new VocabularyModel(_vocabulary, categoryId, vocabularyView));
  hideColumns(vocabularyView);
#ifndef EDITION_FREE
  setPriorityDelegate(vocabularyView);
  vocabularyView->setEditTriggers(QAbstractItemView::AllEditTriggers);
#endif
  connect(vocabularyView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

  stretchColumns(vocabularyView);

        auto tabs     = _ui.tabs;
  const auto tabIndex = tabs->addTab(vocabularyView, _vocabulary->categoryName(categoryId)
#ifndef EDITION_FREE
                                     , _vocabulary->categoryEnabled(categoryId), _vocabulary->categoryPriority(categoryId)
#endif
                                     );
#ifndef EDITION_FREE
  tabs->setTabEnabled(tabIndex, _vocabulary->categoryEnabled(categoryId));
#endif
}

void VocabularyManagerDialog::enableTabControls() const
{
  _ui.categoryRemove->setEnabled(_ui.tabs->currentWidget());
  _ui.wordAdd->setEnabled(_ui.tabs->currentWidget() && _ui.tabs->isTabEnabled(_ui.tabs->currentIndex()));

#ifndef EDITION_FREE
  _ui.wordImport->setEnabled(_ui.tabs->currentWidget());
  _ui.wordExport->setEnabled(_ui.tabs->currentWidget());
#endif
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
#ifndef EDITION_FREE
  _ui.wordCopyMove->setEnabled(selection && vocabularyView->isEnabled() && selection->hasSelection());
#endif
}

void VocabularyManagerDialog::focusOnRecord(quintptr recordId) const
{
  // get found word category
  const auto category = _vocabulary->recordCategory(recordId);

  // get tab for category
  quintptr tabIndex;
  for (tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
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

#ifndef EDITION_FREE
void VocabularyManagerDialog::hideColumns() const
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
  {
    auto vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
    hideColumns(vocabularyView);
  }
}
#endif

void VocabularyManagerDialog::hideColumns(VocabularyView *tableView) const
{
  auto column = 0;
  for (auto fieldId : _vocabulary->fieldIds())
  {
#ifdef EDITION_FREE
    if (!_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
#else
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::Show))
#endif
    {
      tableView->showColumn(column);
    }
    else
    {
      tableView->hideColumn(column);
    }

    column++;
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
      fieldsLeft++;
    }
    else
    {
      fieldsRight++;
    }

    // check if visible or builtin field
    const auto attributes = _vocabulary->fieldAttributes(fieldId);
    if (
#ifndef EDITION_FREE
        !(attributes & VocabularyDatabase::FieldAttribute::Show) ||
#endif
        attributes & VocabularyDatabase::FieldAttribute::BuiltIn)
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
    connect(control, SIGNAL(textEdited(const QString &)), SLOT(on_control_textEdited(const QString &)));
    _ui.editorLayout->addWidget(control, row, column + 1);
  }
}

void VocabularyManagerDialog::initTabs()
{
  auto tabs = _ui.tabs;
#ifndef EDITION_FREE
  tabs->setShowEnabled(_settings->canEnableCategories());
  tabs->setShowPriorities(_settings->canChangeCategoryPriority());
#endif

  const auto categories = _vocabulary->categoryIds();
  VocabularyDatabase::CategoryIdList::const_iterator categoryId;
  for (categoryId = categories.constBegin(); categoryId != categories.constEnd(); categoryId++)
  {
    addTab(*categoryId);
    _categories.append(*categoryId);
  }
}

#ifndef EDITION_FREE
void VocabularyManagerDialog::reassignModels() const
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
  {
    auto vocabularyView  = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
    auto vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
    vocabularyView->setModel(Q_NULLPTR);
    vocabularyView->setModel(vocabularyModel);
    connect(vocabularyView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
  }
}

void VocabularyManagerDialog::selectFirstEnabledTab()
{
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
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
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
  {
    auto vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
    setPriorityDelegate(vocabularyView);
  }
}

void VocabularyManagerDialog::setPriorityDelegate(VocabularyView *tableView)
{
  for (auto column = 0; column < tableView->horizontalHeader()->count(); column++)
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
  for (auto tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
  {
    const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->widget(tabIndex));
    stretchColumns(vocabularyView);
  }
}
#endif

void VocabularyManagerDialog::stretchColumns(const VocabularyView *tableView) const
{
  for (auto column = 0; column < tableView->horizontalHeader()->count(); column++)
  {
#ifndef EDITION_FREE
    const auto fieldId = _vocabulary->fieldId(column);
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Fixed);
      tableView->horizontalHeader()->resizeSection(column, BUILTIN_COLUMN_SIZE);
    }
    else
    {
#endif
      tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
#ifndef EDITION_FREE
    }
#endif
  }
}

#ifndef EDITION_FREE
void VocabularyManagerDialog::uninitEditor() const
{
  QLayoutItem *item;
  while ((item = _ui.editorLayout->takeAt(0)) != Q_NULLPTR)
  {
    item->widget()->deleteLater();
  }
}
#endif

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
  for (auto row = 0; row < _ui.editorLayout->rowCount(); row++)
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
  const auto tabIndex = _ui.tabs->currentIndex();
  _ui.tabs->removeTab(tabIndex);
  _vocabulary->removeCategory(_categories.takeAt(tabIndex));

  enableTabControls();
}

void VocabularyManagerDialog::on_searchButton_clicked(bool checked /* false */) const
{
  // get current word id
  const auto vocabularyView  = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
  const auto vocabularyModel = qobject_cast<const VocabularyModel *>(vocabularyView->model());
  const auto selection       = vocabularyView->selectionModel();
  const auto currentRecord   = _vocabulary->recordId(_categories.at(_ui.tabs->currentIndex()), selection->currentIndex().row());

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
  updateEditor();
  enableWordControls();
}

#ifndef EDITION_FREE
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
#endif

void VocabularyManagerDialog::on_vocabularySettings_clicked(bool checked /* false */)
{
  VocabularySettingsDialog vocabularySettings(_vocabulary,
#ifndef EDITION_FREE
                                              _plugins,
#endif
                                              this);

#ifndef EDITION_FREE
  const auto oldColumnCount = _vocabulary->fieldCount();
#endif

  _vocabulary->endEdit();
  _vocabulary->beginEdit();

  if (vocabularySettings.exec() == QDialog::Accepted)
  {
#ifndef EDITION_FREE
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
#endif
  }
  else
  {
    _vocabulary->endEdit(false);
    _vocabulary->beginEdit();
  }
}

void VocabularyManagerDialog::on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
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

#ifndef EDITION_FREE
void VocabularyManagerDialog::on_wordCopyMove_clicked(bool checked /* false */)
{
  // get selected records
  WordsCopyMoveDialog::RowNumList rowNums;
  const auto vocabularyView  = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  const auto vocabularyModel = qobject_cast<const VocabularyModel *>(vocabularyView->model());
  const auto selection       = vocabularyView->selectionModel();
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
  WordsExportDialog wedExport(_vocabulary, _plugins->explugins(), this);
  wedExport.exec();
}

void VocabularyManagerDialog::on_wordImport_clicked(bool checked /* false */)
{
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

      VocabularyOpenProgressDialog openProgress(_vocabulary, this);
      openProgress.show();
      _vocabulary->close();
# ifdef EDITION_TRY
      _vocabulary->openMemory();
# else
      _vocabulary->open(_vocabulary->vocabularyFile());
# endif
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
#endif

void VocabularyManagerDialog::on_wordRemove_clicked(bool checked /* false */)
{
  const auto vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
  auto vocabularyModel      = qobject_cast<VocabularyModel *>(vocabularyView->model());
  const auto selection      = vocabularyView->selectionModel();
  vocabularyModel->removeRow(selection->currentIndex().row());
}