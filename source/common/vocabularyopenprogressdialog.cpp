#include "pch.h"
#include "common/vocabularyopenprogressdialog.h"

#include "vocabulary.h"

VocabularyOpenProgressDialog::VocabularyOpenProgressDialog(const Vocabulary *vocabulary, QWidget *parent) : QDialog(parent, Qt::FramelessWindowHint)
{
  _ui.setupUi(this);

  // connect to vocabulary database
  connect(vocabulary, &Vocabulary::setOpenProgressValue, this, &VocabularyOpenProgressDialog::on_vocabulary_setOpenProgressValue);
  connect(vocabulary, &Vocabulary::setOpenProgressMax,   this, &VocabularyOpenProgressDialog::on_vocabulary_setOpenProgressMax);
  connect(vocabulary, &Vocabulary::setVocabularyName,    this, &VocabularyOpenProgressDialog::on_vocabulary_setVocabularyName);
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