#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <QtGui/QTableView>

class VocabularyView : public QTableView
{
	public:
		VocabularyView(QWidget *pParent = NULL);

	private:
		void keyPressEvent(QKeyEvent *event);
}; // VocabularyView

#endif // VOCABULARYVIEW_H