#include "vocabularyorganizerdialog.h"

VocabularyOrganizerDialog::VocabularyOrganizerDialog(const VocabularyOrganizer *pOrganizer, QWidget *pParent /* NULL */, Qt::WindowFlags pFlags /* 0 */) : QDialog(pParent, pFlags), _vomModel(pOrganizer)
{
	_voOrganizer = pOrganizer;

	_qdvmOrganizer.setupUi(this);

	_qdvmOrganizer.qtvVocabularies->setModel(&_vomModel);
} // VocabularyOrganizerDialog