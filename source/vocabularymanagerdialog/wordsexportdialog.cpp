#include "vocabularymanagerdialog/wordsexportdialog.h"

#include "vocabulary.h"
#include "../../common/marklineedit.h"

WordsExportDialog::WordsExportDialog(const QSharedPointer<Vocabulary> &vocabulary, const Plugins::ExpPluginList &expPlugins, QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _categoriesModel(vocabulary), _expPluginsModel(&expPlugins), _expPlugins(expPlugins), _vocabulary(vocabulary), _fieldsModel(vocabulary)
{
  _ui.setupUi(this);

  // export plugins
  _ui.expPlugins->setModel(&_expPluginsModel);
  connect(_ui.expPlugins->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WordsExportDialog::on_expPluginsSelectionModel_selectionChanged);

  // categories
  _ui.categories->setModel(&_categoriesModel);
  // fields
  _ui.fields->setModel(&_fieldsModel);
  _ui.fields->setItemDelegateForColumn(static_cast<int>(WordsExportFieldsModel::Column::Mark), &_markDelegate);
  for (auto row = 0; row < _fieldsModel.rowCount(); row++)
  {
    const auto index = _fieldsModel.index(row, static_cast<int>(WordsExportFieldsModel::Column::Mark));
    _ui.fields->openPersistentEditor(index);
  }
  for (auto column = 0; column < _ui.fields->header()->count(); column++)
  {
    _ui.fields->header()->setSectionResizeMode(column, QHeaderView::Stretch);
  }

  _ui.categories->selectAll();
}

WordsExportDialog::~WordsExportDialog()
{
}

void WordsExportDialog::on_exportButton_clicked(bool checked /* false */)
{
  const auto index  = _ui.expPlugins->currentIndex();
  const auto plugin = _expPlugins.at(index.row()).expInterface;
  plugin->beginExport();
}

void WordsExportDialog::on_expPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  const auto index = _ui.expPlugins->currentIndex();
  if (!_expPluginPage.contains(index.row()))
  {
    // create page for new new plugin
    auto pluginWidget = new QWidget(_ui.expPluginsStack);
    auto layout       = new QBoxLayout(QBoxLayout::LeftToRight, pluginWidget);
    layout->setContentsMargins(QMargins());

    // insert page to table
    const auto page = _ui.expPluginsStack->addWidget(pluginWidget);
    _expPluginPage.insert(index.row(), page);

    auto plugin = _expPlugins.at(index.row()).expInterface;

    // connections
    connect(plugin, &ExpInterface::progressExportSetMax,      this, &WordsExportDialog::on_plugin_progressExportSetMax);
    connect(plugin, &ExpInterface::progressExportSetValue,    this, &WordsExportDialog::on_plugin_progressExportSetValue);
    connect(plugin, &ExpInterface::vocabularyGetCategoryIds,  this, &WordsExportDialog::on_plugin_vocabularyGetCategoryIds);
    connect(plugin, &ExpInterface::vocabularyGetCategoryName, this, &WordsExportDialog::on_plugin_vocabularyGetCategoryName);
    connect(plugin, &ExpInterface::vocabularyGetMarks,        this, &WordsExportDialog::on_plugin_vocabularyGetMarks);
    connect(plugin, &ExpInterface::vocabularyGetMarkText,     this, &WordsExportDialog::on_plugin_vocabularyGetMarkText);
    connect(plugin, &ExpInterface::vocabularyGetRecordCount,  this, &WordsExportDialog::on_plugin_vocabularyGetRecordCount);
    connect(plugin, &ExpInterface::vocabularyGetRecordIds,    this, &WordsExportDialog::on_plugin_vocabularyGetRecordIds);

    // setup page
    plugin->setupUi(pluginWidget);
  }

  // set plugin page
  _ui.expPluginsStack->setCurrentIndex(_expPluginPage.value(index.row()));

  _ui.exportButton->setEnabled(true);
}

void WordsExportDialog::on_plugin_progressExportSetMax(quintptr max) const
{
  _ui.progress->setMaximum(max);
}

void WordsExportDialog::on_plugin_progressExportSetValue(quintptr value) const
{
  _ui.progress->setValue(value);
}

void WordsExportDialog::on_plugin_vocabularyGetCategoryIds(ExpInterface::CategoryIdList *categoryIds) const
{
  const auto selection = _ui.categories->selectionModel();
  const auto selected  = selection->selectedRows();

  for (const auto &index : selected)
  {
    const auto categoryId = _vocabulary->categoryId(index.row());
    categoryIds->append(categoryId);
  }
}

void WordsExportDialog::on_plugin_vocabularyGetCategoryName(quintptr categoryId, QString *name) const
{
  *name = _vocabulary->categoryName(categoryId);
}

void WordsExportDialog::on_plugin_vocabularyGetMarks(QStringList *pMarks) const
{
  for (auto row = 0; row < _fieldsModel.rowCount(); row++)
  {
    const auto editorIndex = _fieldsModel.index(row, static_cast<int>(WordsExportFieldsModel::Column::Mark));
    const auto editor      = qobject_cast<const MarkLineEdit *>(_ui.fields->indexWidget(editorIndex));
    pMarks->append(editor->text());
  }
}

void WordsExportDialog::on_plugin_vocabularyGetMarkText(quintptr recordId, const QString &mark, QString *text) const
{
  QStringList marks;
  on_plugin_vocabularyGetMarks(&marks);
  const auto markIndex = marks.indexOf(mark);
  const auto fieldId   = _vocabulary->fieldId(markIndex);

  *text = _vocabulary->dataText(recordId, fieldId);
}

void WordsExportDialog::on_plugin_vocabularyGetRecordCount(quintptr categoryId, quintptr *count) const
{
  *count = _vocabulary->recordCount(categoryId, _ui.exportEnabledOnly->isChecked());
}

void WordsExportDialog::on_plugin_vocabularyGetRecordIds(quintptr categoryId, ExpInterface::RecordIdList *recordIds) const
{
  *recordIds = _vocabulary->recordIds(categoryId);
}