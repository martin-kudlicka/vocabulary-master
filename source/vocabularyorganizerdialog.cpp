#include "vocabularyorganizerdialog.h"
#ifndef EDITION_TRY
# include <QtWidgets/QFileDialog>
#endif

const char   *VOCABULARY_FILTER = QT_TRANSLATE_NOOP("VocabularyOrganizerDialog", "Vocabulary (*.sl3)");
const QString VOCABULARY_SUFFIX = "sl3";

VocabularyOrganizerDialog::VocabularyOrganizerDialog(VocabularyOrganizer *organizer, QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags), _organizer(organizer), _model(organizer, this)
{
	_ui.setupUi(this);
#ifdef EDITION_FREE
	enableControls();
#elif defined EDITION_TRY
	delete _ui.open;
#endif

	_ui.vocabularies->setModel(&_model);

	_ui.vocabularies->header()->setSectionResizeMode(VocabularyOrganizerModel::ColumnVocabularyFile, QHeaderView::Stretch);
#if !defined(EDITION_FREE) && !defined(EDITION_TRY)
	_ui.vocabularies->header()->setSectionResizeMode(VocabularyOrganizerModel::ColumnEnabled, QHeaderView::ResizeToContents);
#endif

	connect(_ui.vocabularies->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), SLOT(on_vocabulariesSelectionModel_selectionChanged(const QItemSelection &, const QItemSelection &)));
} // VocabularyOrganizerDialog

VocabularyOrganizerDialog::~VocabularyOrganizerDialog()
{
}

#ifndef EDITION_TRY
void VocabularyOrganizerDialog::accept()
{
	_organizer->saveAll();
	QDialog::accept();
} // accept
#endif

#ifdef EDITION_FREE
void VocabularyOrganizerDialog::enableControls() const
{
	_ui.new2->setEnabled(_organizer->vocabularyCount() < Settings::EDITION_FREE_VOCABULARIES_MAX);
	_ui.open->setEnabled(_organizer->vocabularyCount() < Settings::EDITION_FREE_VOCABULARIES_MAX);
} // enableControls
#endif

#ifndef EDITION_TRY
QString VocabularyOrganizerDialog::openPath() const
{
	if (_organizer->vocabularyCount() > 0)
	{
		VocabularyOrganizer::VocabularyInfo vocabularyInfo = _organizer->vocabularyInfo(0);
		return QFileInfo(vocabularyInfo.vocabularyInfo.filePath).absolutePath();
	}
	else
	{
		return QDir::homePath();
	} // if else
} // openPath

void VocabularyOrganizerDialog::reject()
{
	accept();
} // reject
#endif

void VocabularyOrganizerDialog::on_close_clicked(bool checked /* false */)
{
	const quint8 index = _ui.vocabularies->currentIndex().row();

	_organizer->remove(index);
	_model.RemoveRow(index);

	on_vocabulariesSelectionModel_selectionChanged(QItemSelection(), QItemSelection());

#ifdef EDITION_FREE
	enableControls();
#endif
} // on_close_clicked

void VocabularyOrganizerDialog::on_new2_clicked(bool checked /* false */)
{
#ifndef EDITION_TRY
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
		} // if else

		_organizer->addNew(vocabularyFile);
		_model.AddRow();

#ifdef EDITION_FREE
		enableControls();
#endif
	} // if
#else
	_organizer->addNew();
	_model.AddRow();
#endif
} // on_new2_clicked

#ifndef EDITION_TRY
void VocabularyOrganizerDialog::on_open_clicked(bool checked /* false */)
{
	const QString vocabularyFile = QFileDialog::getOpenFileName(this, tr("Open vocabulary"), openPath(), tr(VOCABULARY_FILTER));
	if (!vocabularyFile.isEmpty())
	{
		VocabularyOrganizer::VocabularyInfo vocabularyInfo;
		vocabularyInfo.vocabularyInfo.filePath = vocabularyFile;
#ifndef EDITION_FREE
		vocabularyInfo.vocabularyInfo.enabled = true;
#endif

		_organizer->addExisting(&vocabularyInfo, this);
		_model.AddRow();

#ifdef EDITION_FREE
		enableControls();
#endif
	} // if
} // on_open_clicked
#endif

void VocabularyOrganizerDialog::on_vocabulariesSelectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	_ui.close->setEnabled(_ui.vocabularies->currentIndex().isValid());
} // on_vocabulariesSelectionModel_selectionChanged