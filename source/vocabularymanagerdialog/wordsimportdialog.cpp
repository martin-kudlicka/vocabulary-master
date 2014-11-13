#include "vocabularymanagerdialog/wordsimportdialog.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>

WordsImportDialog::WordsImportDialog(const QString &file, Vocabulary *vocabulary, ImpInterface *plugin, QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _importing(false), _interrupt(false), _categoriesModel(vocabulary), _plugin(plugin), _file(file), _editorDelegate(vocabulary), _vocabulary(vocabulary), _fieldsModel(vocabulary)
{
} // WordsImportDialog

WordsImportDialog::~WordsImportDialog()
{
    _plugin->close();
} // ~WordsImportDialog

int WordsImportDialog::exec()
{
    if (!_plugin->open(_file))
	{
        QMessageBox::critical(QApplication::activeWindow(), tr("Words import"), tr("Can't import data from selected file."));
        return QDialog::Rejected;
    } // if

    // vocabulary UI
    _ui.setupUi(this);
    // categories
    _ui.categories->setModel(&_categoriesModel);
    connect(_ui.categories->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_categoriesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
    // fields
    _ui.fields->setModel(&_fieldsModel);
	createFieldEditors();
    _ui.fields->header()->setSectionResizeMode(WordsImportFieldsModel::ColumnName, QHeaderView::ResizeToContents);
	_ui.fields->header()->setSectionResizeMode(WordsImportFieldsModel::ColumnLanguage, QHeaderView::ResizeToContents);
    _ui.fields->header()->setSectionResizeMode(WordsImportFieldsModel::ColumnEditor, QHeaderView::Stretch);
	// preview
	preparePreviewColumns();

    // plugin UI
    new QBoxLayout(QBoxLayout::LeftToRight, _ui.sourceGroup);
    _plugin->setupUI(_ui.sourceGroup);

	return QDialog::exec();
} // exec

void WordsImportDialog::accept()
{
    importData(TargetVocabulary);
    if (!_interrupt)
	{
        QDialog::accept();
    } // if
} // accept

void WordsImportDialog::createFieldEditors()
{
	_ui.fields->setItemDelegateForColumn(WordsImportFieldsModel::ColumnEditor, &_editorDelegate);

    for (quint8 row = 0; row < _fieldsModel.rowCount(); row++)
	{
		bool persistentEditor = true;

		const quint8 fieldId = _vocabulary->fieldId(row);
		if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
		{
			const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
			if (builtIn == VocabularyDatabase::FieldBuiltInEnabled)
			{
				persistentEditor = false;
			} // if
		} // if

		if (persistentEditor)
		{
			const QModelIndex index = _fieldsModel.index(row, WordsImportFieldsModel::ColumnEditor);
			_ui.fields->openPersistentEditor(index);
		} // if
    } // for
} // createFieldEditors

void WordsImportDialog::enableControls() const
{
    const QItemSelectionModel *categorySelection = _ui.categories->selectionModel();

    _ui.okButton->setEnabled(!_importing && categorySelection->hasSelection());
    if (_importing)
	{
	    _ui.cancelButton->setText(tr("Stop"));
    }
	else
	{
        _ui.cancelButton->setText(tr("Cancel"));
    } // if else
} // enableControls

void WordsImportDialog::importData(const Target &target)
{
    _importing = true;
	enableControls();
    _interrupt = false;

	// setup progessbar
	_ui.progress->setMaximum(_plugin->recordCount());

	// patterns
	QStringList patterns;
	for (quint8 pattern = 0; pattern < _vocabulary->fieldCount(); pattern++)
	{
		const QModelIndex index = _fieldsModel.index(pattern, WordsImportFieldsModel::ColumnEditor);
		const QString data      = _fieldsModel.data(index, Qt::EditRole).toString();
		patterns.append(data);
	} // for

	const QStringList marks   = _plugin->marks();
	const quint32 recordCount = _plugin->recordCount();

	quint8 categoryId;
	switch (target)
	{
		case TargetPreview:
			_ui.preview->setRowCount(recordCount);
			break;
		case TargetVocabulary:
			const QModelIndex category = _ui.categories->currentIndex();
			categoryId = _vocabulary->categoryId(category.row());
	} // switch

	quint32 skipCount = 0;
	for (quint32 record = 0; record < recordCount && !_interrupt; record++)
	{
		// get mark data
		bool skip = false;
		QStringList markData;
		foreach (const QString &mark, marks)
		{
			const QString data = _plugin->recordData(record, mark);
			if (_ui.skipPartialRecords->isChecked() && data.isEmpty())
			{
				skip = true;
				skipCount++;
				break;
			} // if
			markData.append(data);
		} // foreach
		if (skip)
		{
			continue;
		} // if

        // get data
        QStringList data;
		for (quint8 column = 0; column < _vocabulary->fieldCount(); column++)
		{
			QString text = patterns.at(column);

			for (quint8 mark = 0; mark < marks.size(); mark++)
			{
				text.replace(marks.at(mark), markData.at(mark));
			} // for
            data.append(text);
        } // for

        // insert data into target
        switch (target)
		{
            case TargetPreview:
                for (quint8 column = 0; column < _vocabulary->fieldCount(); column++)
				{
					QTableWidgetItem *tableItem;

					const quint8 fieldId = _vocabulary->fieldId(column);
					switch (_vocabulary->fieldType(fieldId))
					{
						case VocabularyDatabase::FieldTypeLineEdit:
							tableItem = new QTableWidgetItem(data.at(column));
							break;
						case VocabularyDatabase::FieldTypeCheckBox:
							tableItem = new QTableWidgetItem();
							tableItem->setCheckState(static_cast<Qt::CheckState>(data.at(column).toUInt()));
							tableItem->setFlags(tableItem->flags() | Qt::ItemIsUserCheckable);
							break;
						case VocabularyDatabase::FieldTypeSpinBox:
							tableItem = new QTableWidgetItem(data.at(column));
					} // switch

					tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);
					_ui.preview->setItem(record - skipCount, column, tableItem);
				} // for
				break;
			case TargetVocabulary:
				_vocabulary->addRecord(categoryId, data);
		} // switch

		// progress
        if (record % IMPORT_REFRESHINTERVAL == 0)
		{
		    _ui.progress->setValue(record);
		    QCoreApplication::processEvents();
        } // if
	} // for

	if (target == TargetPreview && skipCount > 0)
	{
		_ui.preview->setRowCount(recordCount - skipCount);
	} // if

	// setup progressbar
	_ui.progress->setValue(0);
	_ui.progress->setMaximum(1);

    _importing = false;
    enableControls();
} // importData

void WordsImportDialog::preparePreviewColumns() const
{
	QStringList columns;
	foreach (quint8 fieldId, _vocabulary->fieldIds())
	{
		columns.append(_vocabulary->fieldName(fieldId));
	} // foreach

	_ui.preview->setColumnCount(columns.size());
	_ui.preview->setHorizontalHeaderLabels(columns);
	for (quint8 column = 0; column < _ui.preview->horizontalHeader()->count(); column++)
	{
		_ui.preview->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
	} // for
} // preparePreviewColumns

void WordsImportDialog::reject()
{
    if (_importing)
	{
        _interrupt = true;
    }
	else
	{
        QDialog::reject();
    } // if else
} // reject

void WordsImportDialog::on_previewRefresh_clicked(bool checked /* false */)
{
    importData(TargetPreview);
} // on_previewRefresh_clicked

void WordsImportDialog::on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
    enableControls();
} // on_categoriesSelectionModel_selectionChanged