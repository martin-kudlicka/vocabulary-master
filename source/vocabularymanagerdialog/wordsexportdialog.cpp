#include "vocabularymanagerdialog/wordsexportdialog.h"

#include "../../common/marklineedit.h"

WordsExportDialog::WordsExportDialog(const Vocabulary *vocabulary, const Plugins::ExpPluginList &expPlugins, QWidget *parent /* nullptr */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _categoriesModel(vocabulary), _expPluginsModel(&expPlugins), _expPlugins(expPlugins), _vocabulary(vocabulary), _fieldsModel(vocabulary)
{
	_ui.setupUi(this);

	// export plugins
	_ui.expPlugins->setModel(&_expPluginsModel);
	connect(_ui.expPlugins->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_expPluginsSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));

	// categories
	_ui.categories->setModel(&_categoriesModel);
	// fields
	_ui.fields->setModel(&_fieldsModel);
    _ui.fields->setItemDelegateForColumn(WordsExportFieldsModel::ColumnMark, &_markDelegate);
    for (quint8 row = 0; row < _fieldsModel.rowCount(); row++)
	{
        const QModelIndex index = _fieldsModel.index(row, WordsExportFieldsModel::ColumnMark);
        _ui.fields->openPersistentEditor(index);
    }
	for (quint8 column = 0; column < _ui.fields->header()->count(); column++)
	{
		_ui.fields->header()->setSectionResizeMode(column, QHeaderView::Stretch);
	}

	_ui.categories->selectAll();
}

WordsExportDialog::~WordsExportDialog()
{
}

const void WordsExportDialog::on_exportButton_clicked(bool checked /* false */)
{
    const QModelIndex index    = _ui.expPlugins->currentIndex();
    const ExpInterface *plugin = _expPlugins.at(index.row()).expInterface;
    plugin->beginExport();
}

const void WordsExportDialog::on_expPluginsSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	const QModelIndex index = _ui.expPlugins->currentIndex();
	if (!_expPluginPage.contains(index.row()))
	{
		// create page for new new plugin
		QWidget *pluginWidget = new QWidget(_ui.expPluginsStack);
		QBoxLayout *layout    = new QBoxLayout(QBoxLayout::LeftToRight, pluginWidget);
		layout->setContentsMargins(QMargins());

		// insert page to table
		const quint8 page = _ui.expPluginsStack->addWidget(pluginWidget);
		_expPluginPage.insert(index.row(), page);

		ExpInterface *plugin = _expPlugins.at(index.row()).expInterface;

        // connections
    connect(plugin, SIGNAL(ProgressExportSetMax(quintptr)), SLOT(on_plugin_ProgressExportSetMax(quintptr)));
    connect(plugin, SIGNAL(ProgressExportSetValue(quintptr)), SLOT(on_plugin_ProgressExportSetValue(quintptr)));
    connect(plugin, SIGNAL(VocabularyGetCategoryIds(ExpInterface::CategoryIdList *)), SLOT(on_plugin_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *)));
    connect(plugin, SIGNAL(VocabularyGetCategoryName(quintptr, QString *)), SLOT(on_plugin_VocabularyGetCategoryName(quintptr, QString *)));
    connect(plugin, SIGNAL(VocabularyGetMarks(QStringList *)), SLOT(on_plugin_VocabularyGetMarks(QStringList *)));
    connect(plugin, SIGNAL(VocabularyGetMarkText(quintptr, const QString &, QString *)), SLOT(on_plugin_VocabularyGetMarkText(quintptr, const QString &, QString *)));
    connect(plugin, SIGNAL(VocabularyGetRecordCount(quintptr, quintptr *)), SLOT(on_plugin_VocabularyGetRecordCount(quintptr, quintptr *)));
    connect(plugin, SIGNAL(VocabularyGetRecordIds(quintptr, ExpInterface::RecordIdList *)), SLOT(on_plugin_VocabularyGetRecordIds(quintptr, ExpInterface::RecordIdList *)));

		// setup page
		plugin->setupUi(pluginWidget);
	}

    // set plugin page
    _ui.expPluginsStack->setCurrentIndex(_expPluginPage.value(index.row()));

	_ui.exportButton->setEnabled(true);
}

const void WordsExportDialog::on_plugin_ProgressExportSetMax(quintptr max) const
{
    _ui.progress->setMaximum(max);
}

const void WordsExportDialog::on_plugin_ProgressExportSetValue(quintptr value) const
{
    _ui.progress->setValue(value);
}

const void WordsExportDialog::on_plugin_VocabularyGetCategoryIds(ExpInterface::CategoryIdList *categoryIds) const
{
    const QItemSelectionModel *selection = _ui.categories->selectionModel();
    const QModelIndexList selected       = selection->selectedRows();

    for (const QModelIndex &index : selected)
	{
        const quint8 categoryId = _vocabulary->categoryId(index.row());
        categoryIds->append(categoryId);
    }
}

const void WordsExportDialog::on_plugin_VocabularyGetCategoryName(quintptr categoryId, QString *name) const
{
    *name = _vocabulary->categoryName(categoryId);
}

const void WordsExportDialog::on_plugin_VocabularyGetMarks(QStringList *pMarks) const
{
    for (quint8 row = 0; row < _fieldsModel.rowCount(); row++)
	{
        const QModelIndex editorIndex = _fieldsModel.index(row, WordsExportFieldsModel::ColumnMark);
        const MarkLineEdit *editor    = qobject_cast<const MarkLineEdit *>(_ui.fields->indexWidget(editorIndex));
        pMarks->append(editor->text());
    }
}

const void WordsExportDialog::on_plugin_VocabularyGetMarkText(quintptr recordId, const QString &mark, QString *text) const
{
    QStringList marks;
    on_plugin_VocabularyGetMarks(&marks);
    const quint8 markIndex = marks.indexOf(mark);
    const quint8 fieldId   = _vocabulary->fieldId(markIndex);

    *text = _vocabulary->dataText(recordId, fieldId);
}

const void WordsExportDialog::on_plugin_VocabularyGetRecordCount(quintptr categoryId, quintptr *count) const
{
    *count = _vocabulary->recordCount(categoryId, _ui.exportEnabledOnly->isChecked());
}

const void WordsExportDialog::on_plugin_VocabularyGetRecordIds(quintptr categoryId, ExpInterface::RecordIdList *recordIds) const
{
    *recordIds = _vocabulary->recordIds(categoryId);
}