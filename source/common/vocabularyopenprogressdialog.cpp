#include "common/vocabularyopenprogressdialog.h"

VocabularyOpenProgressDialog::VocabularyOpenProgressDialog(const Vocabulary *vocabulary, QWidget *parent /* NULL */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags | Qt::FramelessWindowHint)
{
	_ui.setupUi(this);

	// connect to vocabulary database
	connect(vocabulary, SIGNAL(setOpenProgressValue(quint8)), SLOT(on_vocabulary_setOpenProgressValue(quint8)));
	connect(vocabulary, SIGNAL(setOpenProgressMax(quint8)), SLOT(on_vocabulary_setOpenProgressMax(quint8)));
	connect(vocabulary, SIGNAL(setVocabularyName(const QString &)), SLOT(on_vocabulary_setVocabularyName(const QString &)));
} // VocabularyOpenProgressDialog

VocabularyOpenProgressDialog::~VocabularyOpenProgressDialog()
{
}

void VocabularyOpenProgressDialog::on_vocabulary_setOpenProgressValue(quint8 value) const
{
	_ui.progress->setValue(value);
} // on_vocabulary_setOpenProgressValue

void VocabularyOpenProgressDialog::on_vocabulary_setOpenProgressMax(quint8 max) const
{
	_ui.progress->setMaximum(max);
} // on_vocabulary_setOpenProgressMax

void VocabularyOpenProgressDialog::on_vocabulary_setVocabularyName(const QString &name) const
{
	_ui.label->setText(tr("Opening vocabulary %1...").arg(name));
} // on_vocabulary_setVocabularyName