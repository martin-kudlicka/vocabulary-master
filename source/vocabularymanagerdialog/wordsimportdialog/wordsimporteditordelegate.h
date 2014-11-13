#ifndef WORDSIMPORTEDITORDELEGATE_H
#define WORDSIMPORTEDITORDELEGATE_H

#include <QtWidgets/QStyledItemDelegate>
#include "vocabulary.h"

class WordsImportEditorDelegate : public QStyledItemDelegate
{
	public:
		         WordsImportEditorDelegate(const Vocabulary *vocabulary, QObject *parent = NULL);
		virtual ~WordsImportEditorDelegate();

	private:
		const Vocabulary *_vocabulary;

		virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
}; // WordsImportEditorDelegate

#endif // WORDSIMPORTEDITORDELEGATE_H