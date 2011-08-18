#include "common/vocabularyopenprogressdialog.h"

const void VocabularyOpenProgressDialog::on_vVocabulary_SetOpenProgressValue(const int &pValue) const
{
	_qdvopVocabularyOpenProgress.qpbProgress->setValue(pValue);
} // on_vVocabulary_SetOpenProgressValue

const void VocabularyOpenProgressDialog::on_vVocabulary_SetOpenProgressMax(const int &pMax) const
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
	connect(pVocabulary, SIGNAL(SetOpenProgressValue(const int &)), SLOT(on_vVocabulary_SetOpenProgressValue(const int &)));
	connect(pVocabulary, SIGNAL(SetOpenProgressMax(const int &)), SLOT(on_vVocabulary_SetOpenProgressMax(const int &)));
	connect(pVocabulary, SIGNAL(SetVocabularyName(const QString &)), SLOT(on_vVocabulary_SetVocabularyName(const QString &)));
} // VocabularyOpenProgressDialog