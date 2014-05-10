#include "vocabularymanagerdialog/vocabularyview.h"

#ifndef EDITION_FREE
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSpinBox>
#include <QtCore/QCoreApplication>

bool VocabularyView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == viewport() && event->type() == QEvent::Leave) {
        update(_qmiMoveOld);
        _qmiMoveOld = QModelIndex();
    } // if

    return QTableView::eventFilter(watched, event);
} // eventFilter

void VocabularyView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex qmiMouse = indexAt(event->pos());
    if (_qmiMoveOld.isValid() && _qmiMoveOld != qmiMouse) {
        update(_qmiMoveOld);
    } // if
    _qmiMoveOld = qmiMouse;

    int iFieldId = _vVocabulary->GetFieldId(qmiMouse.column());
    if (_vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
        VocabularyDatabase::eFieldBuiltIn efbBuiltIn = _vVocabulary->GetFieldBuiltIn(iFieldId);
        if (efbBuiltIn == VocabularyDatabase::FieldBuiltInPriority) {
            update(qmiMouse);
        } // if
    } // if

    QTableView::mouseMoveEvent(event);
} // mouseMoveEvent

void VocabularyView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex qmiOld = currentIndex();
    QTableView::mousePressEvent(event);
    QModelIndex qmiNew = currentIndex();

    if (qmiOld != qmiNew && event->button() == Qt::LeftButton) {
        int iFieldId = _vVocabulary->GetFieldId(qmiNew.column());
        if (_vVocabulary->FieldHasAttribute(iFieldId, VocabularyDatabase::FieldAttributeBuiltIn)) {
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
#ifndef EDITION_FREE
    Vocabulary *pVocabulary,
#endif
    QWidget *pParent /* NULL */) : QTableView(pParent)
{
#ifndef EDITION_FREE
    _vVocabulary = pVocabulary;

    setMouseTracking(true);
    viewport()->installEventFilter(this);
#endif
} // VocabularyView