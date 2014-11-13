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

const char *PROPERTY_COLUMN = "Column";

VocabularyManagerDialog::VocabularyManagerDialog(Vocabulary *vocabulary,
#ifndef EDITION_FREE
    const Settings *settings,
    const Plugins *plugins,
#endif
	QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags | Qt::WindowMaximizeButtonHint), _vocabulary(vocabulary)
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
	connect(_ui.tabs, SIGNAL(TabEnableChanged(quint8, Qt::CheckState)), SLOT(on_tabs_TabEnableChanged(quint8, Qt::CheckState)));
	connect(_ui.tabs, SIGNAL(TabPriorityChanged(quint8, quint8)), SLOT(on_tabs_TabPriorityChanged(quint8, quint8)));
#endif

    vocabulary->beginEdit();
} // VocabularyManagerDialog

VocabularyManagerDialog::~VocabularyManagerDialog()
{
    _vocabulary->endEdit();
} // ~VocabularyManagerDialog

#ifndef EDITION_FREE
void VocabularyManagerDialog::execOnRecord(quint32 recordId)
{
    focusOnRecord(recordId);
    exec();
} // execOnRecord
#endif

void VocabularyManagerDialog::addTab(quint8 categoryId)
{
    VocabularyView *vocabularyView = new VocabularyView(
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

    VocabularyTabWidget *tabs = _ui.tabs;
    const quint8 tabIndex = tabs->addTab(vocabularyView, _vocabulary->categoryName(categoryId)
#ifndef EDITION_FREE
        , _vocabulary->categoryEnabled(categoryId), _vocabulary->categoryPriority(categoryId)
#endif
        );
#ifndef EDITION_FREE
    tabs->setTabEnabled(tabIndex, _vocabulary->categoryEnabled(categoryId));
#endif
} // addTab

void VocabularyManagerDialog::enableTabControls() const
{
	_ui.categoryRemove->setEnabled(_ui.tabs->currentWidget());
	_ui.wordAdd->setEnabled(_ui.tabs->currentWidget() && _ui.tabs->isTabEnabled(_ui.tabs->currentIndex()));

#ifndef EDITION_FREE
    _ui.wordImport->setEnabled(_ui.tabs->currentWidget());
    _ui.wordExport->setEnabled(_ui.tabs->currentWidget());
#endif
} // enableTabControls

void VocabularyManagerDialog::enableWordControls() const
{
	const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
	const QItemSelectionModel *selection;
	if (vocabularyView)
	{
		selection = vocabularyView->selectionModel();
	}
	else
	{
		selection = NULL;
	} // if else

	_ui.wordRemove->setEnabled(selection && vocabularyView->isEnabled() && selection->hasSelection());
#ifndef EDITION_FREE
	_ui.wordCopyMove->setEnabled(selection && vocabularyView->isEnabled() && selection->hasSelection());
#endif
} // enableWordControls

void VocabularyManagerDialog::focusOnRecord(quint32 recordId) const
{
    // get found word category
    const quint8 category = _vocabulary->recordCategory(recordId);

    // get tab for category
    quint8 tabIndex;
    for (tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
	{
        if (_categories.at(tabIndex) == category)
		{
            break;
        } // if
    } // for

    // switch tabs
    _ui.tabs->setCurrentIndex(tabIndex);

    // focus on word
    VocabularyView *vocabularyView         = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
    const VocabularyModel *vocabularyModel = qobject_cast<const VocabularyModel *>(vocabularyView->model());
    vocabularyView->setCurrentIndex(vocabularyModel->index(_vocabulary->row(recordId, category), 0));
} // focusOnRecord

#ifndef EDITION_FREE
void VocabularyManagerDialog::hideColumns() const
{
	for (quint8 tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
	{
		VocabularyView *vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
		hideColumns(vocabularyView);
	} // for
} // hideColumns
#endif

void VocabularyManagerDialog::hideColumns(VocabularyView *tableView) const
{
	quint8 column = 0;
	foreach (quint8 fieldId, _vocabulary->fieldIds())
	{
#ifdef EDITION_FREE
		if (!_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
#else
		if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeShow))
#endif
		{
			tableView->showColumn(column);
		}
		else
		{
			tableView->hideColumn(column);
		} // if else

		column++;
	} // foreach
} // hideColumns

void VocabularyManagerDialog::initEditor()
{
	quint8 fieldsLeft  = 0;
	quint8 fieldsRight = 0;
    quint8 posLeft     = 0;
	quint8 posRight    = 0;

	foreach (quint8 fieldId, _vocabulary->fieldIds())
	{
		quint8 column, row;

        // update field count
        if (_vocabulary->fieldLanguage(fieldId) == VocabularyDatabase::FieldLanguageLeft)
		{
            fieldsLeft++;
        }
		else
		{
            fieldsRight++;
        } // if else

		// check if visible or builtin field
		const VocabularyDatabase::FieldAttributes attributes = _vocabulary->fieldAttributes(fieldId);
        if (
#ifndef EDITION_FREE
			!(attributes & VocabularyDatabase::FieldAttributeShow) ||
#endif
			attributes & VocabularyDatabase::FieldAttributeBuiltIn)
		{
			continue;
		} // if

        // get field language
        if (_vocabulary->fieldLanguage(fieldId) == VocabularyDatabase::FieldLanguageLeft)
		{
            row    = posLeft++;
            column = EditorColumnLeftLabel;
        }
		else
		{
            row    = posRight++;
            column = EditorColumnRightLabel;
        } // if else

		// label
		QLabel *label = new QLabel(_vocabulary->fieldName(fieldId) + ':', _ui.editorGroup);
		_ui.editorLayout->addWidget(label, row, column);

		// control
		QLineEdit *control = new QLineEdit(_ui.editorGroup);
		control->setProperty(PROPERTY_COLUMN, fieldsLeft + fieldsRight - 1);
		connect(control, SIGNAL(textEdited(const QString &)), SLOT(on_control_textEdited(const QString &)));
		_ui.editorLayout->addWidget(control, row, column + 1);
	} // foreach
} // initEditor

void VocabularyManagerDialog::initTabs()
{
    VocabularyTabWidget *tabs = _ui.tabs;
#ifndef EDITION_FREE
    tabs->setShowEnabled(_settings->canEnableCategories());
	tabs->setShowPriorities(_settings->canChangeCategoryPriority());
#endif

    const VocabularyDatabase::CategoryIdList categories = _vocabulary->categoryIds();
    VocabularyDatabase::CategoryIdList::const_iterator categoryId;
    for (categoryId = categories.constBegin(); categoryId != categories.constEnd(); categoryId++)
	{
        addTab(*categoryId);
        _categories.append(*categoryId);
    } // for
} // initTabs

#ifndef EDITION_FREE
void VocabularyManagerDialog::reassignModels() const
{
    for (quint8 tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
	{
        VocabularyView *vocabularyView   = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
        VocabularyModel *vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
        vocabularyView->setModel(NULL);
        vocabularyView->setModel(vocabularyModel);
        connect(vocabularyView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    } // for
} // reassignModels

void VocabularyManagerDialog::selectFirstEnabledTab()
{
    for (quint8 tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
	{
        if (_ui.tabs->isTabEnabled(tabIndex))
		{
            _ui.tabs->setCurrentIndex(tabIndex);
            return;
        } // if
    } // for
} // selectFirstEnabledTab

void VocabularyManagerDialog::setPriorityDelegate()
{
	for (quint8 tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
	{
		VocabularyView *vocabularyView = qobject_cast<VocabularyView *>(_ui.tabs->widget(tabIndex));
		setPriorityDelegate(vocabularyView);
	} // for
} // setPriorityDelegate

void VocabularyManagerDialog::setPriorityDelegate(VocabularyView *tableView)
{
	for (quint8 column = 0; column < tableView->horizontalHeader()->count(); column++)
	{
		const quint8 fieldId = _vocabulary->fieldId(column);
		if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
			if (builtIn == VocabularyDatabase::FieldBuiltInPriority)
			{
				PriorityDelegate *priorityDelegate = new PriorityDelegate(tableView);
				tableView->setItemDelegateForColumn(column, priorityDelegate);
            }
			else
			{
                tableView->setItemDelegateForColumn(column, NULL);
            } // if else
        }
		else
		{
            tableView->setItemDelegateForColumn(column, NULL);
        } // if else
	} // for
} // setPriorityDelegate

void VocabularyManagerDialog::stretchColumns() const
{
    for (quint8 tabIndex = 0; tabIndex < _ui.tabs->count(); tabIndex++)
	{
        const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->widget(tabIndex));
        stretchColumns(vocabularyView);
    } // for
} // stretchColumns
#endif

void VocabularyManagerDialog::stretchColumns(const VocabularyView *tableView) const
{
	for (quint8 column = 0; column < tableView->horizontalHeader()->count(); column++)
	{
#ifndef EDITION_FREE
		const quint8 fieldId = _vocabulary->fieldId(column);
		if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Fixed);
            tableView->horizontalHeader()->resizeSection(column, BUILTIN_COLUMN_SIZE);
		}
		else
		{
#endif
			tableView->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
#ifndef EDITION_FREE
		} // if else
#endif
	} // for
} // stretchColumns

#ifndef EDITION_FREE
void VocabularyManagerDialog::uninitEditor() const
{
    QLayoutItem *item;
    while ((item = _ui.editorLayout->takeAt(0)) != NULL)
	{
        item->widget()->deleteLater();
    } // while
} // uninitEditor
#endif

void VocabularyManagerDialog::updateEditor() const
{
	const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
	const QItemSelectionModel *selection;
	if (vocabularyView)
	{
		selection = vocabularyView->selectionModel();
	}
	else
	{
		selection = NULL;
	} // if else
	_ui.editorGroup->setEnabled(selection && selection->hasSelection());

	updateEditor(EditorColumnLeftControl);
	updateEditor(EditorColumnRightControl);
} // updateEditor

void VocabularyManagerDialog::updateEditor(EditorColumn controlsColumn) const
{
	for (quint8 row = 0; row <  _ui.editorLayout->rowCount(); row++)
	{
		QLayoutItem *item = _ui.editorLayout->itemAtPosition(row, controlsColumn);
		if (item)
		{
			QWidget *widget = item->widget();
			QLineEdit *control = qobject_cast<QLineEdit *>(widget);

			const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
			const QItemSelectionModel *selection;
			if (vocabularyView)
			{
				selection = vocabularyView->selectionModel();
			}
			else
			{
				selection = NULL;
			} // if else

			if (selection && selection->hasSelection())
			{
				const quint8 tabIndex   = _ui.tabs->currentIndex();
				const QModelIndex index = vocabularyView->currentIndex();
				quint8 column           = control->property(PROPERTY_COLUMN).toUInt();
				control->setText(_vocabulary->dataText(_categories.at(tabIndex), index.row(), _vocabulary->fieldId(column)));
			}
			else
			{
				control->clear();
			} // if else
		} // if
	} // for
} // updateEditor

void VocabularyManagerDialog::on_control_textEdited(const QString &text) const
{
	const quint8 column = sender()->property(PROPERTY_COLUMN).toUInt();

	const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
	const QModelIndex current            = vocabularyView->currentIndex();

	VocabularyModel *vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
	vocabularyModel->setData(vocabularyModel->index(current.row(), column), text);
} // on_control_textEdited

void VocabularyManagerDialog::on_categoryAdd_clicked(bool checked /* false */)
{
    const QString category = QInputDialog::getText(this, tr("Add category"), tr("New category name"));
    if (!category.isEmpty())
	{
        const quint8 categoryId = _vocabulary->addCategory(category);
		addTab(categoryId);
        _categories.append(categoryId);

		_ui.tabs->setCurrentIndex(_ui.tabs->count() - 1);
		enableTabControls();
    } // if
} // on_categoryAdd_clicked

void VocabularyManagerDialog::on_categoryRemove_clicked(bool checked /* false */)
{
    const quint8 tabIndex = _ui.tabs->currentIndex();
    _ui.tabs->removeTab(tabIndex);
    _vocabulary->removeCategory(_categories.takeAt(tabIndex));

	enableTabControls();
} // on_categoryRemove_clicked

void VocabularyManagerDialog::on_searchButton_clicked(bool checked /* false */) const
{
    // get current word id
    const VocabularyView *vocabularyView     = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
    const VocabularyModel *vocabularyModel   = qobject_cast<const VocabularyModel *>(vocabularyView->model());
    const QItemSelectionModel *selection     = vocabularyView->selectionModel();
    const quint32 currentRecord              = _vocabulary->recordId(_categories.at(_ui.tabs->currentIndex()), selection->currentIndex().row());

    // search for next word
    quint32 recordId = _vocabulary->search(_ui.searchEdit->text(), currentRecord + 1);
    if (recordId == VocabularyDatabase::NOT_FOUND)
	{
        return;
    } // if

    focusOnRecord(recordId);
} // on_searchButton_clicked

void VocabularyManagerDialog::on_searchEdit_textChanged(const QString &text) const
{
    _ui.searchButton->setEnabled(!text.isEmpty());
} // on_searchEdit_textChanged

void VocabularyManagerDialog::on_tabs_currentChanged(int index) const
{
	updateEditor();
	enableWordControls();
} // on_tabs_currentChanged

#ifndef EDITION_FREE
void VocabularyManagerDialog::on_tabs_TabEnableChanged(quint8 index, Qt::CheckState state) const
{
    _vocabulary->setCategoryEnabled(_categories.at(index), state);
    _ui.tabs->setTabEnabled(index, state);

	enableTabControls();
	enableWordControls();
} // on_tabs_TabEnableChanged

void VocabularyManagerDialog::on_tabs_TabPriorityChanged(quint8 index, quint8 value) const
{
	_vocabulary->setCategoryPriority(_categories.at(index), value);
} // on_tabs_TabPriorityChanged
#endif

void VocabularyManagerDialog::on_vocabularySettings_clicked(bool checked /* false */)
{
	VocabularySettingsDialog vocabularySettings(_vocabulary,
#ifndef EDITION_FREE
        _plugins,
#endif
        this);

#ifndef EDITION_FREE
	const quint8 oldColumnCount = _vocabulary->fieldCount();
#endif

    _vocabulary->endEdit();
    _vocabulary->beginEdit();

    if (vocabularySettings.exec() == QDialog::Accepted)
	{
#ifndef EDITION_FREE
		if (oldColumnCount != _vocabulary->fieldCount())
		{
			reassignModels();
		} // if
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
    } // if else
} // on_vocabularySettings_clicked

void VocabularyManagerDialog::on_vocabularyViewSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
	const VocabularyView *currentTabView = qobject_cast<const VocabularyView *>(_ui.tabs->widget(_ui.tabs->currentIndex()));
	const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
	if (currentTabView != vocabularyView)
	{
		// tab switching in progress
		return;
	} // if

    updateEditor();
    enableWordControls();
} // on_vocabularyViewSelectionModel_selectionChanged

void VocabularyManagerDialog::on_wordAdd_clicked(bool checked /* false */)
{
	VocabularyView *vocabularyView   = qobject_cast<VocabularyView *>(_ui.tabs->currentWidget());
    VocabularyModel *vocabularyModel = qobject_cast<VocabularyModel *>(vocabularyView->model());
    vocabularyModel->addRow();

	vocabularyView->setCurrentIndex(vocabularyModel->index(vocabularyModel->rowCount() - 1, 0));
	vocabularyView->setFocus(Qt::OtherFocusReason);

    if (!_ui.editorLayout->isEmpty())
	{
        QLayoutItem *control = _ui.editorLayout->itemAtPosition(0, EditorColumnLeftControl);
        control->widget()->setFocus(Qt::OtherFocusReason);
    } // if
} // on_wordAdd_clicked

#ifndef EDITION_FREE
void VocabularyManagerDialog::on_wordCopyMove_clicked(bool checked /* false */)
{
	// get selected records
	WordsCopyMoveDialog::tRowNumList rowNums;
	const VocabularyView *vocabularyView   = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
	const VocabularyModel *vocabularyModel = qobject_cast<const VocabularyModel *>(vocabularyView->model());
	const QItemSelectionModel *selection   = vocabularyView->selectionModel();
	foreach (const QModelIndex &qmiIndex, selection->selectedRows())
	{
		rowNums.append(qmiIndex.row());
	} // foreach

	// copy/move dialog
	WordsCopyMoveDialog copyMoveDialog(_categories.at(_ui.tabs->currentIndex()), rowNums, _vocabulary, this);
    if (copyMoveDialog.exec() == QDialog::Accepted)
	{
        reassignModels();
        stretchColumns();
        hideColumns();
    } // if
} // on_wordCopyMove_clicked

void VocabularyManagerDialog::on_wordExport_clicked(bool checked /* false */)
{
	WordsExportDialog wedExport(_vocabulary, _plugins->explugins(), this);
	wedExport.exec();
} // on_wordExport_clicked

void VocabularyManagerDialog::on_wordImport_clicked(bool checked /* false */)
{
	// prepare filter
	QStringList filters;
    foreach (const Plugins::ImpPlugin &plugin, _plugins->impPlugins())
	{
        const ImpInterface *pluginInterface = plugin.impInterface;
		filters.append(pluginInterface->filter());
	} // foreach

	QString filterText;
	const QString filePath = QFileDialog::getOpenFileName(this, tr("Import words"), QString(), filters.join(";;"), &filterText);
	if (!filePath.isEmpty())
	{
        const quint8 filterIndex = filters.indexOf(filterText);
        ImpInterface *plugin     = _plugins->impPlugins().at(filterIndex).impInterface;
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
		} // if else
	} // if
} // on_wordImport_clicked
#endif

void VocabularyManagerDialog::on_wordRemove_clicked(bool checked /* false */)
{
    const VocabularyView *vocabularyView = qobject_cast<const VocabularyView *>(_ui.tabs->currentWidget());
    VocabularyModel *vocabularyModel     = qobject_cast<VocabularyModel *>(vocabularyView->model());
    const QItemSelectionModel *selection = vocabularyView->selectionModel();
    vocabularyModel->removeRow(selection->currentIndex().row());
} // on_wordRemove_clicked