#include "vocabularymanagerdialog/vocabularyview.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>

void VocabularyView::keyPressEvent(QKeyEvent *event)
{
	if (event->matches(QKeySequence::Paste)) {
		model()->setData(currentIndex(), QApplication::clipboard()->text());
	} // if
} // keyPressEvent

VocabularyView::VocabularyView(QWidget *pParent /* NULL */) : QTableView(pParent)
{
} // VocabularyView