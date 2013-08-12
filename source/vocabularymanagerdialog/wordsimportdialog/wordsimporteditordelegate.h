#ifndef WORDSIMPORTEDITORDELEGATE_H
#define WORDSIMPORTEDITORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include "vocabulary.h"

class WordsImportEditorDelegate : public QStyledItemDelegate
{
	public:
		WordsImportEditorDelegate(const Vocabulary *pVocabulary, QObject *pParent = NULL);

	private:
		const Vocabulary *_vVocabulary;

		virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
}; // WordsImportEditorDelegate

#endif // WORDSIMPORTEDITORDELEGATE_H