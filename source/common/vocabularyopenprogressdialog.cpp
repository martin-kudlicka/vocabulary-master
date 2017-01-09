#include "common/vocabularyopenprogressdialog.h"

VocabularyOpenProgressDialog::VocabularyOpenProgressDialog(const Vocabulary *vocabulary, QWidget *parent /* Q_NULLPTR */, Qt::WindowFlags flags /* 0 */) : QDialog(parent, flags | Qt::FramelessWindowHint)
{
	_ui.setupUi(this);

	// connect to vocabulary database
	connect(vocabulary, SIGNAL(setOpenProgressValue(quintptr)), SLOT(on_vocabulary_setOpenProgressValue(quintptr)));
	connect(vocabulary, SIGNAL(setOpenProgressMax(quintptr)), SLOT(on_vocabulary_setOpenProgressMax(quintptr)));
	connect(vocabulary, SIGNAL(setVocabularyName(const QString &)), SLOT(on_vocabulary_setVocabularyName(const QString &)));
}

VocabularyOpenProgressDialog::~VocabularyOpenProgressDialog()
{
}

void VocabularyOpenProgressDialog::on_vocabulary_setOpenProgressValue(quintptr value) const
{
	_ui.progress->setValue(value);
}

void VocabularyOpenProgressDialog::on_vocabulary_setOpenProgressMax(quintptr max) const
{
	_ui.progress->setMaximum(max);
}

void VocabularyOpenProgressDialog::on_vocabulary_setVocabularyName(const QString &name) const
{
	_ui.label->setText(tr("Opening vocabulary %1...").arg(name));
}