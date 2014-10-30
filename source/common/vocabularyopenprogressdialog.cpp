#include "common/vocabularyopenprogressdialog.h"

const void VocabularyOpenProgressDialog::on_vVocabulary_SetOpenProgressValue(quint8 pValue) const
{
	_qdvopVocabularyOpenProgress.qpbProgress->setValue(pValue);
} // on_vVocabulary_SetOpenProgressValue

const void VocabularyOpenProgressDialog::on_vVocabulary_SetOpenProgressMax(quint8 pMax) const
{
	_qdvopVocabularyOpenProgress.qpbProgress->setMaximum(pMax);
} // on_vVocabulary_SetOpenProgressMax

const void VocabularyOpenProgressDialog::on_vVocabulary_SetVocabularyName(const QString &pName) const
{
	_qdvopVocabularyOpenProgress.qlLabel->setText(tr("Opening vocabulary %1...").arg(pName));
} // on_vVocabulary_SetVocabularyName

VocabularyOpenProgressDialog::VocabularyOpenProgressDialog(const Vocabulary *pVocabulary, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags | Qt::FramelessWindowHint)
{
	_qdvopVocabularyOpenProgress.setupUi(this);

	// connect to vocabulary database
	connect(pVocabulary, SIGNAL(setOpenProgressValue(quint8)), SLOT(on_vVocabulary_SetOpenProgressValue(quint8)));
	connect(pVocabulary, SIGNAL(setOpenProgressMax(quint8)), SLOT(on_vVocabulary_SetOpenProgressMax(quint8)));
	connect(pVocabulary, SIGNAL(setVocabularyName(const QString &)), SLOT(on_vVocabulary_SetVocabularyName(const QString &)));
} // VocabularyOpenProgressDialog