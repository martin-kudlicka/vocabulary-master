#include "vocabularymanagerdialog/wordsimportdialog/wordsimporteditordelegate.h"

#include "vocabularymanagerdialog/spinboxdelegate.h"
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>

QWidget *WordsImportEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int iFieldId = _vVocabulary->GetFieldId(index.row());
	VocabularyDatabase::qfFieldAttributes qfaAttributes = _vVocabulary->GetFieldAttributes(iFieldId);
	if (qfaAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
		VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
		if (efbBuiltIn == VocabularyDatabase::FieldBuiltInPriority) {
			QSpinBox *qsbEditor = new QSpinBox(parent);
			qsbEditor->setMinimum(SpinBoxDelegate::RECORD_PRIORITY_MIN);
			qsbEditor->setMaximum(SpinBoxDelegate::RECORD_PRIORITY_MAX);
			return qsbEditor;
		} // if
	} else {
		QLineEdit *qleEditor = new QLineEdit(parent);
		return qleEditor;
	} // if else

	return NULL;
} // createEditor

WordsImportEditorDelegate::WordsImportEditorDelegate(const Vocabulary *pVocabulary, QObject *pParent /* NULL */) : QStyledItemDelegate(pParent)
{
	_vVocabulary = pVocabulary;
} // WordsImportEditorDelegate