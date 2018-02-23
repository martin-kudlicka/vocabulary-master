#include "vocabularymanagerdialog/wordsimportdialog.h"

#include "vocabulary.h"
#include "../plugins/common/imp-interface.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>

const auto IMPORT_REFRESHINTERVAL = 20;

WordsImportDialog::WordsImportDialog(const QString &file, const QSharedPointer<Vocabulary> &vocabulary, ImpInterface *plugin, QWidget *parent) : QDialog(parent), _importing(false), _interrupt(false), _categoriesModel(vocabulary), _plugin(plugin), _file(file), _editorDelegate(vocabulary), _vocabulary(vocabulary), _fieldsModel(vocabulary)
{
}

WordsImportDialog::~WordsImportDialog()
{
  _plugin->close();
}

int WordsImportDialog::exec()
{
  if (!_plugin->open(_file))
  {
    QMessageBox::critical(QApplication::activeWindow(), tr("Words import"), tr("Can't import data from selected file."));
    return QDialog::Rejected;
  }

  // vocabulary UI
  _ui.setupUi(this);
  // categories
  _ui.categories->setModel(&_categoriesModel);
  connect(_ui.categories->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WordsImportDialog::on_categoriesSelectionModel_selectionChanged);
  // fields
  _ui.fields->setModel(&_fieldsModel);
  createFieldEditors();
  _ui.fields->header()->setSectionResizeMode(static_cast<int>(WordsImportFieldsModel::Column::Name),     QHeaderView::ResizeToContents);
  _ui.fields->header()->setSectionResizeMode(static_cast<int>(WordsImportFieldsModel::Column::Language), QHeaderView::ResizeToContents);
  _ui.fields->header()->setSectionResizeMode(static_cast<int>(WordsImportFieldsModel::Column::Editor),   QHeaderView::Stretch);
  // preview
  preparePreviewColumns();

  // plugin UI
  new QBoxLayout(QBoxLayout::LeftToRight, _ui.sourceGroup);
  _plugin->setupUI(_ui.sourceGroup);

  return QDialog::exec();
}

void WordsImportDialog::accept()
{
  importData(Target::Vocabulary);
  if (!_interrupt)
  {
    QDialog::accept();
  }
}

void WordsImportDialog::createFieldEditors()
{
  _ui.fields->setItemDelegateForColumn(static_cast<int>(WordsImportFieldsModel::Column::Editor), &_editorDelegate);

  for (auto row = 0; row < _fieldsModel.rowCount(); ++row)
  {
    auto persistentEditor = true;

    const auto fieldId = _vocabulary->fieldId(row);
    if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttribute::BuiltIn))
    {
      const auto builtIn = _vocabulary->fieldBuiltIn(fieldId);
      if (builtIn == VocabularyDatabase::FieldBuiltIn::Enabled)
      {
        persistentEditor = false;
      }
    }

    if (persistentEditor)
    {
      const auto index = _fieldsModel.index(row, static_cast<int>(WordsImportFieldsModel::Column::Editor));
      _ui.fields->openPersistentEditor(index);
    }
  }
}

void WordsImportDialog::enableControls() const
{
  const auto categorySelection = _ui.categories->selectionModel();

  _ui.okButton->setEnabled(!_importing && categorySelection->hasSelection());
  if (_importing)
  {
    _ui.cancelButton->setText(tr("Stop"));
  }
  else
  {
    _ui.cancelButton->setText(tr("Cancel"));
  }
}

void WordsImportDialog::importData(const Target &target)
{
  _importing = true;
  enableControls();
  _interrupt = false;

  // setup progessbar
  _ui.progress->setMaximum(_plugin->recordCount());

  // patterns
  QStringList patterns;
  for (auto pattern = 0; pattern < _vocabulary->fieldCount(); ++pattern)
  {
    const auto index = _fieldsModel.index(pattern, static_cast<int>(WordsImportFieldsModel::Column::Editor));
    const auto data  = _fieldsModel.data(index, Qt::EditRole).toString();
    patterns.append(data);
  }

  const auto marks       = _plugin->marks();
  const auto recordCount = _plugin->recordCount();

  quintptr categoryId;
  switch (target)
  {
    case Target::Preview:
      _ui.preview->setRowCount(recordCount);
      break;
    case Target::Vocabulary:
      const auto category = _ui.categories->currentIndex();
      categoryId          = _vocabulary->categoryId(category.row());
  }

  auto skipCount = 0;
  for (auto record = 0; record < recordCount && !_interrupt; ++record)
  {
    // get mark data
    auto skip = false;
    QStringList markData;
    for (const auto &mark : marks)
    {
      const auto data = _plugin->recordData(record, mark);
      if (_ui.skipPartialRecords->isChecked() && data.isEmpty())
      {
        skip = true;
        ++skipCount;
        break;
      }
      markData.append(data);
    }
    if (skip)
    {
      continue;
    }

    // get data
    QStringList data;
    for (auto column = 0; column < _vocabulary->fieldCount(); ++column)
    {
      auto text = patterns.at(column);

      for (auto mark = 0; mark < marks.count(); ++mark)
      {
        text.replace(marks.at(mark), markData.at(mark));
      }
      data.append(text);
    }

    // insert data into target
    switch (target)
    {
      case Target::Preview:
        for (auto column = 0; column < _vocabulary->fieldCount(); ++column)
        {
          QTableWidgetItem *tableItem = Q_NULLPTR;

          const auto fieldId = _vocabulary->fieldId(column);
          switch (_vocabulary->fieldType(fieldId))
          {
            case VocabularyDatabase::FieldType::LineEdit:
              tableItem = new QTableWidgetItem(data.at(column));
              break;
            case VocabularyDatabase::FieldType::CheckBox:
              tableItem = new QTableWidgetItem();
              tableItem->setCheckState(static_cast<Qt::CheckState>(data.at(column).toUInt()));
              tableItem->setFlags(tableItem->flags() | Qt::ItemIsUserCheckable);
              break;
            case VocabularyDatabase::FieldType::SpinBox:
              tableItem = new QTableWidgetItem(data.at(column));
          }

          tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);
          _ui.preview->setItem(record - skipCount, column, tableItem);
        }
        break;
      case Target::Vocabulary:
        _vocabulary->addRecord(categoryId, data);
    }

    // progress
    if (record % IMPORT_REFRESHINTERVAL == 0)
    {
      _ui.progress->setValue(record);
      QCoreApplication::processEvents();
    }
  }

  if (target == Target::Preview && skipCount > 0)
  {
    _ui.preview->setRowCount(recordCount - skipCount);
  }

  // setup progressbar
  _ui.progress->setValue(0);
  _ui.progress->setMaximum(1);

  _importing = false;
  enableControls();
}

void WordsImportDialog::preparePreviewColumns() const
{
  QStringList columns;
  for (auto fieldId : _vocabulary->fieldIds())
  {
    columns.append(_vocabulary->fieldName(fieldId));
  }

  _ui.preview->setColumnCount(columns.count());
  _ui.preview->setHorizontalHeaderLabels(columns);
  for (auto column = 0; column < _ui.preview->horizontalHeader()->count(); ++column)
  {
    _ui.preview->horizontalHeader()->setSectionResizeMode(column, QHeaderView::Stretch);
  }
}

void WordsImportDialog::reject()
{
  if (_importing)
  {
    _interrupt = true;
  }
  else
  {
    QDialog::reject();
  }
}

void WordsImportDialog::on_previewRefresh_clicked(bool checked /* false */)
{
  importData(Target::Preview);
}

void WordsImportDialog::on_categoriesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const
{
  enableControls();
}