#include "vocabularyorganizerdialog.h"

#include "vocabularyorganizer.h"
#include <QtWidgets/QFileDialog>

const auto VOCABULARY_FILTER = QT_TRANSLATE_NOOP("VocabularyOrganizerDialog", "Vocabulary (*.sl3)");
const auto VOCABULARY_SUFFIX = "sl3";

VocabularyOrganizerDialog::VocabularyOrganizerDialog(VocabularyOrganizer *organizer, QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _organizer(organizer), _model(organizer, this)
{
  _ui.setupUi(this);

  _ui.vocabularies->setModel(&_model);

  _ui.vocabularies->header()->setSectionResizeMode(static_cast<int>(VocabularyOrganizerModel::Column::VocabularyFile), QHeaderView::Stretch);
  _ui.vocabularies->header()->setSectionResizeMode(static_cast<int>(VocabularyOrganizerModel::Column::Enabled),        QHeaderView::ResizeToContents);

  connect(_ui.vocabularies->selectionModel(), &QItemSelectionModel::selectionChanged, this, &VocabularyOrganizerDialog::on_vocabulariesSelectionModel_selectionChanged);
}

void VocabularyOrganizerDialog::accept()
{
  _organizer->saveAll();
  QDialog::accept();
}

QString VocabularyOrganizerDialog::openPath() const
{
  if (_organizer->vocabularyCount() > 0)
  {
    auto vocabularyInfo = _organizer->vocabularyInfo(0);
    return QFileInfo(vocabularyInfo.vocabularyInfo.filePath).absolutePath();
  }
  else
  {
    return QDir::homePath();
  }
}

void VocabularyOrganizerDialog::reject()
{
  accept();
}

void VocabularyOrganizerDialog::on_close_clicked(bool checked /* false */)
{
  const auto index = _ui.vocabularies->currentIndex().row();

  _organizer->remove(index);
  _model.removeRow(index);

  _ui.vocabularies->selectionModel()->select(QItemSelection(), QItemSelectionModel::Clear);
}

void VocabularyOrganizerDialog::on_new2_clicked(bool checked /* false */)
{
  QFileDialog newVocabulary(this, tr("Create new vocabulary"), openPath(), tr(VOCABULARY_FILTER));
  newVocabulary.setAcceptMode(QFileDialog::AcceptSave);
  if (newVocabulary.exec() == QDialog::Accepted)
  {
    const QFileInfo vocabularyFileInfo(newVocabulary.selectedFiles().at(0));
    QString vocabularyFile;
    if (vocabularyFileInfo.suffix() != VOCABULARY_SUFFIX)
    {
      vocabularyFile = newVocabulary.selectedFiles().at(0) + "." + VOCABULARY_SUFFIX;
    }
    else
    {
      vocabularyFile = newVocabulary.selectedFiles().at(0);
    }

    _organizer->addNew(vocabularyFile);
    _model.addRow();
  }
}

void VocabularyOrganizerDialog::on_open_clicked(bool checked /* false */)
{
  const auto vocabularyFile = QFileDialog::getOpenFileName(this, tr("Open vocabulary"), openPath(), tr(VOCABULARY_FILTER));
  if (!vocabularyFile.isEmpty())
  {
    VocabularyOrganizer::VocabularyInfo vocabularyInfo;
    vocabularyInfo.vocabularyInfo.filePath = vocabularyFile;
    vocabularyInfo.vocabularyInfo.enabled  = true;

    _organizer->addExisting(&vocabularyInfo, this);
    _model.addRow();
  }
}

void VocabularyOrganizerDialog::on_vocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  _ui.close->setEnabled(_ui.vocabularies->currentIndex().isValid());
}