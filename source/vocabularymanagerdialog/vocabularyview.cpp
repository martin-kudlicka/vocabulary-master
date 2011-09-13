#include "vocabularymanagerdialog/vocabularyview.h"

#ifndef FREE
#include <QtGui/QMouseEvent>
#include <QtGui/QSpinBox>
#include <QtCore/QCoreApplication>

void VocabularyView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex qmiOld = currentIndex();
    QTableView::mousePressEvent(event);
    QModelIndex qmiNew = currentIndex();

    if (qmiOld != qmiNew && event->button() == Qt::LeftButton) {
        int iFieldId = _vVocabulary->GetFieldId(qmiNew.column());
        VocabularyDatabase::qfFieldAttributes qfaAttributes = _vVocabulary->GetFieldAttributes(iFieldId);
        if (qfaAttributes & VocabularyDatabase::FieldAttributeBuiltIn) {
            VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
            if (efbBuiltIn == VocabularyDatabase::FieldBuiltInPriority) {
                QSpinBox *qsbEditor = qobject_cast<QSpinBox *>(indexWidget(qmiNew));
                QPoint qpEditor = qsbEditor->mapFromParent(event->pos());

                QMouseEvent qmePress(QEvent::MouseButtonPress, qpEditor, Qt::LeftButton, Qt::LeftButton, 0);
                QCoreApplication::sendEvent(qsbEditor, &qmePress);
                QMouseEvent qmeRelease(QEvent::MouseButtonRelease, qpEditor, Qt::LeftButton, Qt::LeftButton, 0);
                QCoreApplication::sendEvent(qsbEditor, &qmeRelease);
            } // if
        } // if
    } // if
} // mousePressEvent
#endif

VocabularyView::VocabularyView(
#ifndef FREE
    Vocabulary *pVocabulary,
#endif
    QWidget *pParent /* NULL */) : QTableView(pParent)
{
#ifndef FREE
    _vVocabulary = pVocabulary;
#endif
} // VocabularyView