#include "vocabularymanagerdialog/wordsimportdialog/wordsimporteditordelegate.h"

#include "vocabularymanagerdialog/prioritydelegate.h"
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>

WordsImportEditorDelegate::WordsImportEditorDelegate(const Vocabulary *vocabulary, QObject *parent /* nullptr */) : QStyledItemDelegate(parent), _vocabulary(vocabulary)
{
}

WordsImportEditorDelegate::~WordsImportEditorDelegate()
{
}

QWidget *WordsImportEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	const quint8 fieldId = _vocabulary->fieldId(index.row());
	if (_vocabulary->fieldHasAttribute(fieldId, VocabularyDatabase::FieldAttributeBuiltIn))
	{
		const VocabularyDatabase::FieldBuiltIn builtIn = _vocabulary->fieldBuiltIn(fieldId);
		if (builtIn == VocabularyDatabase::FieldBuiltInPriority)
		{
			QSpinBox *editor = new QSpinBox(parent);
			editor->setMinimum(PriorityDelegate::RECORD_PRIORITY_MIN);
			editor->setMaximum(PriorityDelegate::RECORD_PRIORITY_MAX);
			return editor;
		}
	}
	else
	{
		QLineEdit *editor = new QLineEdit(parent);
		return editor;
	}

	return nullptr;
}