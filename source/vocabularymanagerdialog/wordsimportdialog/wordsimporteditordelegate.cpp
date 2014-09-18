#include "vocabularymanagerdialog/wordsimportdialog/wordsimporteditordelegate.h"

#include "vocabularymanagerdialog/prioritydelegate.h"
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>

QWidget *WordsImportEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int iFieldId = _vVocabulary->fieldId(index.row());
	if (_vVocabulary->fieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
		VocabularyDatabase::FieldBuiltIn efbBuiltIn = _vVocabulary->fieldBuiltIn(iFieldId);
		if (efbBuiltIn == VocabularyDatabase::FieldBuiltInPriority) {
			QSpinBox *qsbEditor = new QSpinBox(parent);
			qsbEditor->setMinimum(PriorityDelegate::RECORD_PRIORITY_MIN);
			qsbEditor->setMaximum(PriorityDelegate::RECORD_PRIORITY_MAX);
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